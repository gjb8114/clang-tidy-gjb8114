#include "UnsignedComparisonCheck.h"

using namespace clang::ast_matchers;

namespace clang::tidy::gjb8114 {
void UnsignedComparisonCheck::registerMatchers(MatchFinder *Finder) {
  Finder->addMatcher(
      binaryOperator(
          unless(isExpansionInSystemHeader()),
          anyOf(allOf(hasOperatorName(">="), hasLHS(expr().bind("var")),
                      hasRHS(ignoringParenImpCasts(integerLiteral(equals(0))))),
                allOf(hasOperatorName("<"), hasLHS(expr().bind("var")),
                      hasRHS(ignoringParenImpCasts(integerLiteral(equals(0))))),
                allOf(hasOperatorName("<="),
                      hasLHS(ignoringParenImpCasts(integerLiteral(equals(0)))),
                      hasRHS(expr().bind("var"))),
                allOf(hasOperatorName(">"),
                      hasLHS(ignoringParenImpCasts(integerLiteral(equals(0)))),
                      hasRHS(expr().bind("var")))))
          .bind("binaryOp"),
      this);
}

void UnsignedComparisonCheck::check(const MatchFinder::MatchResult &Result) {
  const auto *BinOp = Result.Nodes.getNodeAs<BinaryOperator>("binaryOp");
  const auto *Var = Result.Nodes.getNodeAs<Expr>("var");

  if (!BinOp || !Var)
    return;

  auto Loc = BinOp->getBeginLoc();
  if (!Loc.isValid())
    return;

  auto IsUnsigned = [&](const Expr *E) -> bool {
    QualType Type = E->getType();
    return Type->isUnsignedIntegerType();
  };

  if (IsUnsigned(Var)) {
    diag(BinOp->getExprLoc(), "禁止对无符号数进行无意义比较")
        << BinOp->getSourceRange();
  }
}
} // namespace clang::tidy::gjb8114