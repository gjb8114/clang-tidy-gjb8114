#pragma once
#include "clang-tidy/ClangTidyCheck.h"
#include "clang/Lex/Preprocessor.h"
#include "llvm/ADT/DenseMap.h"
#include "llvm/ADT/DenseSet.h"

namespace clang::tidy::gjb8114 {

using llvm::StringRef;

class DuplicateIncludeCheck : public ClangTidyCheck {
public:
  DuplicateIncludeCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}

  void registerPPCallbacks(const SourceManager &SM, Preprocessor *PP,
                           Preprocessor *ModuleExpanderPP) override;

private:
  class IncludeTracker : public clang::PPCallbacks {
  public:
    IncludeTracker(DuplicateIncludeCheck &Check, const SourceManager &SM,
                   Preprocessor &PP);

    void InclusionDirective(SourceLocation HashLoc, const Token &IncludeTok,
                            StringRef FileName, bool IsAngled,
                            CharSourceRange FilenameRange,
                            OptionalFileEntryRef File, StringRef SearchPath,
                            StringRef RelativePath,
                            const Module *SuggestedModule, bool ModuleImported,
                            SrcMgr::CharacteristicKind FileType) override;

    void FileChanged(SourceLocation Loc, FileChangeReason Reason,
                     SrcMgr::CharacteristicKind FileType,
                     FileID PrevFID = FileID()) override;

  private:
    DuplicateIncludeCheck &Check;
    const SourceManager &SM;
    Preprocessor &PP;

    llvm::DenseMap<FileID, llvm::DenseSet<const FileEntry *>> FileIncludes;
    llvm::DenseSet<const FileEntry *> MainFileIncludes;
    std::vector<FileID> FileStack;

    void emitDiagnosticWithFixIt(SourceLocation HashLoc,
                                 CharSourceRange FilenameRange,
                                 DiagnosticBuilder &Diag);
  };
};

} // namespace clang::tidy::gjb8114