#include "ParameterPointerAssignCheck.h"

using namespace clang::ast_matchers;

namespace clang::tidy::gjb8114 {
void ParameterPointerAssignCheck::registerMatchers(MatchFinder *Finder) {
  Finder->addMatcher(
      binaryOperator(
          unless(isExpansionInSystemHeader()), hasOperatorName("="),
          hasLHS(declRefExpr(
              to(parmVarDecl(hasType(pointerType())).bind("pointerParam")))))
          .bind("assignmentOp"),
      this);
}

void ParameterPointerAssignCheck::check(
    const MatchFinder::MatchResult &Result) {
  const auto *PVD = Result.Nodes.getNodeAs<ParmVarDecl>("pointerParam");
  const auto *BO = Result.Nodes.getNodeAs<BinaryOperator>("assignmentOp");
  if (!PVD || !BO)
    return;
  diag(BO->getOperatorLoc(), "禁止对指针型参数重新赋值")
      << BO->getSourceRange();
}
} // namespace clang::tidy::gjb8114