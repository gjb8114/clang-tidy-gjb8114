#include "NullTermStringCheck.h"
#include "utils/CFGVariableUseTracker.h"

using namespace clang::ast_matchers;

namespace clang::tidy::gjb8114 {
class NullTerminatorTracker : public utils::CFGVariableUseTracker {
public:
  explicit NullTerminatorTracker(ASTContext *Context)
      : CFGVariableUseTracker(Context) {}

protected:
  ast_matchers::internal::BindableMatcher<Stmt>
  useMatcher(const StatementMatcher &DeclRefMatcher) override {
    auto AssignNullTerminatorMatcher = binaryOperator(
        isAssignmentOperator(),
        hasLHS(
            arraySubscriptExpr(hasBase(ignoringParenImpCasts(DeclRefMatcher)))),
        hasRHS(ignoringParenImpCasts(characterLiteral(equals('\0')))));
    auto MemsetMatcher =
        callExpr(callee(functionDecl(hasName("::memset"))),
                 hasArgument(0, ignoringParenImpCasts(DeclRefMatcher)));
    return expr(anyOf(AssignNullTerminatorMatcher, MemsetMatcher));
  }
};

void NullTermStringCheck::registerMatchers(MatchFinder *Finder) {
  Finder->addMatcher(
      declRefExpr(
          unless(isExpansionInSystemHeader()),
          to(varDecl(hasType(arrayType(hasElementType(isAnyCharacter()))),
                     // = {0}
                     unless(hasInitializer(initListExpr(hasInit(
                         0, ignoringImpCasts(integerLiteral(equals(0))))))))
                 .bind("charArray")),
          unless(hasAncestor(sizeOfExpr(expr()))),
          // Following func must take the array as a null-str
          hasAncestor(
              callExpr(callee(functionDecl(hasAnyName(
                           "::printf", "::strcmp", "::strlen", "::strncmp",
                           "::fprintf", "::fputs", "::puts"))))
                  .bind("call")),
          hasAncestor(functionDecl(isDefinition()).bind("func")))
          .bind("arrayRef"),
      this);
}

void NullTermStringCheck::check(const MatchFinder::MatchResult &Result) {
  const auto *Func = Result.Nodes.getNodeAs<FunctionDecl>("func");
  const auto *VD = Result.Nodes.getNodeAs<VarDecl>("charArray");
  const auto *DeclRef = Result.Nodes.getNodeAs<DeclRefExpr>("arrayRef");
  const auto *Call = Result.Nodes.getNodeAs<CallExpr>("call");
  if (!Func || !VD || !DeclRef || !Call)
    return;

  const Stmt *FuncBody = Func->getBody();
  NullTerminatorTracker Tracker(Result.Context);
  llvm::SmallSet<const Stmt *, 1> NullTerminatorStmts;
  Tracker.findUseBefore(NullTerminatorStmts, const_cast<Stmt *>(FuncBody), Call,
                        VD);
  if (NullTerminatorStmts.empty())
    diag(DeclRef->getLocation(), "禁止字符串数组未以 '\\0' 结束");
}
} // namespace clang::tidy::gjb8114