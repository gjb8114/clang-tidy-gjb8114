#pragma once

#include "clang-tidy/ClangTidyCheck.h"
#include <clang/Lex/Preprocessor.h>

namespace clang::tidy::gjb8114 {
class NestedCommentsCheck : public ClangTidyCheck {
public:
  NestedCommentsCheck(llvm::StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context), PP(nullptr) {}

  void registerPPCallbacks(const SourceManager &SM, Preprocessor *PP,
                           Preprocessor *ModuleExpanderPP) override;

  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;

private:
  Preprocessor *PP;

  bool isInsideStringLiteral(const clang::StringRef &Buffer,
                             const clang::SourceLocation &FileLoc,
                             unsigned Offset);
};
} // namespace clang::tidy::gjb8114