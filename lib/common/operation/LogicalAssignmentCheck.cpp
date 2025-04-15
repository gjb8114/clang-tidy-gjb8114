#include "LogicalAssignmentCheck.h"

using namespace clang::ast_matchers;

namespace clang::tidy::gjb8114 {
void LogicalAssignmentCheck::registerMatchers(MatchFinder *Finder) {
  auto AssignExpr = binaryOperator(isAssignmentOperator()).bind("assign");
  Finder->addMatcher(ifStmt(hasCondition(AssignExpr)).bind("if"), this);
  Finder->addMatcher(whileStmt(hasCondition(AssignExpr)).bind("while"), this);
  Finder->addMatcher(forStmt(hasCondition(AssignExpr)).bind("for"), this);
}

void LogicalAssignmentCheck::check(const MatchFinder::MatchResult &Result) {
  const auto *Assign = Result.Nodes.getNodeAs<BinaryOperator>("assign");
  if (!Assign)
    return;
  const auto *If = Result.Nodes.getNodeAs<IfStmt>("if");
  const auto *While = Result.Nodes.getNodeAs<WhileStmt>("while");
  const auto *For = Result.Nodes.getNodeAs<ForStmt>("for");
  if (If || While || For)
    diag(Assign->getExprLoc(), "禁止将赋值语句作为逻辑表达式");
}
} // namespace clang::tidy::gjb8114