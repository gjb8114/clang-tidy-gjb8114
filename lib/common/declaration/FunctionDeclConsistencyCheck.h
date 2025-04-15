#pragma once
#include "clang-tidy/ClangTidyCheck.h"

#include <map>
#include <string>
#include <vector>

namespace clang::tidy::gjb8114 {

class FunctionDeclConsistencyCheck : public ClangTidyCheck {
public:
  FunctionDeclConsistencyCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}
  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;

protected:
  enum class Report {
    ReturnType,
    NumParameter,
    ParameterType,
    ParameterName,
  };
  virtual std::set<enum Report> getReportOpt() const {
    return {Report::ReturnType, Report::NumParameter, Report::ParameterType,
            Report::ParameterName};
  }
  virtual const char *getDescription() const {
    return "禁止函数声明与定义不一致";
  }

private:
  std::map<std::string, std::vector<const FunctionDecl *>> Declarations;
  void checkDeclsByDefine(const ASTContext &Context, const FunctionDecl *Def);
  bool diagDeclConsistent(const ASTContext &Context, const FunctionDecl *Decl,
                          const FunctionDecl *Def);
};

} // namespace clang::tidy::gjb8114