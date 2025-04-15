#pragma once
#include "CFGVariableUseTracker.h"
#include "matchers/compose.h"

namespace clang::tidy::gjb8114::utils {
class RawPointerUseTracker : public CFGVariableUseTracker {
public:
  explicit RawPointerUseTracker(ASTContext *Context)
      : CFGVariableUseTracker(Context) {}

protected:
  ast_matchers::internal::BindableMatcher<Stmt> useMatcher(
      const ast_matchers::StatementMatcher &DeclRefMatcher) override {
    using namespace ast_matchers;
    return stmt(anyOf(matchers::derefPointerExpr(DeclRefMatcher),
                      matchers::subscriptPointerExpr(DeclRefMatcher)));
  }
  ast_matchers::internal::BindableMatcher<Stmt> reinitMatcher(
      const ast_matchers::StatementMatcher &DeclRefMatcher) override {
    using namespace ast_matchers;
    return binaryOperator(hasOperatorName("="), hasLHS(DeclRefMatcher),
                          hasRHS(unless(gjb8114::isNull())));
  }
};
} // namespace clang::tidy::gjb8114::utils