#include "IdentifierNameConflictCheck.h"

using namespace clang::ast_matchers;

namespace clang::tidy::gjb8114 {
void IdentifierNameConflictCheck::registerMatchers(MatchFinder *Finder) {
  Finder->addMatcher(
      varDecl(unless(isImplicit()), unless(isExpansionInSystemHeader()))
          .bind("var"),
      this);
  Finder->addMatcher(namedDecl(unless(isExpansionInSystemHeader()),
                               anyOf(recordDecl(), // structs, classes, unions
                                     typedefDecl(), enumDecl()))
                         .bind("type"),
                     this);
}

void IdentifierNameConflictCheck::check(
    const MatchFinder::MatchResult &Result) {
  const auto *VD = Result.Nodes.getNodeAs<VarDecl>("var");
  const auto *TD = Result.Nodes.getNodeAs<NamedDecl>("type");

  if (TD) {
    const auto TypeName = TD->getName().str();
    if (!TypeName.empty()) {
      const auto TypeLoc = Result.Context->getFullLoc(TD->getLocation());
      if (TypeLoc.isValid()) {
        DeclaredTypes[TypeName] = TD;
      }
    }
    return;
  }

  if (VD) {
    const auto VarName = VD->getName().str();
    auto It = DeclaredTypes.find(VarName);
    if (It != DeclaredTypes.end()) {
      const NamedDecl *ConflictingType = It->second;
      if (ConflictingType->isInIdentifierNamespace(Decl::IDNS_Type)) {
        diag(VD->getLocation(), "禁止变量与标识符同名");
        diag(ConflictingType->getLocation(), "冲突标识符在此处",
             DiagnosticIDs::Note);
      }
    }
  }
}
} // namespace clang::tidy::gjb8114