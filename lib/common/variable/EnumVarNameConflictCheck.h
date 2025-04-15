#pragma once
#include "clang-tidy/ClangTidyCheck.h"
#include <string>
#include <unordered_map>

namespace clang::tidy::gjb8114 {
class EnumVarNameConflictCheck : public ClangTidyCheck {
public:
  EnumVarNameConflictCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}
  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;

private:
  std::unordered_map<std::string, const EnumConstantDecl *>
      DeclaredEnumElements;
};
} // namespace clang::tidy::gjb8114