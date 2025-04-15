#include "EmptySwitchCheck.h"
#include "matchers/matcher.h"

using namespace clang::ast_matchers;

namespace clang::tidy::gjb8114 {
void EmptySwitchCheck::registerMatchers(MatchFinder *Finder) {
  Finder->addMatcher(
      switchStmt(unless(has(switchCase())), unless(isExpansionInSystemHeader()))
          .bind("emptySwitch"),
      this);
}

void EmptySwitchCheck::check(const MatchFinder::MatchResult &Result) {
  if (const auto *Switch = Result.Nodes.getNodeAs<SwitchStmt>("emptySwitch")) {
    diag(Switch->getEndLoc(), "禁止空 switch 语句");
  }
}
} // namespace clang::tidy::gjb8114