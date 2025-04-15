#include "ArrayIndexCheck.h"
using namespace clang::ast_matchers;

namespace clang::tidy::gjb8114 {
void ArrayIndexCheck::registerMatchers(MatchFinder *Finder) {
  auto NegLiteral =
      unaryOperator(hasOperatorName("-"), hasUnaryOperand(integerLiteral()));
  Finder->addMatcher(
      arraySubscriptExpr(
          hasIndex(ignoringParenImpCasts(
              anyOf(NegLiteral, declRefExpr(to(varDecl(hasInitializer(
                                    ignoringParenImpCasts(NegLiteral)))))))))
          .bind("arrayAccess"),
      this);
}

void ArrayIndexCheck::check(const MatchFinder::MatchResult &Result) {
  const auto *ArrayAccess =
      Result.Nodes.getNodeAs<ArraySubscriptExpr>("arrayAccess");
  if (!ArrayAccess)
    return;
  diag(ArrayAccess->getIdx()->getExprLoc(), "禁止数组下标为负数");
}
} // namespace clang::tidy::gjb8114