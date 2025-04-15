#include "ZeroPointerCheck.h"
#include "matchers/matcher.h"

using namespace clang::ast_matchers;

namespace clang::tidy::gjb8114 {
void ZeroPointerCheck::registerMatchers(MatchFinder *Finder) {
  Finder->addMatcher(
      castExpr(hasType(isAnyPointer()), has(integerLiteral(equals(0))))
          .bind("cast"),
      this);
}

void ZeroPointerCheck::check(const MatchFinder::MatchResult &Result) {
  const char *Description = "禁止使用整型数 0 表示空指针";
  if (const auto *Cast = Result.Nodes.getNodeAs<CastExpr>("cast")) {
    diag(Cast->getSubExpr()->getExprLoc(), Description);
  }
}
} // namespace clang::tidy::gjb8114