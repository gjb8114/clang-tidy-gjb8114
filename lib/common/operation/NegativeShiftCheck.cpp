#include "NegativeShiftCheck.h"
using namespace clang::ast_matchers;

namespace clang::tidy::gjb8114 {
void NegativeShiftCheck::registerMatchers(MatchFinder *Finder) {
  Finder->addMatcher(
      binaryOperator(
          unless(isExpansionInSystemHeader()),
          anyOf(hasOperatorName("<<"), hasOperatorName(">>")),
          hasRHS(ignoringParenImpCasts(
              anyOf(unaryOperator(hasOperatorName("-"),
                                  hasUnaryOperand(integerLiteral()))
                        .bind("neg"),
                    declRefExpr(
                        to(varDecl(hasType(isSignedInteger())).bind("var")))))))
          .bind("shift"),
      this);
}

void NegativeShiftCheck::check(const MatchFinder::MatchResult &Result) {
  const auto *ShiftOp = Result.Nodes.getNodeAs<BinaryOperator>("shift");
  if (!ShiftOp)
    return;

  const char *Description = "禁止移位数为负数";
  if (const auto *Neg = Result.Nodes.getNodeAs<UnaryOperator>("neg"))
    diag(ShiftOp->getRHS()->getExprLoc(), Description);

  if (const auto *VD = Result.Nodes.getNodeAs<VarDecl>("var"))
    if (VD->getType()->isSignedIntegerType())
      diag(ShiftOp->getRHS()->getExprLoc(), Description);
}
} // namespace clang::tidy::gjb8114