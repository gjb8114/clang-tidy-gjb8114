#include "PointerAssignTypeCheck.h"
#include "matchers/matcher.h"

using namespace clang::ast_matchers;

namespace clang::tidy::gjb8114 {
void PointerAssignTypeCheck::registerMatchers(MatchFinder *Finder) {
  Finder->addMatcher(
      binaryOperator(
          hasOperatorName("="),
          hasLHS(declRefExpr(to(varDecl(hasType(pointerType())).bind("lhs")))),
          hasRHS(implicitCastExpr(hasSourceExpression(
              expr(hasType(pointerType()), unless(gjb8114::isNull()))
                  .bind("rhs")))))
          .bind("assignment"),
      this);
}

void PointerAssignTypeCheck::check(const MatchFinder::MatchResult &Result) {
  const auto *BinOp = Result.Nodes.getNodeAs<BinaryOperator>("assignment");
  const auto *LHS = Result.Nodes.getNodeAs<VarDecl>("lhs");
  const auto *RHSExpr = Result.Nodes.getNodeAs<Expr>("rhs");

  if (!BinOp || !LHS || !RHSExpr)
    return;

  auto LHSPointeeType = LHS->getType()->getPointeeType();
  auto RHSPointeeType = RHSExpr->getType()->getPointeeType();
  if (LHSPointeeType != RHSPointeeType)
    diag(RHSExpr->getBeginLoc(), "禁止指针变量的赋值类型与指针变量类型不一致")
        << BinOp->getSourceRange();
}
} // namespace clang::tidy::gjb8114