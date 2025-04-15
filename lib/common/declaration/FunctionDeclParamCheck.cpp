#include "FunctionDeclParamCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::gjb8114 {
void FunctionDeclParamCheck::registerMatchers(MatchFinder *Finder) {
  Finder->addMatcher(functionDecl(unless(isDefinition())).bind("funcDecl"),
                     this);
}

void FunctionDeclParamCheck::check(const MatchFinder::MatchResult &Result) {
  const auto *FD = Result.Nodes.getNodeAs<FunctionDecl>("funcDecl");
  if (!FD)
    return;

  if (Result.SourceManager->isInSystemHeader(FD->getLocation()))
    return;

  if (FD->isVariadic())
    return;

  const char *Description = "禁止函数声明省略参数类型或名称";
  if (FD->getNumParams() == 0) {
    const FunctionDecl *Definition = nullptr;
    if (FD->hasBody(Definition) && Definition &&
        Definition->getNumParams() > 0) {
      diag(FD->getLocation(), Description);
    }
    return;
  }

  for (const auto *Param : FD->parameters()) {
    if (Param->getName().empty()) {
      diag(FD->getLocation(), Description);
      return;
    }
  }
}
} // namespace clang::tidy::gjb8114