#include "FloatComparisonCheck.h"
#include "clang/AST/Type.h"

using namespace clang::ast_matchers;

namespace clang::tidy::gjb8114 {
void FloatComparisonCheck::registerMatchers(MatchFinder *Finder) {
  Finder->addMatcher(
      binaryOperator(anyOf(hasOperatorName("=="), hasOperatorName("!=")),
                     hasLHS(expr().bind("lhs")), hasRHS(expr().bind("rhs")))
          .bind("binaryOp"),
      this);
}

void FloatComparisonCheck::check(const MatchFinder::MatchResult &Result) {
  const auto *BinOp = Result.Nodes.getNodeAs<BinaryOperator>("binaryOp");
  const auto *LHS = Result.Nodes.getNodeAs<Expr>("lhs");
  const auto *RHS = Result.Nodes.getNodeAs<Expr>("rhs");

  if (!BinOp || !LHS || !RHS)
    return;

  auto Loc = BinOp->getBeginLoc();
  if (!Loc.isValid() || Result.SourceManager->isInSystemHeader(Loc)) {
    return;
  }

  auto IsFloating = [&](const Expr *E) -> bool {
    QualType Type = E->getType();
    return Type->isRealFloatingType();
  };

  if (IsFloating(LHS) && IsFloating(RHS)) {
    diag(BinOp->getExprLoc(), "禁止浮点数相等比较") << BinOp->getSourceRange();
  }
}
} // namespace clang::tidy::gjb8114