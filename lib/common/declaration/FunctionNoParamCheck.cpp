#include "FunctionNoParamCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/Lex/Lexer.h"

using namespace clang::ast_matchers;

namespace clang::tidy::gjb8114 {
namespace {
AST_MATCHER(FunctionDecl, isNonPrototype) {
  return Node.getType()->isFunctionNoProtoType();
}

std::optional<SourceLocation>
checkCxxProtoExplictVoidParam(const FunctionDecl *FD, const SourceManager &SM,
                              const LangOptions &LangOpts) {
  if (FD->isImplicit() || FD->getNumParams() != 0)
    return {};

  const auto *TSI = FD->getTypeSourceInfo();
  if (!TSI)
    return {};
  const auto ProtoLoc = TSI->getTypeLoc().getAs<FunctionProtoTypeLoc>();
  if (!ProtoLoc)
    return {};
  auto LParen = ProtoLoc.getLParenLoc();
  auto RParen = ProtoLoc.getRParenLoc();
  if (LParen.isInvalid() || RParen.isInvalid())
    return {};

  auto ParamText = Lexer::getSourceText(
      CharSourceRange::getTokenRange(LParen, RParen), SM, LangOpts);
  ParamText = ParamText.trim();
  if (ParamText.size() < 2)
    return {};
  auto T = ParamText.substr(1, ParamText.size() - 2).trim();
  if (T.empty()) {
    return {FD->getLocation()};
  }
  return {};
}
} // namespace

void FunctionNoParamCheck::registerMatchers(MatchFinder *Finder) {
  Finder->addMatcher(functionDecl(isNonPrototype()).bind("nonPrototype"), this);
  Finder->addMatcher(functionDecl(unless(isDefinition())).bind("funcDecl"),
                     this);
}

static const char *Description = "禁止函数参数表为空时显式声明 void";

void FunctionNoParamCheck::check(const MatchFinder::MatchResult &Result) {
  if (const auto *FD = Result.Nodes.getNodeAs<FunctionDecl>("nonPrototype")) {
    if ((*Result.SourceManager).isInSystemHeader(FD->getLocation()))
      return;

    if (FD->isVariadic())
      return;

    diag(FD->getLocation(), Description);
  }

  auto LO = Result.Context->getLangOpts();
  if (LO.CPlusPlus) {
    const auto *FD = Result.Nodes.getNodeAs<FunctionDecl>("funcDecl");
    if (!FD)
      return;
    auto Loc = checkCxxProtoExplictVoidParam(FD, *Result.SourceManager, LO);
    if (Loc)
      diag(*Loc, Description);
  }
}
} // namespace clang::tidy::gjb8114
