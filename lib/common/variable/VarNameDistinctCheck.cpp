#include "VarNameDistinctCheck.h"

using namespace clang::ast_matchers;

namespace clang::tidy::gjb8114 {

static const VarDecl *findConflictingVar(
    const std::string &Key, const std::string &Name,
    const std::unordered_map<std::string, const VarDecl *> &Vars) {
  auto It = Vars.find(Key);
  if (It != Vars.end()) {
    const VarDecl *ConflictingVar = It->second;
    // Only report if the names are actually different (case sensitive
    // comparison) This avoids reporting redeclaration of the same variable
    if (ConflictingVar->getName().str() != Name) {
      return ConflictingVar;
    }
  }
  return nullptr;
}

void VarNameDistinctCheck::registerMatchers(MatchFinder *Finder) {
  Finder->addMatcher(
      varDecl(unless(isImplicit()), unless(isExpansionInSystemHeader()))
          .bind("var"),
      this);
}

void VarNameDistinctCheck::check(const MatchFinder::MatchResult &Result) {
  const auto *VD = Result.Nodes.getNodeAs<VarDecl>("var");
  if (!VD)
    return;

  const auto VarName = VD->getName().str();
  if (VarName.empty())
    return;
  const auto Key = getKey(VarName);

  const auto *DeclCtx = VD->getDeclContext();
  if (DeclCtx != CurrentDeclContext) {
    // clear the local variable map if we are in a different
    // function/method/namespace
    LocalVars.clear();
    CurrentDeclContext = DeclCtx;
  }

  const VarDecl *Conflict = nullptr;
  auto IsFuncScoped = DeclCtx->isFunctionOrMethod();
  if (IsFuncScoped)
    Conflict = findConflictingVar(Key, VarName, LocalVars);
  if (!Conflict)
    Conflict = findConflictingVar(Key, VarName, DeclaredVars);

  if (Conflict) {
    auto Loc = VD->getLocation();
    if (Loc.isValid()) {
      diag(Loc, getDescription());
      diag(Conflict->getLocation(), "相似变量在此处声明", DiagnosticIDs::Note);
    }
  }

  (IsFuncScoped ? LocalVars : DeclaredVars)[Key] = VD;
}
} // namespace clang::tidy::gjb8114
