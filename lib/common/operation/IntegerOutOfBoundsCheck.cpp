#include "IntegerOutOfBoundsCheck.h"

using namespace clang::ast_matchers;

namespace clang::tidy::gjb8114 {
void IntegerOutOfBoundsCheck::registerMatchers(MatchFinder *Finder) {
  auto IntLiteral = ignoringParenImpCasts(
      anyOf(integerLiteral().bind("literal"),
            unaryOperator(hasOperatorName("-"),
                          hasUnaryOperand(integerLiteral().bind("literal")))
                .bind("neg")));
  Finder->addMatcher(
      varDecl(hasType(isInteger()), hasInitializer(IntLiteral)).bind("var"),
      this);
  Finder->addMatcher(
      binaryOperator(isAssignmentOperator(),
                     hasLHS(ignoringParenImpCasts(declRefExpr(
                         to(varDecl(hasType(isInteger())).bind("var"))))),
                     hasRHS(IntLiteral))
          .bind("assign"),
      this);
}

void IntegerOutOfBoundsCheck::check(const MatchFinder::MatchResult &Result) {
  const auto *Literal = Result.Nodes.getNodeAs<IntegerLiteral>("literal");
  const auto *Neg = Result.Nodes.getNodeAs<UnaryOperator>("neg");
  const auto *Var = Result.Nodes.getNodeAs<ValueDecl>("var");
  const auto *Assign = Result.Nodes.getNodeAs<BinaryOperator>("assign");
  if (!Var && !Assign)
    return;

  QualType TargetType = Var->getType();
  unsigned TypeWidth = Result.Context->getIntWidth(TargetType);
  bool IsSigned = TargetType->isSignedIntegerOrEnumerationType();
  auto Value = Literal->getValue();
  if (Neg)
    Value.negate();

  bool Fits;
  if (IsSigned) {
    auto Width = Value.getBitWidth();
    auto MinVal = llvm::APInt::getSignedMinValue(TypeWidth).sext(Width);
    auto MaxVal = llvm::APInt::getSignedMaxValue(TypeWidth).sext(Width);
    Fits =
        Value.isSignedIntN(TypeWidth) && Value.sge(MinVal) && Value.sle(MaxVal);
  } else {
    Fits = Value.isIntN(TypeWidth);
  }
  if (!Fits)
    diag(Literal->getBeginLoc(), "禁止整数赋值越界")
        << Literal->getSourceRange();
}
} // namespace clang::tidy::gjb8114