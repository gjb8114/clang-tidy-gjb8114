#include "ForTypeCheck.h"

using namespace clang::ast_matchers;

namespace clang::tidy::gjb8114 {
void ForTypeCheck::registerMatchers(MatchFinder *Finder) {
  Finder->addMatcher(
      forStmt(hasLoopInit(anyOf(
                  // Match init statements like: for (float f = 0.0; ...)
                  declStmt(containsDeclaration(
                      0, varDecl(hasType(realFloatingPointType()))
                             .bind("declControl"))),
                  // Match init assignments like: for (f = 0.0; ...)
                  binaryOperator(hasOperatorName("="),
                                 hasLHS(ignoringParenImpCasts(declRefExpr(to(
                                     varDecl(hasType(realFloatingPointType()))
                                         .bind("refControl")))))))))
          .bind("forStmt"),
      this);
}

void ForTypeCheck::check(const MatchFinder::MatchResult &Result) {
  const auto *For = Result.Nodes.getNodeAs<ForStmt>("forStmt");
  if (!For)
    return;

  const auto *ControlVar = Result.Nodes.getNodeAs<VarDecl>("declControl");
  if (!ControlVar)
    ControlVar = Result.Nodes.getNodeAs<VarDecl>("refControl");
  diag(ControlVar->getBeginLoc(), "禁止 for 循环控制变量为非整数型");
}
} // namespace clang::tidy::gjb8114