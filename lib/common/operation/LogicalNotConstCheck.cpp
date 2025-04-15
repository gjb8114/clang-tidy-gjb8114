#include "LogicalNotConstCheck.h"
using namespace clang::ast_matchers;

namespace clang::tidy::gjb8114 {
void LogicalNotConstCheck::registerMatchers(MatchFinder *Finder) {
  Finder->addMatcher(
      unaryOperator(
          hasOperatorName("!"),
          hasUnaryOperand(ignoringParenImpCasts(
              expr(anyOf(integerLiteral(), floatLiteral(), characterLiteral()))
                  .bind("const"))))
          .bind("op"),
      this);
}

void LogicalNotConstCheck::check(const MatchFinder::MatchResult &Result) {
  const auto *Op = Result.Nodes.getNodeAs<UnaryOperator>("op");
  const auto *Const = Result.Nodes.getNodeAs<Expr>("const");
  if (!Op || !Const)
    return;
  diag(Op->getOperatorLoc(), "禁止对字面量值取逻辑非");
}
} // namespace clang::tidy::gjb8114