#include "LogicalBitwiseCheck.h"

using namespace clang::ast_matchers;

namespace clang::tidy::gjb8114 {
void LogicalBitwiseCheck::registerMatchers(MatchFinder *Finder) {
  auto Bitwise = binaryOperator(
      anyOf(hasOperatorName("&"), hasOperatorName("|")),
      hasEitherOperand(
          anyOf(binaryOperator(isComparisonOperator()),
                hasDescendant(binaryOperator(isComparisonOperator())))));
  Finder->addMatcher(Bitwise.bind("bitwise"), this);
}

void LogicalBitwiseCheck::check(const MatchFinder::MatchResult &Result) {
  if (const auto *Op = Result.Nodes.getNodeAs<BinaryOperator>("bitwise"))
    diag(Op->getExprLoc(), "禁止对逻辑表达式进行位运算");
}
} // namespace clang::tidy::gjb8114