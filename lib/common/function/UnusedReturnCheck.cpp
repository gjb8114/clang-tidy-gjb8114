#include "UnusedReturnCheck.h"

using namespace clang::ast_matchers;

namespace clang::tidy::gjb8114 {
void UnusedReturnCheck::registerMatchers(MatchFinder *Finder) {
  Finder->addMatcher(
      callExpr(
          unless(isExpansionInSystemHeader()),
          callee(functionDecl(unless(returns(voidType())))),
          // Not used as a subexpression
          unless(hasParent(expr())),
          // Not cast to void
          unless(hasParent(cStyleCastExpr(hasDestinationType(voidType())))),
          unless(hasParent(cxxStaticCastExpr(hasDestinationType(voidType()))))
      ).bind("call"),
      this);
}

void UnusedReturnCheck::check(const MatchFinder::MatchResult &Result) {
  if (const auto *Call = Result.Nodes.getNodeAs<CallExpr>("call")) {
    diag(Call->getBeginLoc(), "禁止忽略函数调用返回值而未加 (void)")
        << Call->getSourceRange();
  }
}
} // namespace clang::tidy::gjb8114