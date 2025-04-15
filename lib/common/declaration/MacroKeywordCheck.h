#pragma once
#include "clang-tidy/ClangTidy.h"
#include "clang-tidy/ClangTidyCheck.h"
#include "clang/Lex/PPCallbacks.h"
#include "clang/Lex/Preprocessor.h"

namespace clang::tidy::gjb8114 {
using llvm::StringRef;

class MacroKeywordCheck : public ClangTidyCheck {
public:
  MacroKeywordCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}

  void registerPPCallbacks(const SourceManager &SM, Preprocessor *PP,
                           Preprocessor *ModuleExpanderPP) override;
};

class MacroKeywordPPCallback : public PPCallbacks {
  MacroKeywordCheck &Check;
  Preprocessor &PP;

public:
  MacroKeywordPPCallback(MacroKeywordCheck &Check, Preprocessor &PP)
      : Check(Check), PP(PP) {}

  void MacroDefined(const Token &MacroNameTok,
                    const MacroDirective *MD) override;
};

} // namespace clang::tidy::gjb8114