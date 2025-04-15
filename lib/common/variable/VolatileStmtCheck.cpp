#include "VolatileStmtCheck.h"
#include "clang/AST/RecursiveASTVisitor.h"

using namespace clang::ast_matchers;

namespace clang::tidy::gjb8114 {

namespace {
AST_MATCHER(VarDecl, isVolatileVar) {
  return Node.getType().isVolatileQualified();
}

class VolatileVarCounter : public RecursiveASTVisitor<VolatileVarCounter> {
public:
  explicit VolatileVarCounter(std::map<const VarDecl *, unsigned> &Counts)
      : VolatileVarCounts(Counts) {}

  bool VisitDeclRefExpr(const DeclRefExpr *E) {
    if (const auto *VD = dyn_cast<VarDecl>(E->getDecl())) {
      if (VD->getType().isVolatileQualified()) {
        VolatileVarCounts[VD]++;
      }
    }
    return true;
  }

private:
  std::map<const VarDecl *, unsigned> &VolatileVarCounts;
};

void countVolatileVarOccurrences(
    const Stmt *S, std::map<const VarDecl *, unsigned> &VolatileVarCounts) {
  VolatileVarCounter Counter(VolatileVarCounts);
  Counter.TraverseStmt(const_cast<Stmt *>(S));
}
} // namespace

void VolatileStmtCheck::registerMatchers(MatchFinder *Finder) {
  Finder->addMatcher(
      binaryOperator(
          unless(isExpansionInSystemHeader()),
          hasLHS(hasDescendant(declRefExpr(to(varDecl(isVolatileVar()))))),
          hasRHS(hasDescendant(declRefExpr(to(varDecl(isVolatileVar()))))))
          .bind("binOp"),
      this);
}

void VolatileStmtCheck::check(const MatchFinder::MatchResult &Result) {
  const auto *ExprOp = Result.Nodes.getNodeAs<BinaryOperator>("binOp");
  if (!ExprOp)
    return;

  std::map<const VarDecl *, unsigned> VolatileVarCounts;
  countVolatileVarOccurrences(ExprOp, VolatileVarCounts);
  auto HasMany = [](auto &Pair) { return Pair.second > 1; };
  auto Found = std::ranges::find_if(VolatileVarCounts, HasMany);
  if (Found != VolatileVarCounts.end()) {
    const auto &[VarDecl, Count] = *Found;
    diag(ExprOp->getOperatorLoc(), "禁止表达式多次引用同一 volatile 变量")
        << ExprOp->getSourceRange();
    diag(VarDecl->getLocation(), "volatile 变量 '%0' 在表达式中被使用了 %1 次",
         DiagnosticIDs::Note)
        << VarDecl->getName() << Count;
  }
}

} // namespace clang::tidy::gjb8114