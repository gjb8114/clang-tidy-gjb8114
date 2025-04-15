#include "GlobalVarNameConflictCheck.h"

using namespace clang::ast_matchers;

namespace clang::tidy::gjb8114 {
void GlobalVarNameConflictCheck::registerMatchers(MatchFinder *Finder) {
  Finder->addMatcher(varDecl(unless(isExpansionInSystemHeader()),
                             hasAncestor(functionDecl()), unless(parmVarDecl()))
                         .bind("localVar"),
                     this);
}

void GlobalVarNameConflictCheck::check(const MatchFinder::MatchResult &Result) {
  const auto *LocalVar = Result.Nodes.getNodeAs<VarDecl>("localVar");
  if (!LocalVar)
    return;

  const auto *TUD = Result.Context->getTranslationUnitDecl();
  for (const auto *GD : TUD->decls()) {
    const auto *GVD = dyn_cast<VarDecl>(GD);
    if (!GVD || !GVD->isFileVarDecl())
      continue;

    if (GVD->getName() == LocalVar->getName()) {
      diag(LocalVar->getLocation(), "禁止局部变量与全局变量同名")
          << LocalVar->getSourceRange();
      diag(GVD->getLocation(), "全局变量在此处声明", DiagnosticIDs::Note);
      break;
    }
  }
}
} // namespace clang::tidy::gjb8114