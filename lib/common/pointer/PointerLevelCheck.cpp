#include "PointerLevelCheck.h"

using namespace clang::ast_matchers;

namespace clang::tidy::gjb8114 {
namespace {
int getPointerLevel(QualType Type) {
  int Level = 0;
  QualType CurrentType = Type;

  while (CurrentType->isPointerType()) {
    Level++;
    CurrentType = CurrentType->getPointeeType();
  }

  return Level;
}
} // namespace

void PointerLevelCheck::registerMatchers(MatchFinder *Finder) {
  Finder->addMatcher(
      varDecl(hasType(pointerType()), unless(isExpansionInSystemHeader()))
          .bind("var"),
      this);
  Finder->addMatcher(
      fieldDecl(hasType(pointerType()), unless(isExpansionInSystemHeader()))
          .bind("field"),
      this);
}

void PointerLevelCheck::check(const MatchFinder::MatchResult &Result) {
  const char *Description = "禁止多级指针";
  if (const auto *Var = Result.Nodes.getNodeAs<VarDecl>("var")) {
    if (getPointerLevel(Var->getType()) > 2)
      diag(Var->getLocation(), Description);
  }

  if (const auto *Field = Result.Nodes.getNodeAs<FieldDecl>("field")) {
    if (getPointerLevel(Field->getType()) > 2)
      diag(Field->getLocation(), Description);
  }
}
} // namespace clang::tidy::gjb8114