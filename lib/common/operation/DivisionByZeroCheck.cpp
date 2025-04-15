#include "DivisionByZeroCheck.h"
using namespace clang::ast_matchers;

namespace clang::tidy::gjb8114 {
void DivisionByZeroCheck::registerMatchers(MatchFinder *Finder) {
  Finder->addMatcher(
      binaryOperator(
          anyOf(hasOperatorName("/"), hasOperatorName("%")),
          hasRHS(ignoringParenImpCasts(anyOf(
              integerLiteral(equals(0)), declRefExpr(to(varDecl(hasInitializer(
                                             integerLiteral(equals(0))))))))))
          .bind("division"),
      this);
}

void DivisionByZeroCheck::check(const MatchFinder::MatchResult &Result) {
  if (const auto *DivExpr =
          Result.Nodes.getNodeAs<BinaryOperator>("division")) {
    diag(DivExpr->getOperatorLoc(), "禁止零除");
  }
}
} // namespace clang::tidy::gjb8114