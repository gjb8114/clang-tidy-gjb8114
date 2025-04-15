#include "AnonRecordCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/AST/RecursiveASTVisitor.h"

using namespace clang::ast_matchers;

namespace clang::tidy::gjb8114 {
namespace {
AST_MATCHER(RecordDecl, isAnonymousRecord) {
  return Node.getIdentifier() == nullptr;
}

AST_MATCHER(EnumDecl, isAnonymousEnum) {
  return Node.getIdentifier() == nullptr;
}
} // namespace

void AnonRecordCheck::registerMatchers(MatchFinder *Finder) {
  Finder->addMatcher(recordDecl(isAnonymousRecord()).bind("anonymousRecord"),
                     this);

  Finder->addMatcher(enumDecl(isAnonymousEnum(), unless(isInStdNamespace()))
                         .bind("anonymousEnum"),
                     this);
}

void AnonRecordCheck::check(const MatchFinder::MatchResult &Result) {
  const auto *Description = "禁止匿名结构、联合、枚举";
  auto &SM = Result.Context->getSourceManager();

  const auto *Record = Result.Nodes.getNodeAs<RecordDecl>("anonymousRecord");
  if (Record) {
    if (SM.isInSystemHeader(Record->getLocation()))
      return;

    auto IsTypedef = false;
    for (const auto &D : Record->redecls()) {
      if (const auto *TD = dyn_cast<TagDecl>(D)) {
        if (TD->getTypedefNameForAnonDecl())
          IsTypedef = true;
      }
    }
    if (IsTypedef)
      return;

    diag(Record->getLocation(), Description);
  }

  if (const auto *Enum = Result.Nodes.getNodeAs<EnumDecl>("anonymousEnum")) {
    if (Enum->getTypedefNameForAnonDecl())
      return;

    if (SM.isInSystemHeader(Enum->getLocation()))
      return;

    diag(Enum->getLocation(), Description);
  }
}
} // namespace clang::tidy::gjb8114