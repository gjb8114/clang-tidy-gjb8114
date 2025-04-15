#include "ForVarMutateCheck.h"
using namespace clang::ast_matchers;

namespace clang::tidy::gjb8114 {
void ForVarMutateCheck::registerMatchers(MatchFinder *Finder) {
  Finder->addMatcher(
      forStmt(
          hasLoopInit(
              anyOf(binaryOperator(
                        hasOperatorName("="),
                        hasLHS(declRefExpr(to(varDecl().bind("varDecl"))))),
                    declStmt(hasSingleDecl(varDecl().bind("varDecl"))))),
          hasBody(compoundStmt(forEachDescendant(
              binaryOperator(
                  anyOf(hasOperatorName("="), hasOperatorName("+="),
                        hasOperatorName("-="), hasOperatorName("*="),
                        hasOperatorName("/="), hasOperatorName("%="),
                        hasOperatorName("<<="), hasOperatorName(">>="),
                        hasOperatorName("&="), hasOperatorName("^="),
                        hasOperatorName("|=")),
                  hasLHS(declRefExpr(to(varDecl(equalsBoundNode("varDecl"))))))
                  .bind("assignment")))))
          .bind("forLoop"),
      this);
}

void ForVarMutateCheck::check(const MatchFinder::MatchResult &Result) {
  const auto *ForLoop = Result.Nodes.getNodeAs<ForStmt>("forLoop");
  const auto *Assignment = Result.Nodes.getNodeAs<BinaryOperator>("assignment");
  if (!ForLoop || !Assignment)
    return;
  diag(Assignment->getBeginLoc(), "禁止 for 循环内修改控制变量");
}
} // namespace clang::tidy::gjb8114