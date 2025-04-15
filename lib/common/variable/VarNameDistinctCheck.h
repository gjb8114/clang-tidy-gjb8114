#pragma once
#include "clang-tidy/ClangTidyCheck.h"
#include <string>
#include <unordered_map>

namespace clang::tidy::gjb8114 {
class VarNameDistinctCheck : public ClangTidyCheck {
public:
  VarNameDistinctCheck(llvm::StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}
  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;

protected:
  virtual std::string getKey(const std::string &Name) const = 0;
  virtual const char *getDescription() const = 0;

private:
  std::unordered_map<std::string, const VarDecl *> DeclaredVars;
  std::unordered_map<std::string, const VarDecl *> LocalVars;
  const DeclContext *CurrentDeclContext = nullptr;
};
} // namespace clang::tidy::gjb8114
