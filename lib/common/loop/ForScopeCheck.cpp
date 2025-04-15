#include "ForScopeCheck.h"

using namespace clang::ast_matchers;

namespace clang::tidy::gjb8114 {
void ForScopeCheck::registerMatchers(MatchFinder *Finder) {
  auto VD = varDecl(hasGlobalStorage()).bind("varDecl");
  auto RefExpr = ignoringParenImpCasts(declRefExpr(to(VD)));
  Finder->addMatcher(
      forStmt(hasLoopInit(
                  anyOf(binaryOperator(hasOperatorName("="), hasLHS(RefExpr)),
                        unaryOperator(hasUnaryOperand(RefExpr)))),
              hasIncrement(anyOf(binaryOperator(hasLHS(RefExpr)),
                                 unaryOperator(hasUnaryOperand(RefExpr)))),
              hasCondition(forEachDescendant(declRefExpr(to(VD)))))
          .bind("forStmt"),
      this);
}

void ForScopeCheck::check(const MatchFinder::MatchResult &Result) {
  if (const auto *For = Result.Nodes.getNodeAs<ForStmt>("forStmt")) {
    diag(For->getInit()->getBeginLoc(), "禁止 for 循环控制变量使用非局部变量");
    const auto *ControlVar = Result.Nodes.getNodeAs<VarDecl>("varDecl");
    diag(ControlVar->getLocation(), "定义在这里", DiagnosticIDs::Note);
  }
}
} // namespace clang::tidy::gjb8114