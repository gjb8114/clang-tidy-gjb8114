#include "CaseLevelCheck.h"

using namespace clang::ast_matchers;

namespace clang::tidy::gjb8114 {
void CaseLevelCheck::registerMatchers(MatchFinder *Finder) {
  Finder->addMatcher(
      switchStmt(unless(isExpansionInSystemHeader()),
                 has(compoundStmt(has(stmt(
                     // case: { case: }
                     caseStmt(unless(hasDescendant(switchStmt())),
                              hasDescendant(compoundStmt(hasDescendant(
                                  caseStmt().bind("nestedCase")))))))))),
      this);
  Finder->addMatcher(
      switchStmt(unless(isExpansionInSystemHeader()),
                 has(compoundStmt(has(stmt(
                     // case: {...} if { case: }
                     ifStmt(hasDescendant(caseStmt().bind("nestedCase")))))))),
      this);
}

void CaseLevelCheck::check(const MatchFinder::MatchResult &Result) {
  if (const auto *NestedCase = Result.Nodes.getNodeAs<CaseStmt>("nestedCase"))
    diag(NestedCase->getBeginLoc(), "禁止 switch 语句的分支层次不一致");
}
} // namespace clang::tidy::gjb8114