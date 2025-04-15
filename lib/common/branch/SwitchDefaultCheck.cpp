#include "SwitchDefaultCheck.h"

using namespace clang::ast_matchers;

namespace clang::tidy::gjb8114 {
void SwitchDefaultCheck::registerMatchers(MatchFinder *Finder) {
  Finder->addMatcher(switchStmt(unless(isExpansionInSystemHeader()),
                                hasCondition(expr().bind("condition")),
                                unless(hasDescendant(defaultStmt())))
                         .bind("switch"),
                     this);
}

void SwitchDefaultCheck::check(const MatchFinder::MatchResult &Result) {
  const auto *Switch = Result.Nodes.getNodeAs<SwitchStmt>("switch");
  const auto *Condition = Result.Nodes.getNodeAs<Expr>("condition");

  auto Type = Condition->IgnoreParenImpCasts()->getType().getCanonicalType();
  if (const auto *ET = dyn_cast<EnumType>(Type)) {
    const EnumDecl *ED = ET->getDecl();
    unsigned numEnumValues = 0;
    for (auto It = ED->enumerator_begin(); It != ED->enumerator_end(); ++It) {
      ++numEnumValues;
    }

    unsigned numCases = 0;
    const SwitchCase *SC = Switch->getSwitchCaseList();
    while (SC) {
      if (!isa<DefaultStmt>(SC)) {
        ++numCases;
      }
      SC = SC->getNextSwitchCase();
    }

    if (numCases == numEnumValues)
      return;
  }

  diag(Switch->getBeginLoc(), "禁止 switch 省略 default");
}
} // namespace clang::tidy::gjb8114