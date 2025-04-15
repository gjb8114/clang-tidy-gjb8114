#include "NestedCommentsCheck.h"
#include <clang/Lex/Lexer.h>
#include <clang/Lex/Token.h>

using namespace clang::tidy::gjb8114;
using namespace clang::ast_matchers;

void NestedCommentsCheck::registerMatchers(MatchFinder *Finder) {
  // Using the translation unit as a matcher to ensure our check is called once
  Finder->addMatcher(translationUnitDecl().bind("tu"), this);
}

void NestedCommentsCheck::registerPPCallbacks(
    const clang::SourceManager &SM, clang::Preprocessor *Pp,
    clang::Preprocessor *ModuleExpanderPP) {
  this->PP = Pp;
}

namespace {
bool isTokStringLiteral(clang::Token &Tok) {
  auto Kind = Tok.getKind();
  return Kind == clang::tok::string_literal ||
         Kind == clang::tok::wide_string_literal ||
         Kind == clang::tok::utf8_string_literal ||
         Kind == clang::tok::utf16_string_literal ||
         Kind == clang::tok::utf32_string_literal;
};
} // namespace

bool NestedCommentsCheck::isInsideStringLiteral(
    const clang::StringRef &Buffer, const clang::SourceLocation &FileLoc,
    unsigned Offset) {
  const auto Location = FileLoc.getLocWithOffset(Offset);
  clang::Lexer Lex(FileLoc, PP->getLangOpts(), Buffer.begin(), Buffer.begin(),
                   Buffer.end());
  clang::Token Tok;
  while (!Lex.LexFromRawLexer(Tok) && Tok.getLocation() < Location) {
    if (!isTokStringLiteral(Tok))
      continue;
    if (Tok.getLocation() <= Location && Location < Tok.getEndLoc())
      return true;
  }
  return false;
}

void NestedCommentsCheck::check(
    const ast_matchers::MatchFinder::MatchResult &Result) {
  if (!PP)
    return;

  const auto &SM = *Result.SourceManager;
  auto MainFileID = SM.getMainFileID();
  if (MainFileID.isInvalid())
    return;

  bool Invalid = false;
  const auto FileContent = SM.getBufferData(MainFileID, &Invalid);
  if (Invalid || FileContent.empty())
    return;

  auto FileLoc = SM.getLocForStartOfFile(MainFileID);
  auto Content = FileContent.str();
  std::string_view View{std::string_view{Content}};
  for (std::string::size_type Base = 0; Base < View.size();) {
    auto S = View.find("/*", Base);
    if (S == std::string::npos)
      return;
    auto E = View.find("*/", S + 2);
    if (E == std::string::npos)
      return;

    std::vector<std::string::size_type> Starts;
    std::string_view V{View.substr(0, E - 2)};
    for (auto NS = V.find("/*", S + 2); NS != std::string::npos;
         NS = V.find("/*", NS + 2)) {
      if (!isInsideStringLiteral(FileContent, FileLoc, NS))
        Starts.push_back(NS);
    }

    auto TillNext = View.find("/*", E + 2);
    auto Len = TillNext == std::string::npos ? std::string::npos : TillNext;
    V = View.substr(0, Len);

    std::vector<std::string::size_type> Ends;
    for (auto NE = V.find("*/", E + 2); NE != std::string::npos;
         NE = V.find("*/", NE + 2)) {
      if (isInsideStringLiteral(FileContent, FileLoc, NE))
        continue;
      Ends.push_back(NE);
      if (Starts.size() == Ends.size()) {
        break;
      }
    }

    if (!Starts.empty() && Starts.size() == Ends.size()) {
      for (auto &CS : Starts)
        diag(FileLoc.getLocWithOffset(CS), "禁止嵌套注释");
    }
    Base = (Ends.empty() ? E : Ends.back()) + 2;
  }
}