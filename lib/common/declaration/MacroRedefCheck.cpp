#include "MacroRedefCheck.h"

namespace clang::tidy::gjb8114 {

void MacroRedefCheck::registerPPCallbacks(const SourceManager &SM,
                                          Preprocessor *PP,
                                          Preprocessor *ModuleExpanderPP) {
  PP->addPPCallbacks(std::make_unique<MacroRedefPPCallback>(*this, *PP));
}

void MacroRedefPPCallback::MacroDefined(const Token &MacroNameTok,
                                        const MacroDirective *MD) {
  auto Loc = MacroNameTok.getLocation();
  if (Loc.isInvalid() || PP.getSourceManager().isInSystemHeader(Loc))
    return;

  StringRef MacroName = MacroNameTok.getIdentifierInfo()->getName();
  std::string MacroStr = MacroName.str();

  if (auto It = MacroDefs.find(MacroStr);
      It != MacroDefs.end() && !MacroUndefs[MacroStr]) {
    Check.diag(Loc, "禁止重复定义宏");
  }

  MacroDefs[MacroStr] = Loc;
  MacroUndefs[MacroStr] = false;
}

void MacroRedefPPCallback::MacroUndefined(const Token &MacroNameTok,
                                          const MacroDefinition &MD,
                                          const MacroDirective *Undef) {
  StringRef MacroName = MacroNameTok.getIdentifierInfo()->getName();
  MacroUndefs[MacroName.str()] = true;
}
} // namespace clang::tidy::gjb8114