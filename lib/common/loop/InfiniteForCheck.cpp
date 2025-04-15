#include "InfiniteForCheck.h"
using namespace clang::ast_matchers;

namespace clang::tidy::gjb8114 {
void InfiniteForCheck::registerMatchers(MatchFinder *Finder) {
  Finder->addMatcher(forStmt(unless(hasCondition(expr())),
                             unless(hasIncrement(expr())),
                             unless(hasLoopInit(anything())))
                         .bind("emptyForLoop"),
                     this);
}

void InfiniteForCheck::check(const MatchFinder::MatchResult &Result) {
  if (const auto *ForLoop = Result.Nodes.getNodeAs<ForStmt>("emptyForLoop"))
    diag(ForLoop->getBeginLoc(), "禁止 while(1) 以外的无限循环");
}
} // namespace clang::tidy::gjb8114