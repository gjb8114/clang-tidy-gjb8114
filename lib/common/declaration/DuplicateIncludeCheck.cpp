#include "DuplicateIncludeCheck.h"

namespace clang::tidy::gjb8114 {

void DuplicateIncludeCheck::registerPPCallbacks(
    const SourceManager &SM, Preprocessor *PP, Preprocessor *ModuleExpanderPP) {
  PP->addPPCallbacks(
      std::make_unique<IncludeTracker>(*this, PP->getSourceManager(), *PP));
}

DuplicateIncludeCheck::IncludeTracker::IncludeTracker(
    DuplicateIncludeCheck &Check, const SourceManager &SM, Preprocessor &PP)
    : Check(Check), SM(SM), PP(PP) {}

void DuplicateIncludeCheck::IncludeTracker::FileChanged(
    SourceLocation Loc, FileChangeReason Reason,
    SrcMgr::CharacteristicKind FileType, FileID PrevFID) {

  if (Loc.isInvalid())
    return;

  FileID FID = SM.getFileID(Loc);

  if (Reason == PPCallbacks::EnterFile) {
    FileStack.push_back(FID);
  } else if (Reason == PPCallbacks::ExitFile && !FileStack.empty()) {
    FileStack.pop_back();
  }
}

void DuplicateIncludeCheck::IncludeTracker::InclusionDirective(
    SourceLocation HashLoc, const Token &IncludeTok, StringRef FileName,
    bool IsAngled, CharSourceRange FilenameRange, OptionalFileEntryRef File,
    StringRef SearchPath, StringRef RelativePath, const Module *SuggestedModule,
    bool ModuleImported, SrcMgr::CharacteristicKind FileType) {

  if (IsAngled || !File || FileStack.empty())
    return;

  const auto *IncludedFile = &File->getFileEntry();
  if (MainFileIncludes.count(IncludedFile) > 0) {
    auto Diag = Check.diag(FilenameRange.getBegin(), "禁止重复包含头文件")
                << FilenameRange;
    emitDiagnosticWithFixIt(HashLoc, FilenameRange, Diag);
  } else {
    MainFileIncludes.insert(IncludedFile);
  }
}

void DuplicateIncludeCheck::IncludeTracker::emitDiagnosticWithFixIt(
    SourceLocation HashLoc, CharSourceRange FilenameRange,
    DiagnosticBuilder &Diag) {
  FileID FID = SM.getFileID(HashLoc);
  unsigned Column = SM.getColumnNumber(FID, SM.getFileOffset(HashLoc));
  SourceLocation StartOfLine = HashLoc.getLocWithOffset(-Column + 1);
  SourceLocation EndOfLine = Lexer::findLocationAfterToken(
      FilenameRange.getEnd(), tok::greater, SM, PP.getLangOpts(), false);
  if (EndOfLine.isInvalid()) {
    EndOfLine = FilenameRange.getEnd();
  }

  EndOfLine = Lexer::getLocForEndOfToken(EndOfLine, 0, SM, PP.getLangOpts());
  bool Invalid = false;
  const char *EndOfLinePtr = SM.getCharacterData(EndOfLine, &Invalid);
  if (!Invalid && *EndOfLinePtr == '\n') {
    EndOfLine = EndOfLine.getLocWithOffset(1);
  }

  Diag << FixItHint::CreateRemoval(
      CharSourceRange::getCharRange(StartOfLine, EndOfLine));
}

} // namespace clang::tidy::gjb8114