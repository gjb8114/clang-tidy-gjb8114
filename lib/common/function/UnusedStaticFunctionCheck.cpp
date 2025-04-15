#include "UnusedStaticFunctionCheck.h"

using namespace clang::ast_matchers;

namespace clang::tidy::gjb8114 {
void UnusedStaticFunctionCheck::registerMatchers(MatchFinder *Finder) {
  Finder->addMatcher(functionDecl(isStaticStorageClass(), isDefinition(),
                                  unless(isExpansionInSystemHeader()))
                         .bind("staticFunc"),
                     this);
}

void UnusedStaticFunctionCheck::check(const MatchFinder::MatchResult &Result) {
  const auto *FuncDecl = Result.Nodes.getNodeAs<FunctionDecl>("staticFunc");
  if (FuncDecl != nullptr && !FuncDecl->isReferenced())
    diag(FuncDecl->getLocation(), "禁止未使用的静态函数") << FuncDecl;
}
} // namespace clang::tidy::gjb8114