#pragma once
#include "clang-tidy/ClangTidy.h"
#include "clang-tidy/ClangTidyCheck.h"
#include "clang/Lex/PPCallbacks.h"
#include "clang/Lex/Preprocessor.h"

namespace clang::tidy::gjb8114 {
using llvm::StringRef;

class MacroReplacementCheck : public ClangTidyCheck {
public:
  MacroReplacementCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}

  void registerPPCallbacks(const SourceManager &SM, Preprocessor *PP,
                           Preprocessor *ModuleExpanderPP) override;
};

class MacroReplacementPPCallback : public PPCallbacks {
  MacroReplacementCheck &Check;
  Preprocessor &PP;

public:
  MacroReplacementPPCallback(MacroReplacementCheck &Check, Preprocessor &PP)
      : Check(Check), PP(PP) {}

  void MacroDefined(const Token &MacroNameTok,
                    const MacroDirective *MD) override;

private:
  bool isKeywordOrBasicType(const MacroDirective *MD);
};
} // namespace clang::tidy::gjb8114