#include "FloatToIntAssignCheck.h"

using namespace clang::ast_matchers;

namespace clang::tidy::gjb8114 {
void FloatToIntAssignCheck::registerMatchers(MatchFinder *Finder) {
  Finder->addMatcher(
      varDecl(
          unless(isExpansionInSystemHeader()), hasType(isInteger()),
          hasInitializer(ignoringParenImpCasts(floatLiteral().bind("init")))),
      this);
  Finder->addMatcher(
      binaryOperator(
          unless(isExpansionInSystemHeader()), isAssignmentOperator(),
          hasLHS(expr(hasType(isInteger()))),
          hasRHS(ignoringParenImpCasts(floatLiteral().bind("assign")))),
      this);
}

void FloatToIntAssignCheck::check(const MatchFinder::MatchResult &Result) {
  const char *Description = "禁止将浮点字面量赋值给整型变量";
  if (const auto *Init = Result.Nodes.getNodeAs<FloatingLiteral>("init"))
    diag(Init->getBeginLoc(), Description);
  if (const auto *Assign = Result.Nodes.getNodeAs<FloatingLiteral>("assign"))
    diag(Assign->getBeginLoc(), Description);
}
} // namespace clang::tidy::gjb8114