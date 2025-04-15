#include "AmbiguousComparisonCheck.h"
#include "clang/Lex/Lexer.h"

using namespace clang::ast_matchers;

namespace clang::tidy::gjb8114 {
static const char *Description = "禁止比较表达式中的运算项未使用括号";

namespace {
AST_MATCHER(BinaryOperator, hasLowerPrecedenceThanCompareOperator) {
  const auto *BO = dyn_cast<BinaryOperator>(&Node);
  if (BO) {
    const BinaryOperatorKind Op = BO->getOpcode();
    if (Op == BO_And || Op == BO_Xor || Op == BO_Or || Op == BO_LAnd ||
        Op == BO_LOr)
      return true;
  }
  return false;
}
} // namespace

void AmbiguousComparisonCheck::registerMatchers(MatchFinder *Finder) {
  Finder->addMatcher(
      binaryOperator(unless(isExpansionInSystemHeader()),
                     isComparisonOperator(),
                     anyOf(hasLHS(binaryOperator()), hasRHS(binaryOperator())))
          .bind("compOp"),
      this);
  Finder->addMatcher(
      binaryOperator(unless(isExpansionInSystemHeader()),
                     anyOf(hasOperatorName("&&"), hasOperatorName("||"),
                           hasOperatorName("&"), hasOperatorName("|"),
                           hasOperatorName("^")),
                     anyOf(hasRHS(ignoringParenImpCasts(
                               binaryOperator(isComparisonOperator()))),
                           hasLHS(ignoringParenImpCasts(
                               binaryOperator(isComparisonOperator())))))
          .bind("ambiguousCompOp"),
      this);
}

void AmbiguousComparisonCheck::check(const MatchFinder::MatchResult &Result) {
  const auto *AmbOp = Result.Nodes.getNodeAs<BinaryOperator>("ambiguousCompOp");
  if (AmbOp) {
    diag(AmbOp->getBeginLoc(), Description) << AmbOp->getSourceRange();
  }

  if (const auto *CompOp = Result.Nodes.getNodeAs<BinaryOperator>("compOp")) {
    diag(CompOp->getBeginLoc(), Description) << CompOp->getSourceRange();
  }
}
} // namespace clang::tidy::gjb8114