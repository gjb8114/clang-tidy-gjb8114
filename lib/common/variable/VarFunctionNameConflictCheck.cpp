#include "VarFunctionNameConflictCheck.h"

using namespace clang::ast_matchers;

namespace clang::tidy::gjb8114 {
void VarFunctionNameConflictCheck::registerMatchers(MatchFinder *Finder) {
  Finder->addMatcher(varDecl(unless(isExpansionInSystemHeader()),
                             hasAncestor(functionDecl()), unless(parmVarDecl()))
                         .bind("localVar"),
                     this);
}

void VarFunctionNameConflictCheck::check(
    const MatchFinder::MatchResult &Result) {
  const auto *LocalVar = Result.Nodes.getNodeAs<VarDecl>("localVar");
  if (!LocalVar)
    return;

  const auto VarName = LocalVar->getName();
  if (VarName.empty())
    return;

  for (const auto *D : Result.Context->getTranslationUnitDecl()->decls()) {
    const auto *FD = dyn_cast<FunctionDecl>(D);
    if (!FD)
      continue;

    if (FD->getName() == VarName) {
      diag(LocalVar->getLocation(), "禁止变量与函数同名")
          << LocalVar->getSourceRange();
      diag(FD->getLocation(), "函数在此处声明", DiagnosticIDs::Note);
      break;
    }
  }
}
} // namespace clang::tidy::gjb8114