#include "ReturnTypeCheck.h"

using namespace clang::ast_matchers;

namespace clang::tidy::gjb8114 {
void ReturnTypeCheck::registerMatchers(MatchFinder *Finder) {
  Finder->addMatcher(returnStmt(unless(isExpansionInSystemHeader()),
                                has(implicitCastExpr().bind("cast"))),
                     this);
}

void ReturnTypeCheck::check(const MatchFinder::MatchResult &Result) {
  if (const auto *Cast = Result.Nodes.getNodeAs<ImplicitCastExpr>("cast"))
    diag(Cast->getBeginLoc(), "禁止函数返回值类型与定义不一致");
}
} // namespace clang::tidy::gjb8114