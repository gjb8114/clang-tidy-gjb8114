#include "MacroKeywordCheck.h"
#include "clang/Basic/TokenKinds.h"
#include "clang/Lex/Token.h"

namespace clang::tidy::gjb8114 {

void MacroKeywordCheck::registerPPCallbacks(const SourceManager &SM,
                                            Preprocessor *PP,
                                            Preprocessor *ModuleExpanderPP) {
  PP->addPPCallbacks(std::make_unique<MacroKeywordPPCallback>(*this, *PP));
}

void MacroKeywordPPCallback::MacroDefined(const Token &MacroNameTok,
                                          const MacroDirective *MD) {
  if (PP.getSourceManager().isInSystemHeader(MacroNameTok.getLocation()))
    return;

  StringRef MacroName = MacroNameTok.getIdentifierInfo()->getName();
  tok::TokenKind Kind = PP.getIdentifierInfo(MacroName)->getTokenID();
  if (Kind != tok::identifier) {
    Check.diag(MacroNameTok.getLocation(), "禁止宏定义覆盖关键字");
  }
}
} // namespace clang::tidy::gjb8114