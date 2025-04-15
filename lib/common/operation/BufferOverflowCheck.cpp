#include "BufferOverflowCheck.h"
#include "matchers/matcher.h"

using namespace clang::ast_matchers;

namespace clang::tidy::gjb8114 {
void BufferOverflowCheck::registerMatchers(MatchFinder *Finder) {
  auto ArrayVar = varDecl(hasType(arrayType()));
  Finder->addMatcher(
      callExpr(unless(isExpansionInSystemHeader()),
               callee(functionDecl(isMemcpy())),
               hasArgument(0, declRefExpr(to(ArrayVar.bind("dest")))),
               hasArgument(1, declRefExpr(to(ArrayVar.bind("src")))),
               hasArgument(2, anyOf(sizeOfExpr(has(ignoringParens(expr()))),
                                    ignoringParenImpCasts(integerLiteral()),
                                    ignoringParenImpCasts(expr()))))
          .bind("memcpy"),
      this);
}

void BufferOverflowCheck::check(const MatchFinder::MatchResult &Result) {
  const auto *Call = Result.Nodes.getNodeAs<CallExpr>("memcpy");
  if (!Call)
    return;

  auto &Context = *Result.Context;
  const auto *SizeArg = Call->getArg(2);
  Expr::EvalResult R;
  if (!SizeArg->EvaluateAsInt(R, Context))
    return;
  auto CopySize = R.Val.getInt().getLimitedValue();

  if (shouldCheckRead()) {
    const auto *SrcVar = Result.Nodes.getNodeAs<VarDecl>("src");
    const auto *Src =
        dyn_cast<ConstantArrayType>(SrcVar->getType()->getAsArrayTypeUnsafe());
    const auto SrcSize =
        Context.getTypeSizeInChars(Src->getElementType()).getQuantity() *
        Src->getLimitedSize();
    if (SrcSize < CopySize) {
      diag(SizeArg->getBeginLoc(), "禁止缓存区读取越界");
    }
  }

  if (shouldCheckWrite()) {
    const auto *DstVar = Result.Nodes.getNodeAs<VarDecl>("dest");
    const auto *Dst =
        dyn_cast<ConstantArrayType>(DstVar->getType()->getAsArrayTypeUnsafe());
    const auto DstSize =
        Context.getTypeSizeInChars(Dst->getElementType()).getQuantity() *
        Dst->getLimitedSize();
    if (DstSize < CopySize) {
      diag(SizeArg->getBeginLoc(), "禁止缓存区写入越界");
    }
  }
}
} // namespace clang::tidy::gjb8114