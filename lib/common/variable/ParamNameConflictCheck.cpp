#include "ParamNameConflictCheck.h"

using namespace clang::ast_matchers;

namespace clang::tidy::gjb8114 {
void ParamNameConflictCheck::registerMatchers(MatchFinder *Finder) {
  Finder->addMatcher(parmVarDecl(hasAncestor(functionDecl(
                                     unless(isExpansionInSystemHeader()))))
                         .bind("param"),
                     this);
}

void ParamNameConflictCheck::check(const MatchFinder::MatchResult &Result) {
  const auto *Param = Result.Nodes.getNodeAs<ParmVarDecl>("param");
  if (!Param || !Param->getIdentifier())
    return;

  const auto ParamName = Param->getName();
  if (ParamName.empty())
    return;

  ASTContext &Context = *Result.Context;
  for (const Decl *D : Context.getTranslationUnitDecl()->decls()) {
    if (const auto *VD = dyn_cast<VarDecl>(D)) {
      if (VD->isFileVarDecl() && VD->hasGlobalStorage() &&
          VD->getIdentifier() && VD->getName() == ParamName) {
        diag(Param->getLocation(), "禁止形参与全局变量同名")
            << Param->getSourceRange();
        diag(VD->getLocation(), "全局变量定义在此处", DiagnosticIDs::Note);
        break;
      }
    }
  }
}
} // namespace clang::tidy::gjb8114