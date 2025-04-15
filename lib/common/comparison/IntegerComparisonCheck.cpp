#include "IntegerComparisonCheck.h"

using namespace clang::ast_matchers;

namespace clang::tidy::gjb8114 {
void IntegerComparisonCheck::registerMatchers(MatchFinder *Finder) {
  Finder->addMatcher(binaryOperator(isComparisonOperator(),
                                    hasLHS(expr().bind("lhs")),
                                    hasRHS(expr().bind("rhs")))
                         .bind("binaryOp"),
                     this);
}

void IntegerComparisonCheck::check(const MatchFinder::MatchResult &Result) {
  const auto *BinOp = Result.Nodes.getNodeAs<BinaryOperator>("binaryOp");
  const auto *LHS = Result.Nodes.getNodeAs<Expr>("lhs");
  const auto *RHS = Result.Nodes.getNodeAs<Expr>("rhs");

  if (!BinOp || !LHS || !RHS)
    return;

  auto IsSigned = [&](const Expr *E) -> bool {
    QualType Type = E->getType();
    return Type->isSignedIntegerOrEnumerationType();
  };
  auto IsUnsigned = [&](const Expr *E) -> bool {
    QualType Type = E->getType();
    return Type->isUnsignedIntegerType();
  };

  LHS = LHS->IgnoreImplicit();
  RHS = RHS->IgnoreImplicit();
  if ((IsSigned(LHS) && IsUnsigned(RHS)) ||
      (IsUnsigned(LHS) && IsSigned(RHS))) {
    diag(BinOp->getExprLoc(), "禁止无符号数与有符号数比较")
        << BinOp->getSourceRange();
  }
}
} // namespace clang::tidy::gjb8114