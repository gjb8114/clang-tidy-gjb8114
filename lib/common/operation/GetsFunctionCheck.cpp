#include "GetsFunctionCheck.h"
using namespace clang::ast_matchers;

namespace clang::tidy::gjb8114 {
void GetsFunctionCheck::registerMatchers(MatchFinder *Finder) {
  Finder->addMatcher(
      declRefExpr(to(functionDecl(hasName("gets")))).bind("gets_ptr"), this);
}

void GetsFunctionCheck::check(const MatchFinder::MatchResult &Result) {
  const char *Description = "禁止使用 gets 函数";
  if (const auto *DRE = Result.Nodes.getNodeAs<DeclRefExpr>("gets_ptr")) {
    diag(DRE->getBeginLoc(), Description)
        << FixItHint::CreateReplacement(DRE->getSourceRange(), "fgets");
  }
}
} // namespace clang::tidy::gjb8114