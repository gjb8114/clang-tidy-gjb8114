#include "IncludePathCheck.h"
#include "llvm/Support/Path.h"

namespace clang::tidy::gjb8114 {

void IncludePathCheck::registerPPCallbacks(const SourceManager &SM,
                                           Preprocessor *PP,
                                           Preprocessor *ModuleExpanderPP) {
  PP->addPPCallbacks(std::make_unique<IncludePathPPCallbacks>(*this));
}

IncludePathCheck::IncludePathPPCallbacks::IncludePathPPCallbacks(
    IncludePathCheck &Check)
    : Check(Check) {}

void IncludePathCheck::IncludePathPPCallbacks::InclusionDirective(
    SourceLocation HashLoc, const Token &IncludeTok, StringRef FileName,
    bool IsAngled, CharSourceRange FilenameRange, OptionalFileEntryRef File,
    StringRef SearchPath, StringRef RelativePath, const Module *SuggestedModule,
    bool ModuleImported, SrcMgr::CharacteristicKind FileType) {

  if (IsAngled)
    return;

  using llvm::sys::path::is_absolute;
  using llvm::sys::path::Style;
  if (is_absolute(FileName, Style::windows_slash) ||
      is_absolute(FileName, Style::windows_backslash) ||
      is_absolute(FileName, Style::posix)) {
    Check.diag(FilenameRange.getBegin(), "禁止 #include 绝对路径")
        << FilenameRange;
  }
}
} // namespace clang::tidy::gjb8114