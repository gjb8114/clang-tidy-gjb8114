#include "StringBufferOverflowCheck.h"
#include "matchers/matcher.h"
#include "clang/AST/ASTContext.h"

using namespace clang::ast_matchers;

namespace clang::tidy::gjb8114 {

void StringBufferOverflowCheck::registerMatchers(
    ast_matchers::MatchFinder *Finder) {
  auto ArrayVar = varDecl(hasType(arrayType()));
  Finder->addMatcher(
      callExpr(callee(functionDecl(anyOf(isStrncpy(), isStrncat()))),
               hasArgument(0, declRefExpr(to(ArrayVar.bind("dest")))))
          .bind("call"),
      this);
}

void StringBufferOverflowCheck::check(
    const ast_matchers::MatchFinder::MatchResult &Result) {
  const auto *Call = Result.Nodes.getNodeAs<CallExpr>("call");
  if (!Call)
    return;

  auto &Context = *Result.Context;
  const auto *CountArg = Call->getArg(2);
  Expr::EvalResult R;
  if (!CountArg->EvaluateAsInt(R, Context))
    return;
  auto Count = R.Val.getInt().getLimitedValue();

  const Expr *DstArg = Call->getArg(0);
  const auto *DstDeclRef = dyn_cast<DeclRefExpr>(DstArg->IgnoreParenImpCasts());
  const auto *VD = dyn_cast<VarDecl>(DstDeclRef->getDecl());
  if (const auto *CAT =
          dyn_cast<ConstantArrayType>(VD->getType().getTypePtr())) {
    auto DstSize =
        Context.getTypeSizeInChars(CAT->getElementType()).getQuantity() *
        CAT->getLimitedSize();
    auto RequiredSize = Count;
    if (RequiredSize > DstSize) {
      const char *Description = "禁止字符串函数操作导致目标存储空间越界";
      diag(CountArg->getBeginLoc(), Description) << CountArg->getSourceRange();
    }
  }
}
} // namespace clang::tidy::gjb8114