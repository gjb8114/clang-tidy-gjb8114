#pragma once
#include "CFGVariableUseTracker.h"
#include "matchers/compose.h"

namespace clang::tidy::gjb8114::utils {
class RawPointerDeallocTracker : public CFGVariableUseTracker {
public:
  explicit RawPointerDeallocTracker(ASTContext *Context)
      : CFGVariableUseTracker(Context) {}

protected:
  ast_matchers::internal::BindableMatcher<Stmt> useMatcher(
      const ast_matchers::StatementMatcher &DeclRefMatcher) override {
    using namespace ast_matchers;
    return stmt(
        anyOf(callExpr(callee(functionDecl(hasName("free"))),
                       hasArgument(0, DeclRefMatcher)),
              cxxDeleteExpr(has(ignoringParenImpCasts(DeclRefMatcher)))));
  }
  ast_matchers::internal::BindableMatcher<Stmt> reinitMatcher(
      const ast_matchers::StatementMatcher &DeclRefMatcher) override {
    using namespace ast_matchers;
    return binaryOperator(hasOperatorName("="), hasLHS(DeclRefMatcher),
                          hasRHS(unless(gjb8114::isNull())));
  }
};

} // namespace clang::tidy::gjb8114::utils