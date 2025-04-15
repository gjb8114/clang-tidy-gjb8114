#include "PointerComparisonCheck.h"
#include "clang/AST/Type.h"

using namespace clang::ast_matchers;

namespace clang::tidy::gjb8114 {
void PointerComparisonCheck::registerMatchers(MatchFinder *Finder) {
  Finder->addMatcher(
      binaryOperator(anyOf(hasOperatorName("<"), hasOperatorName(">"),
                           hasOperatorName("<="), hasOperatorName(">=")),
                     hasLHS(expr().bind("lhs")), hasRHS(expr().bind("rhs")))
          .bind("binaryOp"),
      this);
}

void PointerComparisonCheck::check(const MatchFinder::MatchResult &Result) {
  const auto *BinOp = Result.Nodes.getNodeAs<BinaryOperator>("binaryOp");
  const auto *LHS = Result.Nodes.getNodeAs<Expr>("lhs");
  const auto *RHS = Result.Nodes.getNodeAs<Expr>("rhs");

  if (!BinOp || !LHS || !RHS)
    return;

  auto IsPointer = [&](const Expr *E) -> bool {
    QualType Type = E->getType();
    return Type->isPointerType();
  };

  if (IsPointer(LHS) && IsPointer(RHS)) {
    diag(BinOp->getExprLoc(), "禁止对指针进行无意义比较")
        << BinOp->getSourceRange();
  }
}
} // namespace clang::tidy::gjb8114