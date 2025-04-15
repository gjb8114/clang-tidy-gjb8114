#include "BoolComparisonCheck.h"
#include "clang/AST/Type.h"

using namespace clang::ast_matchers;

namespace clang::tidy::gjb8114 {
void BoolComparisonCheck::registerMatchers(MatchFinder *Finder) {
  Finder->addMatcher(
      binaryOperator(anyOf(hasOperatorName("<"), hasOperatorName(">")),
                     hasLHS(expr().bind("lhs")), hasRHS(expr().bind("rhs")))
          .bind("binaryOp"),
      this);
}

void BoolComparisonCheck::check(const MatchFinder::MatchResult &Result) {
  const auto *BinOp = Result.Nodes.getNodeAs<BinaryOperator>("binaryOp");
  const auto *LHS = Result.Nodes.getNodeAs<Expr>("lhs");
  const auto *RHS = Result.Nodes.getNodeAs<Expr>("rhs");

  if (!BinOp || !LHS || !RHS)
    return;

  auto IsBoolean = [&](const Expr *E) -> bool {
    QualType Type = E->getType();

    if (Type->isBooleanType())
      return true;

    // Special handling for builtin type variables that are used as booleans
    // (like in the test case where bool is typedef from unsigned int)
    if (Type->isTypedefNameType()) {
      const TypedefType *TT = Type->getAs<TypedefType>();
      if (TT->getDecl()->getName() == "bool")
        return true;
    }
    return false;
  };

  if (IsBoolean(LHS->IgnoreImpCasts()) && IsBoolean(RHS->IgnoreImpCasts())) {
    diag(BinOp->getExprLoc(), "禁止对 boolean 值进行无意义比较")
        << BinOp->getSourceRange();
  }
}
} // namespace clang::tidy::gjb8114