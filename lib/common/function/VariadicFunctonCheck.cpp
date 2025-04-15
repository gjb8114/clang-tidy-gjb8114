#include "VariadicFunctionCheck.h"
#include "matchers/matcher.h"

using namespace clang::ast_matchers;

namespace clang::tidy::gjb8114 {
namespace {
// getEllipsisLoc() always returns the location of prototype, even if FuncDecl
// is a definition that leads to duplicated diag to the function declaration.
auto getFuncDefinitionEllipsisLoc(const FunctionDecl *FD, SourceManager &SM,
                                  ASTContext &Ctx) {
  SourceLocation Result = FD->getLocation();
  const auto *TSI = FD->getTypeSourceInfo();
  if (!TSI)
    return Result;
  const auto ProtoLoc = TSI->getTypeLoc().getAs<FunctionProtoTypeLoc>();
  if (!ProtoLoc)
    return Result;
  auto LParen = ProtoLoc.getLParenLoc();
  auto RParen = ProtoLoc.getRParenLoc();
  if (LParen.isInvalid() || RParen.isInvalid())
    return Result;

  SourceRange SearchRange = SourceRange(LParen, RParen);
  bool Invalid = false;
  auto Range = CharSourceRange::getTokenRange(SearchRange);
  auto Buffer = Lexer::getSourceText(Range, SM, Ctx.getLangOpts(), &Invalid);
  if (Invalid || Buffer.empty())
    return Result;

  Lexer RawLexer(SearchRange.getBegin(), Ctx.getLangOpts(), Buffer.begin(),
                 Buffer.begin(), Buffer.end());
  RawLexer.SetKeepWhitespaceMode(false);

  Token Tok;
  while (!RawLexer.LexFromRawLexer(Tok)) {
    if (Tok.is(tok::ellipsis))
      return Tok.getLocation();
    if (Tok.is(tok::eof))
      break;
  }
  return Result;
}
} // namespace

void VariadicFunctonCheck::registerMatchers(MatchFinder *Finder) {
  Finder->addMatcher(functionDecl(unless(isExpansionInSystemHeader()),
                                  unless(isImplicit()),
                                  unless(isInStdNamespace()), isVariadic())
                         .bind("func_decl"),
                     this);
}

void VariadicFunctonCheck::check(const MatchFinder::MatchResult &Result) {
  const auto *FuncDecl = Result.Nodes.getNodeAs<FunctionDecl>("func_decl");
  if (!FuncDecl)
    return;
  auto &SM = FuncDecl->getASTContext().getSourceManager();
  auto Loc = FuncDecl->isThisDeclarationADefinition()
                 ? getFuncDefinitionEllipsisLoc(FuncDecl, SM,
                                                FuncDecl->getASTContext())
                 : FuncDecl->getEllipsisLoc();
  diag(Loc, "禁止函数参数列表中使用省略号");
}
} // namespace clang::tidy::gjb8114