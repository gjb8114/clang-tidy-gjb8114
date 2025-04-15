#include "TypedefRedefCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::gjb8114 {
void TypedefRedefCheck::registerMatchers(MatchFinder *Finder) {
  Finder->addMatcher(typedefDecl().bind("typedef"), this);
}

void TypedefRedefCheck::check(const MatchFinder::MatchResult &Result) {
  const auto *TD = Result.Nodes.getNodeAs<TypedefDecl>("typedef");
  if (!TD)
    return;

  const auto Loc = TD->getLocation();
  if (Loc.isInvalid() ||
      Result.Context->getSourceManager().isInSystemHeader(Loc))
    return;

  auto Name = TD->getDeclName();
  auto *Prev = Result.Context->getTranslationUnitDecl()->lookup(Name).front();
  if (isa_and_nonnull<TypedefNameDecl>(Prev) && Prev != TD) {
    diag(Loc, "禁止遮蔽 typedef 类型");
  }
}
} // namespace clang::tidy::gjb8114