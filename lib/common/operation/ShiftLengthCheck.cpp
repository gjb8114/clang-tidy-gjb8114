#include "ShiftLengthCheck.h"
using namespace clang::ast_matchers;

namespace clang::tidy::gjb8114 {
void ShiftLengthCheck::registerMatchers(MatchFinder *Finder) {
  Finder->addMatcher(
      binaryOperator(anyOf(hasOperatorName("<<"), hasOperatorName(">>")),
                     hasRHS(integerLiteral().bind("shift_amount")))
          .bind("shift"),
      this);
}

void ShiftLengthCheck::check(const MatchFinder::MatchResult &Result) {
  const auto *ShiftOp = Result.Nodes.getNodeAs<BinaryOperator>("shift");
  const auto *ShiftAmount =
      Result.Nodes.getNodeAs<IntegerLiteral>("shift_amount");

  if (!ShiftOp || !ShiftAmount)
    return;

  QualType LHSType = ShiftOp->getLHS()->IgnoreImpCasts()->getType();
  if (const auto *BT =
          dyn_cast<BuiltinType>(LHSType->getUnqualifiedDesugaredType())) {
    auto TypeWidth = Result.Context->getTypeSize(BT);
    auto Value = ShiftAmount->getValue();
    if (Value.getZExtValue() >= TypeWidth)
      diag(ShiftOp->getOperatorLoc(), "禁止移位运算超出变量长度");
  }
}
} // namespace clang::tidy::gjb8114