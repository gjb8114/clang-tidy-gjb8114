#include "MacroReplacementCheck.h"
#include "clang/Lex/Token.h"

namespace clang::tidy::gjb8114 {

void MacroReplacementCheck::registerPPCallbacks(
    const SourceManager &SM, Preprocessor *PP, Preprocessor *ModuleExpanderPP) {
  PP->addPPCallbacks(std::make_unique<MacroReplacementPPCallback>(*this, *PP));
}

void MacroReplacementPPCallback::MacroDefined(const Token &MacroNameTok,
                                              const MacroDirective *MD) {
  auto Loc = MacroNameTok.getLocation();
  if (Loc.isInvalid() || PP.getSourceManager().isInSystemHeader(Loc))
    return;

  if (!MD->getMacroInfo()->isObjectLike())
    return;

  if (isKeywordOrBasicType(MD))
    Check.diag(MD->getLocation(), "禁止宏定义到关键字");
}

bool MacroReplacementPPCallback::isKeywordOrBasicType(
    const MacroDirective *MD) {
  const auto *Info = MD->getMacroInfo();
  if (Info->getNumTokens() != 1)
    return false;

  const Token &ReplacementToken = Info->getReplacementToken(0);
  if (ReplacementToken.isAnyIdentifier() || ReplacementToken.isLiteral())
    return false;

  return true;
}
} // namespace clang::tidy::gjb8114