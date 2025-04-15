#pragma once

#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/Tooling/Tooling.h"

namespace clang::tidy::gjb8114 {
static auto runMatcherOnCode(StringRef Code,
                             const ast_matchers::StatementMatcher &Matcher) {
  std::map<std::string, clang::DynTypedNode> Matches;
  class MatchCollector : public ast_matchers::MatchFinder::MatchCallback {
  public:
    std::map<std::string, clang::DynTypedNode> &Matches;
    MatchCollector(std::map<std::string, clang::DynTypedNode> &Matches)
        : Matches(Matches) {}
    void run(const ast_matchers::MatchFinder::MatchResult &Result) override {
      Matches.insert(Result.Nodes.getMap().begin(),
                     Result.Nodes.getMap().end());
    }
  } Collector(Matches);

  ast_matchers::MatchFinder Finder;
  Finder.addMatcher(Matcher, &Collector);

  using namespace clang::tooling;
  auto Factory = newFrontendActionFactory(&Finder);
  // runToolOnCode() runs a complete Clang tool; here we ignore diagnostics.
  EXPECT_TRUE(runToolOnCode(Factory->create(), Code));
  return Matches;
}
} // namespace clang::tidy::gjb8114
