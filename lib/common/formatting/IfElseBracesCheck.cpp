#include "IfElseBracesCheck.h"

#include "clang/Lex/Lexer.h"

using namespace clang::ast_matchers;

namespace clang::tidy::gjb8114 {
void IfElseBracesCheck::registerMatchers(MatchFinder *Finder) {
  Finder->addMatcher(ifStmt(unless(isExpansionInSystemHeader()),
                            hasThen(stmt(unless(compoundStmt())).bind("then")))
                         .bind("if"),
                     this);
  Finder->addMatcher(
      ifStmt(unless(isExpansionInSystemHeader()),
             hasElse(stmt(allOf(unless(compoundStmt()), unless(ifStmt())))
                         .bind("else")))
          .bind("if-else"),
      this);
}

void IfElseBracesCheck::emitDiagnosticWithFixIt(
    const Stmt *Body, StringRef StmtKind,
    const MatchFinder::MatchResult &Result) {
  diag(Body->getBeginLoc(), "禁止 %0 省略大括号")
      << StmtKind.str()
      << FixItHint::CreateInsertion(Body->getBeginLoc(), "{\n  ")
      << FixItHint::CreateInsertion(
             Lexer::getLocForEndOfToken(Body->getEndLoc(), 0,
                                        *Result.SourceManager,
                                        Result.Context->getLangOpts()),
             "\n}");
}

void IfElseBracesCheck::check(const MatchFinder::MatchResult &Result) {
  if (const auto *ThenStmt = Result.Nodes.getNodeAs<Stmt>("then")) {
    const auto *IfStatement = Result.Nodes.getNodeAs<IfStmt>("if");
    if (IfStatement && IfStatement->getThen() == ThenStmt) {
      bool IsElseIf = false;
      const auto &Parents = Result.Context->getParents(*IfStatement);
      if (auto *DTNL = Parents.empty() ? nullptr : Parents[0].get<IfStmt>()) {
        if (DTNL->getElse() == IfStatement) {
          emitDiagnosticWithFixIt(ThenStmt, "else if", Result);
          IsElseIf = true;
        }
      }

      if (!IsElseIf) {
        emitDiagnosticWithFixIt(ThenStmt, "if", Result);
      }
    }
  }

  if (const auto *ElseStmt = Result.Nodes.getNodeAs<Stmt>("else")) {
    emitDiagnosticWithFixIt(ElseStmt, "else", Result);
  }
}
} // namespace clang::tidy::gjb8114