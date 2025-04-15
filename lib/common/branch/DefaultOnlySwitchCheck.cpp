#include "DefaultOnlySwitchCheck.h"

using namespace clang::ast_matchers;

namespace clang::tidy::gjb8114 {
void DefaultOnlySwitchCheck::registerMatchers(MatchFinder *Finder) {
  Finder->addMatcher(
      switchStmt(hasDescendant(defaultStmt().bind("default")), 
                unless(hasDescendant(caseStmt()))).bind("switch"),
      this);
}

void DefaultOnlySwitchCheck::check(const MatchFinder::MatchResult &Result) {
  const auto *DefaultCase = Result.Nodes.getNodeAs<DefaultStmt>("default");
  diag(DefaultCase->getKeywordLoc(), "禁止 switch 语句仅包含 default");
}
} // namespace clang::tidy::gjb8114