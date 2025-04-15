#pragma once
#include "clang-tidy/ClangTidy.h"
#include "clang-tidy/ClangTidyCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/Lex/PPCallbacks.h"
#include "clang/Lex/Preprocessor.h"
#include <string>
#include <vector>

namespace clang::tidy::gjb8114 {
using llvm::StringRef;

class MacroPairingCheck : public ClangTidyCheck {
  class MacroPairingPPCallback : public PPCallbacks {
    using OnMacroDefinedFunc =
        std::function<void(const Token &, const MacroDirective *)>;
    using OnMacroUndefinedFunc = std::function<void(
        const Token &, const MacroDefinition &, const MacroDirective *)>;
    OnMacroDefinedFunc OnMacroDefined;
    OnMacroUndefinedFunc OnMacroUndefined;

  public:
    MacroPairingPPCallback(OnMacroDefinedFunc OnMacroDefined,
                           OnMacroUndefinedFunc OnMacroUndefined)
        : OnMacroDefined(OnMacroDefined), OnMacroUndefined(OnMacroUndefined) {}

    void MacroDefined(const Token &MacroNameTok,
                      const MacroDirective *MD) override {
      if (OnMacroDefined)
        OnMacroDefined(MacroNameTok, MD);
    }

    void MacroUndefined(const Token &MacroNameTok, const MacroDefinition &MD,
                        const MacroDirective *Undef) override {
      if (OnMacroUndefined)
        OnMacroUndefined(MacroNameTok, MD, Undef);
    }
  };

public:
  MacroPairingCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context), SM(nullptr) {}
  void registerPPCallbacks(const SourceManager &SM, Preprocessor *PP,
                           Preprocessor *ModuleExpanderPP) override;
  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
  void onEndOfTranslationUnit() override;

private:
  struct MacroDecl {
    std::string Name;
    SourceLocation Loc;
    enum class Type { Define, Undef } Op;
  };

  struct FunctionMacros {
    std::string Name;
    SourceRange Range;
    std::vector<const MacroDecl *> MacroDecls;
  };

  std::vector<MacroDecl> MacroDecls;
  std::vector<FunctionMacros> Functions;
  const SourceManager *SM;
};
} // namespace clang::tidy::gjb8114