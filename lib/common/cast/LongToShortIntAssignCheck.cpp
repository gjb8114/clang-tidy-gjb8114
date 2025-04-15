#include "LongToShortIntAssignCheck.h"

using namespace clang::ast_matchers;

namespace clang::tidy::gjb8114 {
void LongToShortIntAssignCheck::registerMatchers(MatchFinder *Finder) {
  Finder->addMatcher(
      binaryOperator(
          hasOperatorName("="),
          hasLHS(declRefExpr(to(varDecl(hasType(isInteger())).bind("lhs")))),
          hasRHS(ignoringParenImpCasts(
              declRefExpr(to(varDecl(hasType(isInteger())).bind("rhs"))))))
          .bind("assignment"),
      this);
}

void LongToShortIntAssignCheck::check(const MatchFinder::MatchResult &Result) {
  const auto *BinOp = Result.Nodes.getNodeAs<BinaryOperator>("assignment");
  if (!BinOp)
    return;

  auto LHSType = Result.Nodes.getNodeAs<VarDecl>("lhs")->getType();
  auto RHSType = Result.Nodes.getNodeAs<VarDecl>("rhs")->getType();
  auto LSize = Result.Context->getTypeSize(LHSType);
  auto RSize = Result.Context->getTypeSize(RHSType);
  if (RSize > LSize) {
    const Expr *RHSExpr = BinOp->getRHS()->IgnoreImplicit();
    if (!isa<ExplicitCastExpr>(RHSExpr) && !isa<CStyleCastExpr>(RHSExpr) &&
        !isa<CXXFunctionalCastExpr>(RHSExpr) &&
        !isa<CXXNamedCastExpr>(RHSExpr)) {
      diag(RHSExpr->getBeginLoc(), "禁止长整数变量赋给短整数变量")
          << BinOp->getSourceRange();
    }
  }
}
} // namespace clang::tidy::gjb8114
