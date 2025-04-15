#include "UnreachableBranchCheck.h"
#include "matchers/compose.h"
#include "utils/CFGVariableUseTracker.h"
#include "clang/AST/StmtVisitor.h"
#include "clang/Analysis/Analyses/ReachableCode.h"

using namespace clang::ast_matchers;

namespace clang::tidy::gjb8114 {

namespace {
class VariableMutateTracker : public utils::CFGVariableUseTracker {
public:
  explicit VariableMutateTracker(ASTContext *Context)
      : CFGVariableUseTracker(Context) {}

protected:
  ast_matchers::internal::BindableMatcher<Stmt>
  useMatcher(const ast_matchers::StatementMatcher &DeclRefMatcher) override {
    return matchers::mutableDeclRef(DeclRefMatcher);
  }
};

class UnreachableBranchFinder
    : public ConstStmtVisitor<UnreachableBranchFinder> {
public:
  UnreachableBranchFinder(ASTContext &Context, CFG &TheCFG, const Stmt *Root)
      : Context(Context), TheCFG(TheCFG), Root(Root) {}

  void VisitStmt(const Stmt *S) {
    if (!S)
      return;

    for (const Stmt *Child : S->children())
      if (Child)
        Visit(Child);
  }

  void VisitIfStmt(const IfStmt *If) {
    if (!If)
      return;

    if (bool IsUnreachableElseOfElseIF = UnreachableBranches.contains(If)) {
      // then all the consequent else-if and else are unreachable
      UnreachableBranches.emplace(If->getThen(), If->getBeginLoc());
      UnreachableBranches.emplace(If->getElse(), If->getElseLoc());
      // remove the current prepending else to eliminate duplicate
      UnreachableBranches.erase(If);
    } else {
      const Expr *Cond = If->getCond();
      if (!Cond)
        return;

      std::optional<bool> CondValue;
      Expr::EvalResult R;
      if (Cond->EvaluateAsInt(R, Context)) {
        llvm::outs() << "Condition value: " << R.Val.getInt() << "\n";
        CondValue = R.Val.getInt().getBoolValue();
      } else {
        llvm::outs() << "Condition is not a constant expression\n";
        CondValue = tryDeduceCondExpr(Cond);
        if (!CondValue) {
          llvm::outs() << "Condition is not deducible\n";
        }
      }

      if (CondValue) {
        // If condition is always true, else branch is unreachable
        if (*CondValue && If->getElse()) {
          UnreachableBranches.emplace(If->getElse(), If->getElseLoc());
        }
        // If condition is always false, then branch is unreachable
        else if (!*CondValue && If->getThen()) {
          UnreachableBranches.emplace(If->getThen(), If->getBeginLoc());
        }
      }
    }

    if (const auto *Then = If->getThen()) {
      if (!UnreachableBranches.contains(Then))
        Visit(Then);
    }
    if (const auto *Else = If->getElse()) {
      bool IsElseIf = isa<IfStmt>(Else);
      if (IsElseIf || !UnreachableBranches.contains(Else))
        Visit(Else);
    }
  }

  std::optional<uint64_t> tryDeduceDeclRefExpr(const DeclRefExpr *DRE) {
    if (const auto *VD = dyn_cast<VarDecl>(DRE->getDecl())) {
      VariableMutateTracker Tracker(&Context);
      llvm::SmallPtrSet<const Stmt *, 8> MutableUsages;
      Tracker.findUseBefore(MutableUsages, const_cast<Stmt *>(Root), DRE,
                            DRE->getDecl(), &TheCFG);
      if (!MutableUsages.empty())
        return std::nullopt;
      if (const Expr *Init = VD->getInit()) {
        Expr::EvalResult R;
        if (Init->EvaluateAsInt(R, Context)) {
          return R.Val.getInt().getZExtValue();
        }
      }
    }
    return std::nullopt;
  }

  std::optional<uint64_t> tryDeduceExpr(const Expr *E) {
    if (const auto *DRE = dyn_cast<DeclRefExpr>(E->IgnoreParenImpCasts())) {
      return tryDeduceDeclRefExpr(DRE);
    } else if (const auto *Lit =
                   dyn_cast<IntegerLiteral>(E->IgnoreParenImpCasts())) {
      return Lit->getValue().getZExtValue();
    }
    return std::nullopt;
  }

  std::optional<bool> tryDeduceCondExpr(const Expr *Cond) {
    if (const auto *BO = dyn_cast<BinaryOperator>(Cond)) {
      if (BO->isComparisonOp()) {
        auto *LHS = BO->getLHS()->IgnoreParenImpCasts();
        auto *RHS = BO->getRHS()->IgnoreParenImpCasts();
        auto LV = tryDeduceExpr(LHS);
        auto RV = tryDeduceExpr(RHS);
        if (!LV && !RV)
          return std::nullopt;

        auto Op = BO->getOpcode();
        if (LV && RV) {
          switch (Op) {
          case BO_LT:
            return *LV < *RV;
          case BO_GT:
            return *LV > *RV;
          case BO_LE:
            return *LV <= *RV;
          case BO_GE:
            return *LV >= *RV;
          case BO_EQ:
            return *LV == *RV;
          case BO_NE:
            return *LV != *RV;
          default:
            break;
          }
        }

        // only one side is deducible, try to deduce ALWAYS-TRUE pattern
        // - unsigned > 0, unsigned >= 0, 0 < unsigned, 0 <= unsigned
        if (!LV && LHS->getType()->isUnsignedIntegerType()) {
          // unsigned >= 0, and unsigned > 0
          if (RV == 0 && (Op == BO_GE || Op == BO_GT))
            return true;
        }
        if (!RV && RHS->getType()->isUnsignedIntegerType()) {
          // 0 <= unsigned, and 0 < unsigned
          if (LV == 0 && (Op == BO_LE || Op == BO_LT))
            return true;
        }
      }
    }
    return std::nullopt;
  }

  const auto &getUnreachableBranches() const { return UnreachableBranches; }

private:
  std::map<const Stmt *, SourceLocation> UnreachableBranches;
  ASTContext &Context;
  CFG &TheCFG;
  const Stmt *Root;
};
} // namespace

void UnreachableBranchCheck::registerMatchers(MatchFinder *Finder) {
  Finder->addMatcher(
      functionDecl(isDefinition(), unless(isExpansionInSystemHeader()))
          .bind("func"),
      this);
}

void UnreachableBranchCheck::check(const MatchFinder::MatchResult &Result) {
  const auto *FD = Result.Nodes.getNodeAs<FunctionDecl>("func");
  if (!FD || !FD->hasBody())
    return;

  auto TheCFG =
      CFG::buildCFG(FD, FD->getBody(), Result.Context, CFG::BuildOptions());
  if (!TheCFG || TheCFG->size() == 0)
    return;

  UnreachableBranchFinder Finder(*Result.Context, *TheCFG, FD->getBody());
  Finder.Visit(FD->getBody());

  for (const auto &[S, Loc] : Finder.getUnreachableBranches())
    if (Loc.isValid())
      diag(Loc, "禁止不可达分支");
}

} // namespace clang::tidy::gjb8114