#include "LoopBracesCheck.h"
#include "clang/Lex/Lexer.h"

using namespace clang::ast_matchers;

namespace clang::tidy::gjb8114 {
void LoopBracesCheck::registerMatchers(MatchFinder *Finder) {
  Finder->addMatcher(
      forStmt(hasBody(stmt(unless(compoundStmt())))).bind("for-loop"), this);
  Finder->addMatcher(
      whileStmt(hasBody(stmt(unless(compoundStmt())))).bind("while-loop"),
      this);
  Finder->addMatcher(
      doStmt(hasBody(stmt(unless(compoundStmt())))).bind("do-loop"), this);
}

void LoopBracesCheck::emitDiagnosticWithFixIt(
    const Stmt *LoopBody, const MatchFinder::MatchResult &Result) {
  diag(LoopBody->getBeginLoc(), "禁止循环体省略大括号")
      << FixItHint::CreateInsertion(LoopBody->getBeginLoc(), "{\n  ")
      << FixItHint::CreateInsertion(
             Lexer::getLocForEndOfToken(LoopBody->getEndLoc(), 0,
                                        *Result.SourceManager,
                                        Result.Context->getLangOpts()),
             "\n}");
}

void LoopBracesCheck::check(const MatchFinder::MatchResult &Result) {
  if (const auto *ForLoop = Result.Nodes.getNodeAs<ForStmt>("for-loop")) {
    emitDiagnosticWithFixIt(ForLoop->getBody(), Result);
    return;
  }
  if (const auto *WhileLoop = Result.Nodes.getNodeAs<WhileStmt>("while-loop")) {
    emitDiagnosticWithFixIt(WhileLoop->getBody(), Result);
    return;
  }
  if (const auto *DoLoop = Result.Nodes.getNodeAs<DoStmt>("do-loop")) {
    emitDiagnosticWithFixIt(DoLoop->getBody(), Result);
    return;
  }
}
} // namespace clang::tidy::gjb8114