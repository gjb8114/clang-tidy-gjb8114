#include "FreeDeallocCheck.h"
#include "utils/CFGVariableUseTracker.h"

using namespace clang::ast_matchers;

namespace clang::tidy::gjb8114 {
class FreePointerMutateTracker : public utils::CFGVariableUseTracker {
public:
  explicit FreePointerMutateTracker(ASTContext *Context)
      : CFGVariableUseTracker(Context) {}

protected:
  ast_matchers::internal::BindableMatcher<Stmt>
  useMatcher(const StatementMatcher &DeclRefMatcher) override {
    auto OpAssignMatcher =
        binaryOperator(anyOf(hasOperatorName("+="), hasOperatorName("-=")),
                       hasLHS(ignoringParenImpCasts(DeclRefMatcher)),
                       hasRHS(ignoringParenImpCasts(integerLiteral())));
    auto UnaryOpMatcher =
        unaryOperator(anyOf(hasOperatorName("++"), hasOperatorName("--")),
                      hasUnaryOperand(DeclRefMatcher));
    return expr(anyOf(OpAssignMatcher, UnaryOpMatcher));
  }
};

void FreeDeallocCheck::registerMatchers(MatchFinder *Finder) {
  auto PointerVarDecl =
      varDecl(hasType(isAnyPointer()),
              hasAncestor(functionDecl(unless(isExpansionInSystemHeader()),
                                       isDefinition())))
          .bind("ptrVar");
  auto PointerDeclRef = declRefExpr(to(PointerVarDecl));
  Finder->addMatcher(
      callExpr(callee(functionDecl(hasName("free"))),
               hasArgument(0, PointerDeclRef.bind("freeArg")),
               hasAncestor(functionDecl(unless(isExpansionInSystemHeader()),
                                        isDefinition())
                               .bind("func")))
          .bind("free"),
      this);
}

void FreeDeallocCheck::check(const MatchFinder::MatchResult &Result) {
  const auto *FreeCall = Result.Nodes.getNodeAs<CallExpr>("free");
  const auto *FreeArg = Result.Nodes.getNodeAs<Expr>("freeArg");
  if (!FreeCall || !FreeArg)
    return;
  const auto *Ptr = Result.Nodes.getNodeAs<VarDecl>("ptrVar");
  FreePointerMutateTracker Tracker{Result.Context};
  llvm::SmallPtrSet<const Stmt *, 1> MutateStmts;
  const auto *Func = Result.Nodes.getNodeAs<FunctionDecl>("func");
  Tracker.findUseBefore(MutateStmts, Func->getBody(), FreeCall, Ptr);
  if (!MutateStmts.empty()) {
    diag(FreeCall->getArg(0)->getBeginLoc(), "禁止释放非最初分配地址的指针");
  }
}
} // namespace clang::tidy::gjb8114