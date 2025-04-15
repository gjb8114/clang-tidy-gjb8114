#include "UnaryOpInExprCheck.h"

using namespace clang::ast_matchers;

namespace clang::tidy::gjb8114 {
void UnaryOpInExprCheck::registerMatchers(MatchFinder *Finder) {
  Finder->addMatcher(
      unaryOperator(
          unless(isExpansionInSystemHeader()),
          anyOf(hasOperatorName("++"), hasOperatorName("--")),
          anyOf(hasAncestor(binaryOperator(unless(hasParent(compoundStmt())))),
                hasAncestor(callExpr())))
          .bind("unaryOp"),
      this);
}

void UnaryOpInExprCheck::check(const MatchFinder::MatchResult &Result) {
  if (const auto *Op = Result.Nodes.getNodeAs<UnaryOperator>("unaryOp")) {
    diag(Op->getExprLoc(), "禁止在运算表达式或函数调用参数中使用自增自减");
  }
}
} // namespace clang::tidy::gjb8114