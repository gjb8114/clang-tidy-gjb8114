#include "AmbiguousLogicCheck.h"

using namespace clang::ast_matchers;

namespace clang::tidy::gjb8114 {

static const char *Description = "禁止二义性逻辑表达式";

void AmbiguousLogicCheck::registerMatchers(MatchFinder *Finder) {
  Finder->addMatcher(
      binaryOperator(unless(isExpansionInSystemHeader()),
                     anyOf(
                         // a || b && c pattern
                         allOf(hasOperatorName("||"),
                               hasRHS(binaryOperator(hasOperatorName("&&")))),
                         // a && b || c pattern
                         allOf(hasOperatorName("||"),
                               hasLHS(binaryOperator(hasOperatorName("&&"))))))
          .bind("bin_op"),
      this);

  Finder->addMatcher(
      binaryOperator(unless(isExpansionInSystemHeader()),
                     anyOf(
                         // !a && b pattern
                         allOf(hasOperatorName("&&"),
                               hasLHS(unaryOperator(hasOperatorName("!")))),
                         // !a || b pattern
                         allOf(hasOperatorName("||"),
                               hasLHS(unaryOperator(hasOperatorName("!"))))))
          .bind("not_op"),
      this);
}

void AmbiguousLogicCheck::check(const MatchFinder::MatchResult &Result) {
  if (const auto *BinOp = Result.Nodes.getNodeAs<BinaryOperator>("bin_op"))
    diag(BinOp->getBeginLoc(), Description);

  if (const auto *NotOp = Result.Nodes.getNodeAs<BinaryOperator>("not_op"))
    diag(NotOp->getBeginLoc(), Description);
}
} // namespace clang::tidy::gjb8114