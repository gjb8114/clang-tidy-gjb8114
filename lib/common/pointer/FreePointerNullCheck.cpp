#include "FreePointerNullCheck.h"
#include "matchers/matcher.h"
#include "utils/CFGVariableUseTracker.h"

using namespace clang::ast_matchers;

namespace clang::tidy::gjb8114 {
class PointerSetNullTracker : public utils::CFGVariableUseTracker {
public:
  explicit PointerSetNullTracker(ASTContext *Context)
      : CFGVariableUseTracker(Context) {}

protected:
  ast_matchers::internal::BindableMatcher<Stmt> useMatcher(
      const ast_matchers::StatementMatcher &DeclRefMatcher) override {
    return binaryOperator(hasOperatorName("="), hasLHS(DeclRefMatcher),
                          hasRHS(gjb8114::isNull()));
  }
  ast_matchers::internal::BindableMatcher<Stmt> reinitMatcher(
      const ast_matchers::StatementMatcher &DeclRefMatcher) override {
    return stmt(unless(stmt()));
  }
};

void FreePointerNullCheck::registerMatchers(MatchFinder *Finder) {
  auto DeclRef = declRefExpr(to(varDecl().bind("ptr")));
  auto FuncDecl =
      functionDecl(unless(isExpansionInSystemHeader()), isDefinition())
          .bind("func");
  Finder->addMatcher(
      stmt(anyOf(callExpr(callee(functionDecl(hasName("free"))),
                          hasArgument(0, DeclRef), hasAncestor(FuncDecl))
                     .bind("free"),
                 cxxDeleteExpr(has(ignoringParenImpCasts(DeclRef)),
                               hasAncestor(FuncDecl))
                     .bind("delete")))
          .bind("dealloc"),
      this);
}

void FreePointerNullCheck::check(const MatchFinder::MatchResult &Result) {
  const Stmt *Free = Result.Nodes.getNodeAs<Stmt>("free");
  const Stmt *Delete = Result.Nodes.getNodeAs<Stmt>("delete");
  auto *Func = Result.Nodes.getNodeAs<FunctionDecl>("func");
  auto *VD = Result.Nodes.getNodeAs<VarDecl>("ptr");
  for (auto *Trigger : {Free, Delete}) {
    if (!Trigger)
      continue;
    llvm::SmallPtrSet<const Stmt *, 1> SetNulls;
    PointerSetNullTracker Tracker(Result.Context);
    Tracker.findUseAfter(SetNulls, Func->getBody(), Trigger, VD);
    if (SetNulls.empty())
      diag(Trigger->getBeginLoc(), "禁止释放指针变量后未置空");
  }
}
} // namespace clang::tidy::gjb8114