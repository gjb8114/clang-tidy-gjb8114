#include "BitFieldExplictSignedCheck.h"
#include "clang/AST/ASTContext.h"
#include <clang/Lex/Lexer.h>

using namespace clang::ast_matchers;

namespace clang::tidy::gjb8114 {
void BitFieldExplictSignedCheck::registerMatchers(MatchFinder *Finder) {
  Finder->addMatcher(
      fieldDecl(isBitField(), hasType(isInteger())).bind("bitField"), this);
}

void BitFieldExplictSignedCheck::check(const MatchFinder::MatchResult &Result) {
  const auto *FD = Result.Nodes.getNodeAs<FieldDecl>("bitField");
  if (!FD)
    return;

  auto Loc = FD->getLocation();
  const auto &SM = *Result.SourceManager;
  if (Loc.isInvalid() || SM.isInSystemHeader(Loc))
    return;

  QualType Type = FD->getType();
  auto TName = Type.getAsString();
  std::string CheckList[] = {"char", "short", "int", "long", "long long"};
  if (std::ranges::find(CheckList, TName) == std::end(CheckList))
    return;

  if (TypeSourceInfo *TSI = FD->getTypeSourceInfo()) {
    TypeLoc TL = TSI->getTypeLoc();
    auto Range = CharSourceRange::getTokenRange(TL.getSourceRange());
    auto Text = Lexer::getSourceText(Range, SM, Result.Context->getLangOpts());
    if (!Text.starts_with("signed"))
      diag(FD->getTypeSpecStartLoc(), "禁止位域未明确符号性");
  }
}
} // namespace clang::tidy::gjb8114