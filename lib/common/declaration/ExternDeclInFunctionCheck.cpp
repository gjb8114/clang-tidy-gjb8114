#include "ExternDeclInFunctionCheck.h"

using namespace clang::ast_matchers;

namespace clang::tidy::gjb8114 {
namespace {
AST_MATCHER(Decl, hasExternalStorage) {
  if (const auto *VD = dyn_cast<VarDecl>(&Node))
    return VD->getStorageClass() == SC_Extern;
  if (const auto *FD = dyn_cast<FunctionDecl>(&Node))
    return FD->getStorageClass() == SC_Extern;
  return false;
}
} // namespace

void ExternDeclInFunctionCheck::registerMatchers(MatchFinder *Finder) {
  Finder->addMatcher(
      functionDecl(hasDescendant(declStmt(
                       has(varDecl(hasExternalStorage()).bind("externVar")))))
          .bind("function"),
      this);
  Finder->addMatcher(
      functionDecl(hasDescendant(declStmt(has(
                       functionDecl(hasExternalStorage()).bind("externFunc")))))
          .bind("function"),
      this);
}

void ExternDeclInFunctionCheck::check(const MatchFinder::MatchResult &Result) {
  const auto *Function = Result.Nodes.getNodeAs<FunctionDecl>("function");
  if (!Function)
    return;

  const char *Description = "禁止函数体内 extern 声明";

  if (const auto *EVD = Result.Nodes.getNodeAs<VarDecl>("externVar"))
    diag(EVD->getBeginLoc(), Description);

  if (const auto *EFD = Result.Nodes.getNodeAs<FunctionDecl>("externFunc"))
    diag(EFD->getBeginLoc(), Description);
}
} // namespace clang::tidy::gjb8114