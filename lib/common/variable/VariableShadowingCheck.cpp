#include "VariableShadowingCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/Basic/SourceManager.h"
#include <ranges>
#include <unordered_map>
#include <vector>

using namespace clang::ast_matchers;

namespace clang::tidy::gjb8114 {

namespace {
class ShadowingVisitor : public RecursiveASTVisitor<ShadowingVisitor> {
public:
  ShadowingVisitor(ClangTidyCheck &Check) : Check(Check) {}

  bool VisitFunctionDecl(FunctionDecl *FD) {
    VarScopes.clear();
    return true;
  }

  bool TraverseCompoundStmt(CompoundStmt *CS) {
    if (!CS)
      return true;

    VarScopes.push_back({});

    bool Result =
        RecursiveASTVisitor<ShadowingVisitor>::TraverseCompoundStmt(CS);

    if (!VarScopes.empty())
      VarScopes.pop_back();

    return Result;
  }

  bool VisitDeclStmt(DeclStmt *DS) {
    if (!DS || VarScopes.empty())
      return true;

    auto IsNamedVar = [](Decl *D) {
      if (auto *VD = dyn_cast<VarDecl>(D))
        return !VD->getName().empty() && !VD->isParameterPack() &&
               VD->getKind() != Decl::ParmVar;
      return false;
    };
    for (Decl *D : DS->decls() | std::views::filter(IsNamedVar)) {
      VarDecl *VD = dyn_cast<VarDecl>(D);
      auto Name = VD->getName().str();

      if (const auto *Shadowed = findShadowedVariable(Name)) {
        Check.diag(VD->getLocation(), "禁止变量遮蔽");
        Check.diag(Shadowed->getLocation(), "变量首次定义在此处",
                   clang::DiagnosticIDs::Note);
      }

      if (!VarScopes.empty())
        VarScopes.back()[Name] = VD;
    }
    return true;
  }

  VarDecl *findShadowedVariable(const std::string &Name) const {
    for (auto &Scope : VarScopes | std::views::take(VarScopes.size() - 1)) {
      auto It = Scope.find(Name);
      if (It != Scope.end())
        return It->second;
    }
    return nullptr;
  }

private:
  ClangTidyCheck &Check;
  std::vector<std::unordered_map<std::string, VarDecl *>> VarScopes;
};
} // namespace

void VariableShadowingCheck::registerMatchers(MatchFinder *Finder) {
  Finder->addMatcher(functionDecl(unless(isImplicit()),
                                  unless(isExpansionInSystemHeader()),
                                  has(compoundStmt()))
                         .bind("function"),
                     this);
}

void VariableShadowingCheck::check(const MatchFinder::MatchResult &Result) {
  const auto *FD = Result.Nodes.getNodeAs<FunctionDecl>("function");
  if (!FD || !FD->hasBody())
    return;

  ShadowingVisitor Visitor(*this);
  Visitor.TraverseFunctionDecl(const_cast<FunctionDecl *>(FD));
}
} // namespace clang::tidy::gjb8114