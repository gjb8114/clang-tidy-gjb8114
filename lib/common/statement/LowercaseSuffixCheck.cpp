#include "LowercaseSuffixCheck.h"
#include "clang/Lex/Lexer.h"

using namespace clang::ast_matchers;

namespace clang::tidy::gjb8114 {
void LowercaseSuffixCheck::registerMatchers(MatchFinder *Finder) {
  Finder->addMatcher(
      expr(anyOf(integerLiteral().bind("int"), floatLiteral().bind("float"))),
      this);
}

void LowercaseSuffixCheck::check(const MatchFinder::MatchResult &Result) {
  const Expr *IntLiteral = Result.Nodes.getNodeAs<IntegerLiteral>("int");
  const Expr *FloatLiteral = Result.Nodes.getNodeAs<FloatingLiteral>("float");

  for (const auto *Literal : {IntLiteral, FloatLiteral}) {
    if (!Literal)
      continue;

    const SourceManager &SM = *Result.SourceManager;
    const LangOptions &LangOpts = Result.Context->getLangOpts();

    CharSourceRange Range =
        CharSourceRange::getTokenRange(Literal->getSourceRange());
    auto Text = Lexer::getSourceText(Range, SM, LangOpts);
    auto LowerCaseSuffixes = {"u", "l", "ul", "lu", "ll", "ull", "llu", "f"};
    auto Found = std::ranges::find_if(
        LowerCaseSuffixes, [&](auto Suffix) { return Text.ends_with(Suffix); });
    if (Found != LowerCaseSuffixes.end()) {
      auto FixItText = std::string{*Found};
      auto SuffixLen = FixItText.size();
      auto Offset = Text.size() - SuffixLen;
      auto SuffixLoc = Literal->getBeginLoc().getLocWithOffset(Offset);
      auto FixItRange = CharSourceRange::getTokenRange(
          SourceRange(SuffixLoc, SuffixLoc.getLocWithOffset(SuffixLen)));
      std::ranges::transform(FixItText, FixItText.begin(), ::toupper);
      diag(SuffixLoc, "禁止小写字母数字后缀")
          << FixItHint::CreateReplacement(FixItRange, FixItText);
    }
  }
}
} // namespace clang::tidy::gjb8114