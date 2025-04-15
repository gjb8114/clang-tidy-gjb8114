#include "TypedefVarNameConflictCheck.h"

using namespace clang::ast_matchers;

namespace clang::tidy::gjb8114 {
void TypedefVarNameConflictCheck::registerMatchers(MatchFinder *Finder) {
  Finder->addMatcher(
      varDecl(unless(isImplicit()), unless(isExpansionInSystemHeader()))
          .bind("var"),
      this);
  Finder->addMatcher(
      typedefDecl(unless(isExpansionInSystemHeader())).bind("typedef"),
      this);
}

void TypedefVarNameConflictCheck::check(
    const MatchFinder::MatchResult &Result) {
  const auto *VD = Result.Nodes.getNodeAs<VarDecl>("var");
  const auto *TD = Result.Nodes.getNodeAs<TypedefNameDecl>("typedef");

  if (TD) {
    const auto TypeName = TD->getName().str();
    if (!TypeName.empty()) {
      const auto TypeLoc = Result.Context->getFullLoc(TD->getLocation());
      if (TypeLoc.isValid()) {
        DeclaredTypedefs[TypeName] = TD;
      }
    }
    return;
  }

  if (VD) {
    const auto VarName = VD->getName().str();
    auto It = DeclaredTypedefs.find(VarName);
    if (It != DeclaredTypedefs.end()) {
      const TypedefNameDecl *ConflictingTypedef = It->second;
      diag(VD->getLocation(), "禁止变量与 typedef 类型同名");
      diag(ConflictingTypedef->getLocation(), "冲突类型在此处",
           DiagnosticIDs::Note);
    }
  }
}
} // namespace clang::tidy::gjb8114