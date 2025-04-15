#include "InvalidPointerUseCheck.h"

#include "utils/RawPointerDeallocTracker.h"
#include "utils/RawPointerUseTracker.h"

using namespace clang::ast_matchers;

namespace clang::tidy::gjb8114 {

void InvalidPointerUseCheck::registerMatchers(MatchFinder *Finder) {
  Finder->addMatcher(
      declStmt(has(varDecl(hasAncestor(
                               functionDecl(unless(isExpansionInSystemHeader()),
                                            isDefinition())
                                   .bind("func")),
                           hasType(isAnyPointer()), hasInitializer(isNull()))
                       .bind("ptrVar")))
          .bind("nullInitPtrDecl"),
      this);
  auto PointerVarDecl =
      varDecl(hasType(isAnyPointer()),
              hasAncestor(functionDecl(unless(isExpansionInSystemHeader()),
                                       isDefinition())
                              .bind("func")))
          .bind("ptrVar");
  auto PointerDeclRef = declRefExpr(to(PointerVarDecl));
  Finder->addMatcher(
      binaryOperator(hasOperatorName("="),
                     hasLHS(ignoringParenImpCasts(PointerDeclRef)),
                     hasRHS(isNull()))
          .bind("assignNull"),
      this);
  Finder->addMatcher(callExpr(callee(functionDecl(hasName("free"))),
                              hasArgument(0, PointerDeclRef))
                         .bind("free"),
                     this);
  Finder->addMatcher(
      cxxDeleteExpr(has(ignoringParenImpCasts(PointerDeclRef))).bind("delete"),
      this);
}

static const char *Description = "禁止使用或释放未分配或已释放的指针";

void InvalidPointerUseCheck::check(const MatchFinder::MatchResult &Result) {
  const Stmt *NullInitPtr = Result.Nodes.getNodeAs<DeclStmt>("nullInitPtrDecl");
  const Stmt *AssignNull = Result.Nodes.getNodeAs<BinaryOperator>("assignNull");
  const Stmt *Free = Result.Nodes.getNodeAs<CallExpr>("free");
  const Stmt *Delete = Result.Nodes.getNodeAs<CXXDeleteExpr>("delete");
  const auto *Func = Result.Nodes.getNodeAs<FunctionDecl>("func");
  auto *Block = Func->getBody();
  auto *VD = Result.Nodes.getNodeAs<VarDecl>("ptrVar");
  llvm::SmallPtrSet<const Stmt *, 8> Usages;
  for (const auto *Trigger : {NullInitPtr, AssignNull}) {
    if (Trigger) {
      utils::RawPointerUseTracker Tracker{Result.Context};
      Tracker.findUseAfter(Usages, Block, Trigger, VD);
    }
  }
  for (const auto *Trigger : {NullInitPtr, AssignNull, Free, Delete}) {
    if (Trigger) {
      utils::RawPointerDeallocTracker Tracker{Result.Context};
      Tracker.findUseAfter(Usages, Block, Trigger, VD);
    }
  }
  for (const auto *Usage : Usages)
    emitDiagnostics(Usage);
}

void InvalidPointerUseCheck::emitDiagnostics(const Stmt *Usage) {
  if (Reported.count(Usage))
    return;
  Reported.insert(Usage);
  diag(Usage->getBeginLoc(), Description);
}
} // namespace clang::tidy::gjb8114