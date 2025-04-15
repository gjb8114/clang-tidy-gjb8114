#include "ExternDeclInitCheck.h"

using namespace clang::ast_matchers;

namespace clang::tidy::gjb8114 {
namespace {
AST_MATCHER(VarDecl, hasExternalStorageWithInit) {
  return Node.hasExternalStorage() && Node.hasInit();
}
} // namespace

void ExternDeclInitCheck::registerMatchers(MatchFinder *Finder) {
  Finder->addMatcher(varDecl(hasExternalStorageWithInit()).bind("externVar"),
                     this);
}

void ExternDeclInitCheck::check(const MatchFinder::MatchResult &Result) {
  const auto *EVD = Result.Nodes.getNodeAs<VarDecl>("externVar");
  if (!EVD)
    return;

  auto Loc = EVD->getInit()->getExprLoc();
  diag(Loc, "禁止 extern 声明初始化变量") << EVD->getSourceRange();
}
} // namespace clang::tidy::gjb8114