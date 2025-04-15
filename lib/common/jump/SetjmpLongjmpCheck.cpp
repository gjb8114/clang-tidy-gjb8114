#include "SetjmpLongjmpCheck.h"

using namespace clang::ast_matchers;

namespace clang::tidy::gjb8114 {
void SetjmpLongjmpCheck::registerMatchers(MatchFinder *Finder) {
  Finder->addMatcher(
      callExpr(callee(functionDecl(hasAnyName("setjmp", "longjmp"))))
          .bind("jmp"),
      this);
}

void SetjmpLongjmpCheck::check(const MatchFinder::MatchResult &Result) {
  const auto *Call = Result.Nodes.getNodeAs<CallExpr>("jmp");
  diag(Call->getBeginLoc(), "禁止 setjmp/longjmp");
}
} // namespace clang::tidy::gjb8114