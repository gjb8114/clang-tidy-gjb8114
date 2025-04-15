#include "UnsignedNegAssignCheck.h"

using namespace clang::ast_matchers;

namespace clang::tidy::gjb8114 {
void UnsignedNegAssignCheck::registerMatchers(
    clang::ast_matchers::MatchFinder *Finder) {
  Finder->addMatcher(
      binaryOperator(unless(isExpansionInSystemHeader()),
                     isAssignmentOperator(),
                     hasLHS(declRefExpr(to(
                         varDecl(hasType(isUnsignedInteger())).bind("lhs")))),
                     hasRHS(ignoringParenImpCasts(
                         unaryOperator(hasOperatorName("-"),
                                       hasUnaryOperand(integerLiteral()))
                             .bind("rhs"))))
          .bind("assignment"),
      this);
}

void UnsignedNegAssignCheck::check(const MatchFinder::MatchResult &Result) {
  const auto *BinOp = Result.Nodes.getNodeAs<BinaryOperator>("assignment");
  const auto *LHS = Result.Nodes.getNodeAs<VarDecl>("lhs");
  const auto *RHSExpr = Result.Nodes.getNodeAs<Expr>("rhs");

  if (!BinOp || !LHS || !RHSExpr)
    return;

  diag(RHSExpr->getBeginLoc(), "禁止无符号变量赋负值")
      << BinOp->getSourceRange();
}
} // namespace clang::tidy::gjb8114
