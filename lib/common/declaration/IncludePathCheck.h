#pragma once
#include "clang-tidy/ClangTidyCheck.h"
#include "clang/Lex/PPCallbacks.h"
#include "clang/Lex/Preprocessor.h"

namespace clang::tidy::gjb8114 {

using llvm::StringRef;

class IncludePathCheck : public ClangTidyCheck {
public:
  IncludePathCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}

  void registerPPCallbacks(const SourceManager &SM, Preprocessor *PP,
                           Preprocessor *ModuleExpanderPP) override;

private:
  class IncludePathPPCallbacks : public clang::PPCallbacks {
  public:
    IncludePathPPCallbacks(IncludePathCheck &Check);

    void InclusionDirective(SourceLocation HashLoc, const Token &IncludeTok,
                            StringRef FileName, bool IsAngled,
                            CharSourceRange FilenameRange,
                            OptionalFileEntryRef File, StringRef SearchPath,
                            StringRef RelativePath,
                            const Module *SuggestedModule, bool ModuleImported,
                            SrcMgr::CharacteristicKind FileType) override;

  private:
    IncludePathCheck &Check;
  };
};

} // namespace clang::tidy::gjb8114