#include "SizeofAssignmentCheck.h"
using namespace clang::ast_matchers;

namespace clang::tidy::gjb8114 {
void SizeofAssignmentCheck::registerMatchers(MatchFinder *Finder) {
  Finder->addMatcher(
      sizeOfExpr(
          hasDescendant(binaryOperator(isAssignmentOperator()).bind("assign"))),
      this);
}

void SizeofAssignmentCheck::check(const MatchFinder::MatchResult &Result) {
  if (const auto *AssignOp = Result.Nodes.getNodeAs<BinaryOperator>("assign")) {
    diag(AssignOp->getOperatorLoc(), "禁止 sizeof 中使用赋值");
  }
}
} // namespace clang::tidy::gjb8114