#include "MallocNullCheck.h"
#include "matchers/compose.h"

#include <algorithm>

namespace clang::tidy::gjb8114 {
using namespace clang::ast_matchers;
void MallocNullCheck::registerMatchers(MatchFinder *Finder) {
  Finder->addMatcher(
      functionDecl(unless(isExpansionInSystemHeader()), isDefinition())
          .bind("func"),
      this);
  Finder->addMatcher(
      varDecl(matchers::hasMallocInitializer()).bind("mallocVar"), this);
  Finder->addMatcher(
      callExpr(callee(functionDecl(anyOf(hasName("malloc"), hasName("calloc"),
                                         hasName("realloc")))),
               hasParent(castExpr(hasParent(binaryOperator(
                   hasLHS(declRefExpr(to(varDecl().bind("mallocPtr")))))))),
               unless(hasAncestor(varDecl())))
          .bind("mallocCall"),
      this);
  Finder->addMatcher(matchers::derefPointerExpr().bind("deref"), this);
  Finder->addMatcher(matchers::subscriptPointerExpr().bind("subscript"), this);
  Finder->addMatcher(
      binaryOperator(anyOf(hasOperatorName("!="), hasOperatorName("==")),
                     hasEitherOperand(ignoringParenImpCasts(
                         declRefExpr(to(varDecl().bind("checkedPtr"))))),
                     hasEitherOperand(isNull()))
          .bind("nullCheck"),
      this);
  Finder->addMatcher(castExpr(hasType(booleanType()),
                              hasSourceExpression(expr(hasType(pointerType()))))
                         .bind("boolCast"),
                     this);
  // C: matches !!p, !p
  Finder->addMatcher(
      unaryOperator(hasOperatorName("!"),
                    hasUnaryOperand(ignoringParenImpCasts(declRefExpr(
                        to(varDecl(hasType(isAnyPointer())).bind("boolPtr"))))))
          .bind("cUnaryNullCheck"),
      this);
  // C: matches if (p)
  Finder->addMatcher(
      ifStmt(hasCondition(ignoringParenImpCasts(declRefExpr(
                 to(varDecl(hasType(isAnyPointer())).bind("boolPtr"))))))
          .bind("cIfNullCheck"),
      this);
}

void MallocNullCheck::check(const MatchFinder::MatchResult &Result) {
  SM = Result.SourceManager;
  if (auto *F = Result.Nodes.getNodeAs<FunctionDecl>("func")) {
    doCheck();
    return;
  }

  if (const auto *VD = Result.Nodes.getNodeAs<VarDecl>("mallocVar")) {
    PointerAllocates[VD] = true;
  }
  if (const auto *Call = Result.Nodes.getNodeAs<CallExpr>("mallocCall")) {
    const auto *VD = Result.Nodes.getNodeAs<VarDecl>("mallocPtr");
    PointerOps[VD].push_back(Call);
  }
  if (const auto *Deref = Result.Nodes.getNodeAs<UnaryOperator>("deref")) {
    const auto *DRE = dyn_cast<DeclRefExpr>(Deref->getSubExpr()->IgnoreCasts());
    const auto *VD = dyn_cast<VarDecl>(DRE->getDecl());
    PointerOps[VD].push_back(Deref);
  }
  if (const auto *S = Result.Nodes.getNodeAs<ArraySubscriptExpr>("subscript")) {
    const auto *DRE = dyn_cast<DeclRefExpr>(S->getLHS()->IgnoreCasts());
    const auto *VD = dyn_cast<VarDecl>(DRE->getDecl());
    PointerOps[VD].push_back(S);
  }
  if (const auto *Check = Result.Nodes.getNodeAs<BinaryOperator>("nullCheck")) {
    const auto *VD = Result.Nodes.getNodeAs<VarDecl>("checkedPtr");
    PointerOps[VD].push_back(Check);
  }
  if (const auto *Cast = Result.Nodes.getNodeAs<CastExpr>("boolCast")) {
    const auto *DRE = dyn_cast<DeclRefExpr>(Cast->getSubExpr()->IgnoreCasts());
    const auto *VD = dyn_cast<VarDecl>(DRE->getDecl());
    PointerOps[VD].push_back(Cast);
  }
  if (const auto *Unary =
          Result.Nodes.getNodeAs<UnaryOperator>("cUnaryNullCheck")) {
    const auto *VD = Result.Nodes.getNodeAs<VarDecl>("boolPtr");
    PointerOps[VD].push_back(Unary);
  }
  if (const auto *If = Result.Nodes.getNodeAs<IfStmt>("cIfNullCheck")) {
    const auto *VD = Result.Nodes.getNodeAs<VarDecl>("boolPtr");
    PointerOps[VD].push_back(If);
  }
}

void MallocNullCheck::onEndOfTranslationUnit() {
  MatchCallback::onEndOfTranslationUnit();
  doCheck();
}

void MallocNullCheck::doCheck() {
  const char *Description = "禁止动态分配的指针变量未检查即使用";

  for (auto &[VD, Ops] : PointerOps) {
    bool Allocated = PointerAllocates.count(VD);
    bool Checked = false;
    std::ranges::sort(Ops, [this](auto *L, auto *R) {
      return SM->isBeforeInTranslationUnit(L->getBeginLoc(), R->getBeginLoc());
    });
    for (const auto *Op : Ops) {
      if (isa<CallExpr>(Op)) {
        Allocated = true;
        Checked = false;
      } else if (const auto *BO = dyn_cast<BinaryOperator>(Op)) {
        if (BO->getOpcode() == BO_NE || BO->getOpcode() == BO_EQ) {
          if (Allocated)
            Checked = true;
        }
      } else if (isa<CastExpr>(Op)) {
        if (Allocated)
          Checked = true;
      } else if (const auto *Unary = dyn_cast<UnaryOperator>(Op)) {
        if (Unary->getOpcode() == UO_LNot) {
          if (Allocated)
            Checked = true;
        } else if (Unary->getOpcode() == UO_Deref) {
          if (Allocated && !Checked) {
            diag(Unary->getOperatorLoc(), Description);
            break;
          }
        }
      } else if (isa<IfStmt>(Op)) {
        if (Allocated)
          Checked = true;
      } else if (const auto *S = dyn_cast<ArraySubscriptExpr>(Op)) {
        if (Allocated && !Checked) {
          diag(S->getBeginLoc(), Description);
          break;
        }
      }
    }
  }
  PointerOps.clear();
}
} // namespace clang::tidy::gjb8114