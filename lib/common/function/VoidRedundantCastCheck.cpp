#include "VoidRedundantCastCheck.h"

using namespace clang::ast_matchers;

namespace clang::tidy::gjb8114 {
void VoidRedundantCastCheck::registerMatchers(MatchFinder *Finder) {
  Finder->addMatcher(
      cStyleCastExpr(
          unless(isExpansionInSystemHeader()), hasType(voidType()),
          has(callExpr(callee(functionDecl(returns(voidType())))).bind("call")))
          .bind("cast"),
      this);
}

void VoidRedundantCastCheck::check(const MatchFinder::MatchResult &Result) {
  if (const auto *Cast = Result.Nodes.getNodeAs<CStyleCastExpr>("cast"))
    diag(Cast->getBeginLoc(), "禁止使用 (void) 转换 void 函数调用的返回值");
}
} // namespace clang::tidy::gjb8114