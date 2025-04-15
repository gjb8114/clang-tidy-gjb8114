#include "OctalNumberCheck.h"
#include "clang/Lex/Lexer.h"

using namespace clang::ast_matchers;

namespace clang::tidy::gjb8114 {

void OctalNumberCheck::registerMatchers(MatchFinder *Finder) {
  Finder->addMatcher(
      integerLiteral(unless(isExpansionInSystemHeader())).bind("octal"), this);
}

static bool hasOctalComment(const IntegerLiteral *Literal,
                            const SourceManager &SM,
                            const LangOptions &LangOpts) {
  auto Loc = Lexer::getLocForEndOfToken(Literal->getEndLoc(), 0, SM, LangOpts);
  auto LineNo = SM.getSpellingLineNumber(Literal->getEndLoc());
  while (Loc.isValid()) {
    Token Tok;
    if (Lexer::getRawToken(Loc, Tok, SM, LangOpts, true))
      break;

    auto TokLineNo = SM.getSpellingLineNumber(Tok.getLocation());
    if (TokLineNo > LineNo)
      break;

    if (Tok.is(tok::comment)) {
      StringRef CommentText(SM.getCharacterData(Tok.getLocation()),
                            Tok.getLength());
      if (CommentText.contains("/* octal */"))
        return true;
    }

    Loc = Tok.getEndLoc();
    Loc = Lexer::getLocForEndOfToken(Loc, 0, SM, LangOpts);
  }
  return false;
}

void OctalNumberCheck::check(const MatchFinder::MatchResult &Result) {
  const auto *Literal = Result.Nodes.getNodeAs<IntegerLiteral>("octal");
  if (!Literal)
    return;

  const SourceManager &SM = *Result.SourceManager;
  const LangOptions &LangOpts = Result.Context->getLangOpts();
  SourceLocation Loc = Literal->getBeginLoc();
  if (!Loc.isValid())
    return;

  bool Invalid = false;
  auto Text = Lexer::getSourceText(CharSourceRange::getTokenRange(Loc), SM,
                                   LangOpts, &Invalid);
  if (Invalid || Text.empty())
    return;

  bool IsOctal = Text.size() >= 2 && Text[0] == '0' &&
                 !(Text.size() >= 3 && (Text[1] == 'x' || Text[1] == 'X' ||
                                        Text[1] == 'b' || Text[1] == 'B'));
  if (!IsOctal)
    return;
  if (!hasOctalComment(Literal, SM, LangOpts))
    diag(Literal->getBeginLoc(), "禁止使用八进制数");
}

} // namespace clang::tidy::gjb8114