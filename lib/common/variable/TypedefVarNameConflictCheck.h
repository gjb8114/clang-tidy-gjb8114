#pragma once
#include "clang-tidy/ClangTidyCheck.h"
#include <map>
#include <string>

namespace clang::tidy::gjb8114 {
class TypedefVarNameConflictCheck : public ClangTidyCheck {
public:
  TypedefVarNameConflictCheck(llvm::StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}
  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;

private:
  std::map<std::string, const TypedefNameDecl *> DeclaredTypedefs;
};
} // namespace clang::tidy::gjb8114