#include "MacroParenthesesCheck.h"
#include "clang/Frontend/FrontendActions.h"
#include "clang/Lex/Lexer.h"

#include <optional>

using namespace clang::ast_matchers;

namespace clang::tidy::gjb8114 {
namespace {
class MacroParenthesesPPCallback : public PPCallbacks {
public:
  MacroParenthesesPPCallback(MacroParenthesesCheck *Check,
                             const SourceManager &SM)
      : Check(Check), SM(SM) {}

  void MacroDefined(const Token &MacroNameTok,
                    const MacroDirective *MD) override {
    const auto *MI = MD->getMacroInfo();
    if (!MI->isFunctionLike())
      return;

    auto Loc = MacroNameTok.getLocation();
    if (Loc.isInvalid() || SM.isInSystemHeader(Loc))
      return;

    if (MI->getNumTokens() <= 0)
      return;

    auto DiagLoc = checkOuterParens(MI);
    if (!DiagLoc)
      DiagLoc = checkParamsParens(MI);
    if (DiagLoc)
      Check->diag(*DiagLoc, "禁止宏函数的参数和结果未加括号");
  }

  static bool isParens(const MacroInfo *MI, unsigned LHS, unsigned RHS) {
    return MI->getReplacementToken(LHS).is(tok::l_paren) &&
           MI->getReplacementToken(RHS).is(tok::r_paren);
  }

  static bool isParamWrappedInParens(const MacroInfo *MI, unsigned ParamPos) {
    if (ParamPos == 0 || ParamPos == MI->getNumTokens() - 1)
      return false;
    return isParens(MI, ParamPos - 1, ParamPos + 1);
  }

  static std::optional<SourceLocation> checkOuterParens(const MacroInfo *MI) {
    if (MI->getNumTokens() >= 3) { // At minimum, we need (, something, )
      if (!isParens(MI, 0, MI->getNumTokens() - 1))
        return MI->getReplacementToken(0).getLocation();
    }
    return std::nullopt;
  }

  std::optional<SourceLocation> checkParamsParens(const MacroInfo *MI) const {
    llvm::SmallVector<const IdentifierInfo *, 8> Params;
    std::generate_n(std::back_inserter(Params), MI->getNumParams(),
                    [MI, I = 0]() mutable { return MI->params()[I++]; });

    for (unsigned I = 0; I < MI->getNumTokens(); ++I) {
      const auto &Tok = MI->getReplacementToken(I);
      if (!Tok.is(tok::identifier))
        continue;
      const IdentifierInfo *II = Tok.getIdentifierInfo();
      if (std::find(Params.begin(), Params.end(), II) == Params.end())
        continue;
      if (!isParamWrappedInParens(MI, I))
        return Tok.getLocation();
    }
    return std::nullopt;
  }

private:
  MacroParenthesesCheck *Check;
  const SourceManager &SM;
};
} // namespace

MacroParenthesesCheck::MacroParenthesesCheck(StringRef Name,
                                             ClangTidyContext *Context)
    : ClangTidyCheck(Name, Context) {}

void MacroParenthesesCheck::registerPPCallbacks(
    const SourceManager &SM, Preprocessor *PP, Preprocessor *ModuleExpanderPP) {
  PP->addPPCallbacks(std::make_unique<MacroParenthesesPPCallback>(this, SM));
}

void MacroParenthesesCheck::storeOptions(ClangTidyOptions::OptionMap &Opts) {}
} // namespace clang::tidy::gjb8114