#include "FunctionParamTypeCheck.h"
#include "clang/AST/ASTContext.h"

using namespace clang::ast_matchers;

namespace clang::tidy::gjb8114 {
void FunctionParamTypeCheck::registerMatchers(MatchFinder *Finder) {
  Finder->addMatcher(functionDecl(isDefinition()).bind("func"), this);
}

void FunctionParamTypeCheck::check(const MatchFinder::MatchResult &Result) {
  const auto *FD = Result.Nodes.getNodeAs<FunctionDecl>("func");
  if (!FD)
    return;

  if (Result.SourceManager->isInSystemHeader(FD->getLocation()))
    return;

  if (!FD->hasPrototype() && FD->getNumParams() > 0) {
    auto Loc = FD->getParamDecl(0)->getLocation();
    diag(Loc, "禁止函数参数省略类型") << FD->getParametersSourceRange();
    return;
  }
}
} // namespace clang::tidy::gjb8114