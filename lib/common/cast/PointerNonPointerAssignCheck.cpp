#include "PointerNonPointerAssignCheck.h"
#include "matchers/matcher.h"

using namespace clang::ast_matchers;

namespace clang::tidy::gjb8114 {

void PointerNonPointerAssignCheck::registerMatchers(MatchFinder *Finder) {
  Finder->addMatcher(
      binaryOperator(
          hasOperatorName("="),
          hasLHS(expr(hasType(pointerType())).bind("lhs")),
          hasRHS(implicitCastExpr(hasSourceExpression(
              expr(unless(hasType(pointerType())), unless(gjb8114::isNull()))
                  .bind("rhs")))))
          .bind("nonPtrToPtr"),
      this);
  Finder->addMatcher(
      binaryOperator(hasOperatorName("="),
                     hasLHS(expr(unless(hasType(pointerType()))).bind("lhs")),
                     hasRHS(implicitCastExpr(hasSourceExpression(
                         expr(hasType(pointerType())).bind("rhs")))))
          .bind("ptrToNonPtr"),
      this);
}

void PointerNonPointerAssignCheck::check(
    const MatchFinder::MatchResult &Result) {
  auto *NonPtrToPtr = Result.Nodes.getNodeAs<BinaryOperator>("nonPtrToPtr");
  auto *PtrToNonPtr = Result.Nodes.getNodeAs<BinaryOperator>("ptrToNonPtr");
  if (!NonPtrToPtr && !PtrToNonPtr)
    return;

  for (const auto *Assign : {NonPtrToPtr, PtrToNonPtr}) {
    if (!Assign)
      continue;
    auto *RHSExpr = Result.Nodes.getNodeAs<Expr>("rhs");
    auto *D = "禁止指针与非指针变量之间赋值";
    diag(RHSExpr->getBeginLoc(), D) << Assign->getSourceRange();
  }
}

} // namespace clang::tidy::gjb8114