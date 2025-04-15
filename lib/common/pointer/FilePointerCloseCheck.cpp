#include "FilePointerCloseCheck.h"
#include "utils/CFGVariableUseTracker.h"

using namespace clang::ast_matchers;

namespace clang::tidy::gjb8114 {
class FilePointerCloseTracker : public utils::CFGVariableUseTracker {
public:
  explicit FilePointerCloseTracker(ASTContext *Context)
      : CFGVariableUseTracker(Context) {}

protected:
  ast_matchers::internal::BindableMatcher<Stmt> useMatcher(
      const ast_matchers::StatementMatcher &DeclRefMatcher) override {
    return callExpr(callee(functionDecl(hasName("fclose"))),
                    hasArgument(0, DeclRefMatcher));
  }
  ast_matchers::internal::BindableMatcher<Stmt> reinitMatcher(
      const ast_matchers::StatementMatcher &DeclRefMatcher) override {
    return stmt(unless(stmt()));
  }
};

void FilePointerCloseCheck::registerMatchers(MatchFinder *Finder) {
  auto Func = functionDecl(unless(isExpansionInSystemHeader()), isDefinition())
                  .bind("func");
  Finder->addMatcher(
      declStmt(has(varDecl(hasType(pointerType(pointee(
                               hasDeclaration(namedDecl(hasName("FILE")))))),
                           hasDescendant(
                               callExpr(callee(functionDecl(hasName("fopen"))))
                                   .bind("fopen")),
                           unless(hasGlobalStorage()), hasAncestor(Func))
                       .bind("filePtr")))
          .bind("fopenDecl"),
      this);
  Finder->addMatcher(
      binaryOperator(hasOperatorName("="),
                     hasLHS(ignoringParenImpCasts(
                         declRefExpr(to(varDecl().bind("filePtr"))))),
                     hasRHS(callExpr(callee(functionDecl(hasName("fopen"))))),
                     hasAncestor(Func))
          .bind("fopenAssign"),
      this);
}

void FilePointerCloseCheck::check(const MatchFinder::MatchResult &Result) {
  const auto *Func = Result.Nodes.getNodeAs<FunctionDecl>("func");
  const Stmt *Assign = Result.Nodes.getNodeAs<BinaryOperator>("fopenAssign");
  const Stmt *Decl = Result.Nodes.getNodeAs<DeclStmt>("fopenDecl");
  const auto *FilePtr = Result.Nodes.getNodeAs<VarDecl>("filePtr");
  for (const auto *Trigger : {Assign, Decl}) {
    if (!Trigger)
      continue;
    llvm::SmallPtrSet<const Stmt *, 1> Closes;
    FilePointerCloseTracker Tracker(Result.Context);
    Tracker.findUseAfter(Closes, Func->getBody(), Trigger, FilePtr);
    if (Closes.empty()) {
      auto Loc = isa<DeclStmt>(Trigger) ? FilePtr->getLocation()
                                        : Trigger->getBeginLoc();
      diag(Loc, "禁止文件指针未关闭");
    }
  }
}
} // namespace clang::tidy::gjb8114