#include "NullAsIntegerCheck.h"

using namespace clang::ast_matchers;
namespace clang::tidy::gjb8114 {
void NullAsIntegerCheck::registerMatchers(
    clang::ast_matchers::MatchFinder *Finder) {
  Finder->addMatcher(
      implicitCastExpr(unless(isExpansionInSystemHeader()),
                       hasSourceExpression(anyOf(
                           // C NULL
                           ignoringParens(cStyleCastExpr(
                               hasSourceExpression(integerLiteral(equals(0))))),
                           // C++ NULL
                           gnuNullExpr())),
                       hasImplicitDestinationType(isInteger()))
          .bind("implicitCastNullToInt"),
      this);
}

void NullAsIntegerCheck::check(const MatchFinder::MatchResult &Result) {
  const auto *ImplicitCast =
      Result.Nodes.getNodeAs<ImplicitCastExpr>("implicitCastNullToInt");
  if (!ImplicitCast)
    return;

  auto &SM = *Result.SourceManager;
  if (ImplicitCast->getExprLoc().isMacroID()) {
    auto MacroLoc = SM.getExpansionLoc(ImplicitCast->getBeginLoc());
    auto MacroRange = CharSourceRange::getTokenRange(MacroLoc, MacroLoc);
    diag(MacroLoc, "禁止将 NULL 用作整型数") << MacroRange;
  }
}
} // namespace clang::tidy::gjb8114