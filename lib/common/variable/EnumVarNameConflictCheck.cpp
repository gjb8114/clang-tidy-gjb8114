#include "EnumVarNameConflictCheck.h"

using namespace clang::ast_matchers;

namespace clang::tidy::gjb8114 {
void EnumVarNameConflictCheck::registerMatchers(MatchFinder *Finder) {
  Finder->addMatcher(
      varDecl(unless(isImplicit()), unless(isExpansionInSystemHeader()))
          .bind("var"),
      this);
  Finder->addMatcher(enumConstantDecl(unless(isExpansionInSystemHeader()))
                         .bind("enumConstant"),
                     this);
}

void EnumVarNameConflictCheck::check(const MatchFinder::MatchResult &Result) {
  const auto *VD = Result.Nodes.getNodeAs<VarDecl>("var");
  const auto *ECD = Result.Nodes.getNodeAs<EnumConstantDecl>("enumConstant");

  if (ECD) {
    const auto EnumElementName = ECD->getName().str();
    if (!EnumElementName.empty()) {
      const auto EnumLoc = Result.Context->getFullLoc(ECD->getLocation());
      if (EnumLoc.isValid())
        DeclaredEnumElements[EnumElementName] = ECD;
    }
    return;
  }

  if (VD) {
    const auto VarName = VD->getName().str();
    auto It = DeclaredEnumElements.find(VarName);
    if (It != DeclaredEnumElements.end()) {
      const EnumConstantDecl *ConflictingEnum = It->second;
      diag(VD->getLocation(), "禁止变量与枚举元素同名");
      diag(ConflictingEnum->getLocation(), "枚举元素在此处",
           DiagnosticIDs::Note);
    }
  }
}
} // namespace clang::tidy::gjb8114