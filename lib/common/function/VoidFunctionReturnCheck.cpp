#include "VoidFunctionReturnCheck.h"

using namespace clang::ast_matchers;

namespace clang::tidy::gjb8114 {
void VoidFunctionReturnCheck::registerMatchers(MatchFinder *Finder) {
  Finder->addMatcher(
      returnStmt(unless(isExpansionInSystemHeader()), hasReturnValue(expr()),
                 hasAncestor(functionDecl(returns(voidType())).bind("func")))
          .bind("return"),
      this);
}

void VoidFunctionReturnCheck::check(const MatchFinder::MatchResult &Result) {
  if (const auto *RetStmt = Result.Nodes.getNodeAs<ReturnStmt>("return"))
    diag(RetStmt->getBeginLoc(), "禁止 void 函数 return 语句带值");
}
} // namespace clang::tidy::gjb8114