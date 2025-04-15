#include "IfElseIfBranchCheck.h"

using namespace clang::ast_matchers;

namespace clang::tidy::gjb8114 {
void IfElseIfBranchCheck::registerMatchers(MatchFinder *Finder) {
  Finder->addMatcher(ifStmt(has(ifStmt(unless(hasElse(stmt())))),
                            unless(isExpansionInSystemHeader()))
                         .bind("ifElseIf"),
                     this);
}

void IfElseIfBranchCheck::check(const MatchFinder::MatchResult &Result) {
  const auto *IfStmt = Result.Nodes.getNodeAs<clang::IfStmt>("ifElseIf");
  if (!IfStmt)
    return;
  diag(IfStmt->getEndLoc(), "禁止省略 if-else if 语句的 else 分支");
}
} // namespace clang::tidy::gjb8114