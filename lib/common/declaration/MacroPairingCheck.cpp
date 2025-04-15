#include "MacroPairingCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/Basic/SourceManager.h"
#include "clang/Lex/Token.h"

using namespace clang::ast_matchers;

namespace clang::tidy::gjb8114 {

void MacroPairingCheck::registerPPCallbacks(const SourceManager &SM,
                                            Preprocessor *PP,
                                            Preprocessor *ModuleExpanderPP) {
  this->SM = &SM;

  auto HandleDecl = [this, PP](const Token &MacroNameTok, auto type) {
    auto Loc = MacroNameTok.getLocation();
    if (Loc.isInvalid() || PP->getSourceManager().isInSystemHeader(Loc))
      return;
    auto MacroName = MacroNameTok.getIdentifierInfo()->getName();
    MacroDecls.push_back({MacroName.str(), Loc, type});
  };
  auto Callback = std::make_unique<MacroPairingPPCallback>(
      [=](auto &Tok, auto *MD) {
        return HandleDecl(Tok, MacroDecl::Type::Define);
      },
      [=](auto &Tok, auto &, auto *) {
        return HandleDecl(Tok, MacroDecl::Type::Undef);
      });
  PP->addPPCallbacks(std::move(Callback));
}

void MacroPairingCheck::registerMatchers(MatchFinder *Finder) {
  Finder->addMatcher(functionDecl(hasBody(stmt())).bind("function"), this);
}

void MacroPairingCheck::check(const MatchFinder::MatchResult &Result) {
  const auto *FuncDecl = Result.Nodes.getNodeAs<FunctionDecl>("function");
  if (!FuncDecl)
    return;

  Functions.push_back(
      {FuncDecl->getNameAsString(), FuncDecl->getBody()->getSourceRange()});
}

void MacroPairingCheck::onEndOfTranslationUnit() {
  if (!SM)
    return;

  auto LocateInRange = [this](const SourceLocation &Loc, const auto &Range) {
    return SM->isBeforeInTranslationUnit(Range.getBegin(), Loc) &&
           SM->isBeforeInTranslationUnit(Loc, Range.getEnd());
  };
  for (const auto &MacroDecl : MacroDecls) {
    for (auto &Function : Functions) {
      if (LocateInRange(MacroDecl.Loc, Function.Range))
        Function.MacroDecls.push_back(&MacroDecl);
    }
  }

  for (auto &Function : Functions) {
    std::vector<const MacroDecl *> Pairing;
    for (const auto *Decl : Function.MacroDecls) {
      if (Decl->Op == MacroDecl::Type::Define) {
        Pairing.push_back(Decl);
        continue;
      }
      auto Matched =
          std::find_if(Pairing.rbegin(), Pairing.rend(), [&](const auto *P) {
            return P->Op == MacroDecl::Type::Define && P->Name == Decl->Name;
          });
      if (Matched != Pairing.rend()) {
        Pairing.erase(std::next(Matched).base());
      } else {
        Pairing.push_back(Decl);
      }
    }
    for (const auto *Decl : Pairing) {
      diag(Decl->Loc, "禁止函数内的 #define 和 #undef 不配对");
    }
  }
}
} // namespace clang::tidy::gjb8114