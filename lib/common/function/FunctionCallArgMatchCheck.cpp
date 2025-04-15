#include "FunctionCallArgMatchCheck.h"
#include "clang/AST/ASTContext.h"

using namespace clang::ast_matchers;

namespace clang::tidy::gjb8114 {
namespace {
AST_MATCHER(Expr, isRecoveryExpr) { return isa<RecoveryExpr>(Node); }
} // namespace

void FunctionCallArgMatchCheck::registerMatchers(MatchFinder *Finder) {
  Finder->addMatcher(expr(unless(isExpansionInSystemHeader()), isRecoveryExpr(),
                          has(declRefExpr()))
                         .bind("recoveryNode"),
                     this);
}

void FunctionCallArgMatchCheck::check(const MatchFinder::MatchResult &Result) {
  const auto *Recovery = Result.Nodes.getNodeAs<RecoveryExpr>("recoveryNode");
  if (!Recovery)
    return;
  auto Exprs = Recovery->subExpressions();
  if (Exprs.empty())
    return;
  auto *First = dyn_cast<DeclRefExpr>(*Recovery->child_begin());
  auto *FuncDecl = dyn_cast<FunctionDecl>(First->getDecl());
  auto ExpectNumParams = FuncDecl->getNumParams();
  auto ActualNumParams = Exprs.size() - 1;
  if (ExpectNumParams == ActualNumParams)
    return;

  auto Loc =
      ExpectNumParams < ActualNumParams
          ? Recovery->subExpressions()[ExpectNumParams + 1]->getBeginLoc()
          : Recovery->getEndLoc();
  const char *Description = "禁止实参与形参的个数不一致";
  diag(Loc, Description);
}
} // namespace clang::tidy::gjb8114