#pragma once
#include "clang-tidy/ClangTidyCheck.h"

namespace clang::tidy::gjb8114 {

using llvm::StringRef;

class MallocNullCheck : public ClangTidyCheck {
public:
  MallocNullCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}
  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
  void onEndOfTranslationUnit() override;

private:
  void doCheck();

  SourceManager* SM;
  using VarDeclPtr = const clang::VarDecl *;
  std::unordered_map<VarDeclPtr, std::vector<const clang::Stmt *>> PointerOps;
  std::unordered_map<VarDeclPtr, bool> PointerAllocates;
};
} // namespace clang::tidy::gjb8114