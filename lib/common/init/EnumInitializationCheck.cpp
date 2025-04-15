#include "EnumInitializationCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/AST/RecursiveASTVisitor.h"

using namespace clang::ast_matchers;

namespace clang::tidy::gjb8114 {
void EnumInitializationCheck::registerMatchers(MatchFinder *Finder) {
  Finder->addMatcher(enumDecl().bind("enum"), this);
}

void EnumInitializationCheck::check(const MatchFinder::MatchResult &Result) {
  const auto *EnumD = Result.Nodes.getNodeAs<EnumDecl>("enum");
  if (!EnumD)
    return;

  if (Result.SourceManager->isInSystemHeader(EnumD->getLocation()))
    return;

  const auto Enums = EnumD->enumerators();
  if (Enums.empty())
    return;

  const auto &FirstEnum = *Enums.begin();
  if (FirstEnum->getInitExpr())
    return;

  diag(FirstEnum->getLocation(), "禁止枚举元素定义不完整");
  diag(FirstEnum->getLocation(), "枚举元素 '%0' 在此处未定义值",
       DiagnosticIDs::Note)
      << FirstEnum->getName();
}
} // namespace clang::tidy::gjb8114