#include "FloatToIntVarAssignCheck.h"

using namespace clang::ast_matchers;

namespace clang::tidy::gjb8114 {
void FloatToIntVarAssignCheck::registerMatchers(MatchFinder *Finder) {
  Finder->addMatcher(
      binaryOperator(hasOperatorName("="),
                     hasLHS(declRefExpr(to(varDecl(hasType(isInteger()))))),
                     hasRHS(ignoringParenImpCasts(declRefExpr(
                         to(varDecl(hasType(realFloatingPointType())))))))
          .bind("floatVarAssignment"),
      this);
}

void FloatToIntVarAssignCheck::check(const MatchFinder::MatchResult &Result) {
  if (const auto *Assignment =
          Result.Nodes.getNodeAs<BinaryOperator>("floatVarAssignment")) {
    const Expr *RHS = Assignment->getRHS()->IgnoreParens();
    if (bool HasExplicitCast =
            isa<ExplicitCastExpr>(RHS) || isa<CStyleCastExpr>(RHS))
      return;
    diag(RHS->getExprLoc(), "禁止浮点数变量赋给整型变量");
  }
}
} // namespace clang::tidy::gjb8114