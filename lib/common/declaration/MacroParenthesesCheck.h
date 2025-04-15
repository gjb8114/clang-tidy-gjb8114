#pragma once
#include "clang-tidy/ClangTidyCheck.h"
#include "clang/Frontend/FrontendActions.h"
#include "clang/Lex/PPCallbacks.h"
#include "clang/Lex/Preprocessor.h"
#include "clang/Tooling/Tooling.h"

namespace clang::tidy::gjb8114 {
class MacroParenthesesCheck : public ClangTidyCheck {
public:
  MacroParenthesesCheck(StringRef Name, ClangTidyContext *Context);
  void registerPPCallbacks(const SourceManager &SM, Preprocessor *PP,
                           Preprocessor *ModuleExpanderPP) override;
  void storeOptions(ClangTidyOptions::OptionMap &Opts) override;
};
} // namespace clang::tidy::gjb8114