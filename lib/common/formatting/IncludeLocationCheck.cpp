#include "IncludeLocationCheck.h"
#include "clang/Lex/Lexer.h"

using namespace clang::ast_matchers;

namespace clang::tidy::gjb8114 {

namespace {
bool isStartOfLine(const char *Pos, const char *BufferStart) {
  const char *LineStart = Pos;
  while (LineStart > BufferStart && *(LineStart - 1) != '\n')
    LineStart--;
  while (LineStart < Pos && (*LineStart == ' ' || *LineStart == '\t'))
    LineStart++;
  return LineStart == Pos;
}

bool isInStringLiteralOrComment(const char *Pos, const char *BufferStart,
                                const char *BufferEnd) {
  bool InSingleLineComment = false;
  bool InMultiLineComment = false;
  bool InStringLiteral = false;
  bool InCharLiteral = false;
  bool Escaped = false;

  for (const char *Curr = BufferStart; Curr < Pos && Curr < BufferEnd; ++Curr) {
    if (InSingleLineComment) {
      if (*Curr == '\n') {
        InSingleLineComment = false;
      }
      continue;
    }

    if (InMultiLineComment) {
      if (*Curr == '*' && *(Curr + 1) == '/') {
        InMultiLineComment = false;
        ++Curr;
      }
      continue;
    }

    if (InStringLiteral) {
      if (Escaped) {
        Escaped = false;
      } else if (*Curr == '\\') {
        Escaped = true;
      } else if (*Curr == '"') {
        InStringLiteral = false;
      }
      continue;
    }

    if (InCharLiteral) {
      if (Escaped) {
        Escaped = false;
      } else if (*Curr == '\\') {
        Escaped = true;
      } else if (*Curr == '\'') {
        InCharLiteral = false;
      }
      continue;
    }

    if (*Curr == '/') {
      if (*(Curr + 1) == '/') {
        InSingleLineComment = true;
        ++Curr;
      } else if (*(Curr + 1) == '*') {
        InMultiLineComment = true;
        ++Curr;
      }
    } else if (*Curr == '"') {
      InStringLiteral = true;
    } else if (*Curr == '\'') {
      InCharLiteral = true;
    }
  }

  return InSingleLineComment || InMultiLineComment || InStringLiteral ||
         InCharLiteral;
}
} // namespace

void IncludeLocationCheck::registerMatchers(MatchFinder *Finder) {
  Finder->addMatcher(
      functionDecl(unless(isExpansionInSystemHeader()), hasBody(compoundStmt()))
          .bind("func"),
      this);
}

void IncludeLocationCheck::check(const MatchFinder::MatchResult &Result) {
  const auto *FuncDecl = Result.Nodes.getNodeAs<FunctionDecl>("func");
  if (!FuncDecl)
    return;

  const auto &SM = *Result.SourceManager;
  if (!SM.isInMainFile(FuncDecl->getBeginLoc()))
    return;

  const Stmt *Body = FuncDecl->getBody();
  if (!Body)
    return;

  SourceRange BodyRange = Body->getSourceRange();

  bool Invalid = false;
  const char *BodyStart = SM.getCharacterData(BodyRange.getBegin(), &Invalid);
  if (Invalid)
    return;

  const char *BodyEnd = SM.getCharacterData(BodyRange.getEnd(), &Invalid);
  if (Invalid)
    return;

  auto FID = SM.getFileID(BodyRange.getBegin());
  const char *BufferStart = SM.getBufferData(FID, &Invalid).data();
  if (Invalid)
    return;

  const char *BufferEnd = BufferStart + SM.getBufferData(FID, &Invalid).size();
  if (Invalid)
    return;

  const std::string Directive = "#include";
  std::string BodyText(BodyStart, BodyEnd - BodyStart);
  size_t IncludePos = 0;
  while ((IncludePos = BodyText.find(Directive, IncludePos)) !=
         std::string::npos) {
    const char *IncludeStart = BodyStart + IncludePos;
    if (isStartOfLine(IncludeStart, BufferStart) &&
        !isInStringLiteralOrComment(IncludeStart, BufferStart, BufferEnd)) {
      auto IncludeLoc = BodyRange.getBegin().getLocWithOffset(IncludePos);
      diag(IncludeLoc, "禁止头文件包含出现在可执行代码之后");
    }
    IncludePos += Directive.size();
  }
}

} // namespace clang::tidy::gjb8114