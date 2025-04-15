#pragma once
#include "clang-tidy/ClangTidy.h"
#include "clang-tidy/ClangTidyCheck.h"
#include "clang/AST/Decl.h"
#include "clang/AST/Expr.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/ASTMatchers/ASTMatchers.h"

namespace clang::tidy::gjb8114 {
using llvm::StringRef;

class SystemInitializedVariableCheck : public ClangTidyCheck {
public:
  SystemInitializedVariableCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}

  void registerMatchers(ast_matchers::MatchFinder *Finder) override {
    using namespace ast_matchers;
    Finder->addMatcher(
        varDecl(unless(hasInitializer(anything()))).bind("uninitializedVar"),
        this);
  }

  void check(const ast_matchers::MatchFinder::MatchResult &Result) override {
    const auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("uninitializedVar");
    if (!VD) {
      return;
    }

    auto Loc = VD->getLocation();
    if (!Loc.isValid() || Result.SourceManager->isInSystemHeader(Loc)) {
      return;
    }

    if (VD->hasGlobalStorage()) {
      diag(VD->getLocation(), "禁止变量缺省初始化");
      diag(VD->getLocation(), "变量 '%0' 初始化在此处", DiagnosticIDs::Note)
          << VD->getName();
    }
  }
};
} // namespace clang::tidy::gjb8114