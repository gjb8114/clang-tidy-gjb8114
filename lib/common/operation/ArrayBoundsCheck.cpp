#include "ArrayBoundsCheck.h"

using namespace clang::ast_matchers;

namespace clang::tidy::gjb8114 {
void ArrayBoundsCheck::registerMatchers(MatchFinder *Finder) {
  Finder->addMatcher(
      arraySubscriptExpr(
          hasIndex(ignoringParenImpCasts(integerLiteral().bind("idx"))),
          hasBase(ignoringParenImpCasts(
              declRefExpr(hasDeclaration(decl().bind("array"))))))
          .bind("subscript"),
      this);

  auto ParamPtr = parmVarDecl(hasType(isAnyPointer())).bind("ptr");
  auto ParamBound = parmVarDecl().bind("bound");
  Finder->addMatcher(
      forStmt(
          hasCondition(binaryOperator(hasOperatorName("<="),
                                      hasLHS(ignoringParenImpCasts(declRefExpr(
                                          to(varDecl().bind("idxVar"))))),
                                      hasRHS(ignoringParenImpCasts(
                                          declRefExpr(to(ParamBound)))))
                           .bind("le")),
          hasBody(compoundStmt(hasDescendant(
              arraySubscriptExpr(hasBase(ignoringParenImpCasts(
                                     declRefExpr(to(ParamPtr)).bind("array"))),
                                 hasIndex(ignoringParenImpCasts(declRefExpr(
                                     to(varDecl(equalsBoundNode("idxVar")))))))
                  .bind("array_access")))))
          .bind("for"),
      this);
}

void ArrayBoundsCheck::check(const MatchFinder::MatchResult &Result) {
  const char *Description = "禁止数组越界使用";
  const auto *ConstantIndexSubscriptExpr =
      Result.Nodes.getNodeAs<ArraySubscriptExpr>("subscript");
  if (ConstantIndexSubscriptExpr) {
    const auto *Idx = Result.Nodes.getNodeAs<IntegerLiteral>("idx");
    const auto *Array = Result.Nodes.getNodeAs<DeclaratorDecl>("array");
    if (const auto *ArrayType = dyn_cast<ConstantArrayType>(
            Array->getType()->getAsArrayTypeUnsafe())) {
      uint64_t Size = ArrayType->getSize().getZExtValue();
      uint64_t Index = Idx->getValue().getZExtValue();

      if (Index >= Size) {
        diag(ConstantIndexSubscriptExpr->getExprLoc(), Description)
            << FixItHint::CreateReplacement(Idx->getSourceRange(),
                                            std::to_string(Size - 1));
      }
    }
  }

  const auto *ForLoop = Result.Nodes.getNodeAs<ForStmt>("for");
  if (ForLoop) {
    const auto *Le = Result.Nodes.getNodeAs<BinaryOperator>("le");
    auto OpLoc = Le->getOperatorLoc();
    if (!OpLoc.isValid() || OpLoc.isMacroID())
      return;
    auto OpLength = Le->getOpcodeStr().size();
    SourceRange ReplaceRange(OpLoc, OpLoc.getLocWithOffset(OpLength));
    diag(ForLoop->getCond()->getBeginLoc(), Description)
        << FixItHint::CreateReplacement(ReplaceRange, "<");
  }
}
} // namespace clang::tidy::gjb8114