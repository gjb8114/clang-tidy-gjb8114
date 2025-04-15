#include "BoolSwitchCheck.h"
#include "matchers/matcher.h"

using namespace clang::ast_matchers;

namespace clang::tidy::gjb8114 {
void BoolSwitchCheck::registerMatchers(MatchFinder *Finder) {
  Finder->addMatcher(
      switchStmt(hasCondition(ignoringParenImpCasts(
                     expr(hasType(booleanType())).bind("boolExpr"))),
                 unless(isExpansionInSystemHeader()))
          .bind("switch"),
      this);
  // following for C
  Finder->addMatcher(
      switchStmt(
          hasCondition(binaryOperator(
                           anyOf(hasOperatorName("=="), hasOperatorName("!="),
                                 hasOperatorName("<"), hasOperatorName(">"),
                                 hasOperatorName("<="), hasOperatorName(">="),
                                 hasOperatorName("&&"), hasOperatorName("||")))
                           .bind("boolExpr")),
          unless(isExpansionInSystemHeader()))
          .bind("switch"),
      this);
  Finder->addMatcher(
      switchStmt(
          hasCondition(unaryOperator(hasOperatorName("!")).bind("boolExpr")),
          unless(isExpansionInSystemHeader()))
          .bind("switch"),
      this);
}

void BoolSwitchCheck::check(const MatchFinder::MatchResult &Result) {
  if (const auto *Switch = Result.Nodes.getNodeAs<SwitchStmt>("switch"))
    diag(Switch->getCond()->getBeginLoc(), "禁止对逻辑表达式使用 switch 语句");
}
} // namespace clang::tidy::gjb8114