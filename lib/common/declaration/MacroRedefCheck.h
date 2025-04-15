#pragma once
#include "clang-tidy/ClangTidy.h"
#include "clang-tidy/ClangTidyCheck.h"
#include "clang/Lex/PPCallbacks.h"
#include "clang/Lex/Preprocessor.h"
#include <unordered_map>

namespace clang::tidy::gjb8114 {
using llvm::StringRef;

class MacroRedefCheck : public ClangTidyCheck {
public:
  MacroRedefCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}

  void registerPPCallbacks(const SourceManager &SM, Preprocessor *PP,
                           Preprocessor *ModuleExpanderPP) override;
};

class MacroRedefPPCallback : public PPCallbacks {
  MacroRedefCheck &Check;
  Preprocessor &PP;
  std::unordered_map<std::string, SourceLocation> MacroDefs;
  std::unordered_map<std::string, bool> MacroUndefs;

public:
  MacroRedefPPCallback(MacroRedefCheck &Check, Preprocessor &PP)
      : Check(Check), PP(PP) {}

  void MacroDefined(const Token &MacroNameTok,
                    const MacroDirective *MD) override;
  void MacroUndefined(const Token &MacroNameTok, const MacroDefinition &MD,
                      const MacroDirective *Undef) override;
};

} // namespace clang::tidy::gjb8114