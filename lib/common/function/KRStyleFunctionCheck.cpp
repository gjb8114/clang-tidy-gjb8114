#include "KRStyleFunctionCheck.h"

using namespace clang::ast_matchers;

namespace clang::tidy::gjb8114 {
void KRStyleFunctionCheck::registerMatchers(MatchFinder *Finder) {
  Finder->addMatcher(functionDecl(unless(isImplicit()),
                                  unless(isExpansionInSystemHeader()),
                                  hasBody(anything()))
                         .bind("func"),
                     this);
}

void KRStyleFunctionCheck::check(const MatchFinder::MatchResult &Result) {
  const auto *FD = Result.Nodes.getNodeAs<FunctionDecl>("func");
  if (!FD)
    return;

  bool IsKRStyle = !FD->hasWrittenPrototype() && FD->getNumParams() > 0;
  if (IsKRStyle)
    diag(FD->getLocation(), "禁止 K&R 风格函数定义");
}
} // namespace clang::tidy::gjb8114