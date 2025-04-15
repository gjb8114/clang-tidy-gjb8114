#pragma once

#include "clang/AST/AST.h"
#include "clang/Lex/Lexer.h"

namespace clang::tidy::gjb8114::predicate {
inline bool isNull(const Expr &Node) {
  const auto *Init = Node.IgnoreParenImpCasts();
  if (isa<GNUNullExpr>(Init) || isa<CXXNullPtrLiteralExpr>(Init))
    return true;

  // For C, ignore C-style cast is required
  const auto *CInit = Init->IgnoreCasts();
  return isa<IntegerLiteral>(CInit) &&
         cast<IntegerLiteral>(CInit)->getValue() == 0;
}

inline bool hasComment(const Stmt *S, const SourceManager &SM,
                       const LangOptions &LangOpts, const char *comment) {
  if (isa<NullStmt>(S)) {
    auto Loc = Lexer::getLocForEndOfToken(S->getEndLoc(), 0, SM, LangOpts);
    auto LineNo = SM.getSpellingLineNumber(S->getEndLoc());
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
        if (CommentText.contains(comment))
          return true;
      }

      Loc = Tok.getEndLoc();
      Loc = Lexer::getLocForEndOfToken(Loc, 0, SM, LangOpts);
    }
    return false;
  } else /* compound */ {
    auto Range = CharSourceRange::getCharRange(S->getSourceRange());
    auto Text = Lexer::getSourceText(Range, SM, LangOpts);
    return Text.contains(comment);
  }
};

} // namespace clang::tidy::gjb8114::predicate