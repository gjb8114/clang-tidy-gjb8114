#include "SingleLetterVarNameCheck.h"

using namespace clang::ast_matchers;

namespace clang::tidy::gjb8114 {
void SingleLetterVarNameCheck::registerMatchers(MatchFinder *Finder) {
  Finder->addMatcher(varDecl(unless(isImplicit()),
                             unless(isExpansionInSystemHeader()),
                             unless(hasAncestor(declRefExpr())))
                         .bind("var"),
                     this);
}

void SingleLetterVarNameCheck::check(const MatchFinder::MatchResult &Result) {
  const auto *VD = Result.Nodes.getNodeAs<VarDecl>("var");
  if (!VD)
    return;
  const auto VarName = VD->getName();
  if (VarName != "l" && VarName != "O")
    return;
  if (ReportedVars.insert(VD).second)
    diag(VD->getLocation(),
         "禁止单独使用小写字母 “l” 或大写字母 “O” 作为变量名");
}
} // namespace clang::tidy::gjb8114