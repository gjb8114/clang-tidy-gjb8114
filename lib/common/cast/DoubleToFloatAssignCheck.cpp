#include "DoubleToFloatAssignCheck.h"

using namespace clang::ast_matchers;

namespace clang::tidy::gjb8114 {
void DoubleToFloatAssignCheck::registerMatchers(MatchFinder *Finder) {
  Finder->addMatcher(
      binaryOperator(
          hasOperatorName("="), hasLHS(declRefExpr(to(varDecl().bind("lhs")))),
          hasRHS(ignoringParenImpCasts(declRefExpr(to(varDecl().bind("rhs"))))))
          .bind("assignment"),
      this);
}

void DoubleToFloatAssignCheck::check(const MatchFinder::MatchResult &Result) {
  const auto *BinOp = Result.Nodes.getNodeAs<BinaryOperator>("assignment");
  if (!BinOp)
    return;

  const auto *LHS = Result.Nodes.getNodeAs<VarDecl>("lhs");
  const auto *RHS = Result.Nodes.getNodeAs<VarDecl>("rhs");
  auto LHSType = LHS->getType();
  auto RHSType = RHS->getType();
  if (LHSType->isSpecificBuiltinType(BuiltinType::Float) &&
      RHSType->isSpecificBuiltinType(BuiltinType::Double)) {
    const Expr *RHSExpr = BinOp->getRHS()->IgnoreImplicit();
    if (!isa<ExplicitCastExpr>(RHSExpr) && !isa<CStyleCastExpr>(RHSExpr) &&
        !isa<CXXFunctionalCastExpr>(RHSExpr) &&
        !isa<CXXNamedCastExpr>(RHSExpr)) {
      diag(RHSExpr->getBeginLoc(), "禁止 double 型变量赋给 float 型变量")
          << BinOp->getSourceRange();
    }
  }
}
} // namespace clang::tidy::gjb8114