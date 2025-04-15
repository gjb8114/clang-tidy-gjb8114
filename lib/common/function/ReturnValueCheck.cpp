#include "ReturnValueCheck.h"
#include "matchers/matcher.h"

using namespace clang::ast_matchers;

namespace clang::tidy::gjb8114 {
void ReturnValueCheck::registerMatchers(MatchFinder *Finder) {
  Finder->addMatcher(
      functionDecl(
          unless(isImplicit()), unless(isExpansionInSystemHeader()),
          unless(hasType(voidType())),
          forEachDescendant(returnStmt(unless(has(expr()))).bind("return"))),
      this);
}

void ReturnValueCheck::check(const MatchFinder::MatchResult &Result) {
  const auto *RetStmt = Result.Nodes.getNodeAs<ReturnStmt>("return");
  if (!RetStmt)
    return;
  diag(RetStmt->getBeginLoc(), "禁止非 void 函数的 return 语句不带值");
}
} // namespace clang::tidy::gjb8114