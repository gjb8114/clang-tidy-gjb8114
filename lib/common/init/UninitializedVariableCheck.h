#pragma once
#include "clang-tidy/ClangTidy.h"
#include "clang-tidy/ClangTidyCheck.h"
#include "clang/AST/Decl.h"
#include "clang/AST/Expr.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/ASTMatchers/ASTMatchers.h"

#include <set>

namespace clang::tidy::gjb8114 {
using llvm::StringRef;

class UninitializedVariableCheck : public ClangTidyCheck {
public:
  UninitializedVariableCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}

  void registerMatchers(ast_matchers::MatchFinder *Finder) override {
    using namespace ast_matchers;
    Finder->addMatcher(
        varDecl(unless(hasInitializer(anything()))).bind("uninitializedVar"),
        this);
    Finder->addMatcher(
        declRefExpr(to(varDecl().bind("usedVar"))).bind("varRef"), this);
    Finder->addMatcher(binaryOperator(hasOperatorName("="),
                                      hasLHS(ignoringParenImpCasts(declRefExpr(
                                          to(varDecl().bind("assignedVar"))))))
                           .bind("assignExpr"),
                       this);
  }

  void check(const ast_matchers::MatchFinder::MatchResult &Result) override {
    const auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("uninitializedVar");
    if (VD && !VD->hasGlobalStorage()) {
      auto Loc = VD->getLocation();
      if (Loc.isValid() && !Result.SourceManager->isInSystemHeader(Loc)) {
        if (!InitializedVars.count(VD)) {
          UninitializedVars.insert(VD);
        }
      }
    }

    const auto *ID = Result.Nodes.getNodeAs<clang::VarDecl>("assignedVar");
    if (ID) {
      auto Loc = ID->getLocation();
      if (Loc.isValid() && !Result.SourceManager->isInSystemHeader(Loc)) {
        InitializedVars.insert(ID);
        UninitializedVars.erase(ID);
      }
    }

    const auto *UsedVar = Result.Nodes.getNodeAs<clang::VarDecl>("usedVar");
    if (!UsedVar) {
      return;
    }

    const auto *VarRef = Result.Nodes.getNodeAs<clang::DeclRefExpr>("varRef");
    if (!VarRef) {
      return;
    }

    if (UninitializedVars.count(UsedVar)) {
      diag(VarRef->getLocation(), "禁止使用未初始化变量");
      diag(UsedVar->getLocation(), "变量 '%0' 的使用在此处",
           DiagnosticIDs::Note)
          << UsedVar->getName();
    }
  }

private:
  std::set<const clang::VarDecl *> UninitializedVars;
  std::set<const clang::VarDecl *> InitializedVars;
};
} // namespace clang::tidy::gjb8114
