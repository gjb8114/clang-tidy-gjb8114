#include "UselessStatementCheck.h"

using namespace clang::ast_matchers;

namespace clang::tidy::gjb8114 {

void UselessStatementCheck::registerMatchers(MatchFinder *Finder) {
  // Match simple variable references as standalone statements (e.g., "local;")
  Finder->addMatcher(
      expr(hasParent(compoundStmt()),
           ignoringParenImpCasts(declRefExpr().bind("uselessRef")),
           // Not within control structures
           unless(
               hasAncestor(stmt(anyOf(ifStmt(), whileStmt(), forStmt(),
                                      doStmt(), switchStmt(), returnStmt()))))),
      this);

  // Match binary operations without side effects (e.g., "a + b;", "a == b;")
  Finder->addMatcher(
      binaryOperator(anyOf(hasOperatorName("-"), hasOperatorName("+"),
                           hasOperatorName("*"), hasOperatorName("/"),
                           hasOperatorName("%"), hasOperatorName("<<"),
                           hasOperatorName(">>"), hasOperatorName("&"),
                           hasOperatorName("|"), hasOperatorName("^"),
                           hasOperatorName("=="), hasOperatorName("!="),
                           hasOperatorName("<"), hasOperatorName(">"),
                           hasOperatorName("<="), hasOperatorName(">=")),
                     // Make sure it's a standalone statement
                     hasParent(compoundStmt()),
                     // Not within control structures
                     unless(hasAncestor(
                         stmt(anyOf(ifStmt(), whileStmt(), forStmt(), doStmt(),
                                    switchStmt(), returnStmt())))))
          .bind("uselessBinOp"),
      this);
}

void UselessStatementCheck::check(const MatchFinder::MatchResult &Result) {
  const auto *Description = "禁止无效语句";
  if (const auto *UselessRef =
          Result.Nodes.getNodeAs<DeclRefExpr>("uselessRef")) {
    if (UselessRef->getBeginLoc().isMacroID())
      return;

    diag(UselessRef->getBeginLoc(), Description);
    return;
  }

  if (const auto *UselessBinOp =
          Result.Nodes.getNodeAs<BinaryOperator>("uselessBinOp")) {
    if (UselessBinOp->getBeginLoc().isMacroID())
      return;

    diag(UselessBinOp->getBeginLoc(), Description);
  }
}

} // namespace clang::tidy::gjb8114