#include "FunctionPointerAddressCheck.h"

using namespace clang::ast_matchers;

namespace clang::tidy::gjb8114 {
void FunctionPointerAddressCheck::registerMatchers(MatchFinder *Finder) {
  Finder->addMatcher(
      implicitCastExpr(
          hasSourceExpression(declRefExpr(to(functionDecl())).bind("funcRef")),
          hasImplicitDestinationType(pointerType(pointee(functionType())))
      ).bind("implicitCast"),
      this);
}

void FunctionPointerAddressCheck::check(const MatchFinder::MatchResult &Result) {
  const auto *ImplicitCast = Result.Nodes.getNodeAs<ImplicitCastExpr>("implicitCast");
  const auto *FuncRef = Result.Nodes.getNodeAs<DeclRefExpr>("funcRef");
  
  if (!ImplicitCast || !FuncRef || Result.SourceManager->isInSystemHeader(ImplicitCast->getBeginLoc()))
    return;
  
  const Expr *Parent = ImplicitCast->getSubExpr();
  bool hasAddressOf = false;
  
  if (const auto *UO = dyn_cast<UnaryOperator>(Parent))
    if (UO->getOpcode() == UO_AddrOf)
      hasAddressOf = true;
  
  if (!hasAddressOf) {
    diag(FuncRef->getBeginLoc(), "禁止函数隐式转换为指针")
        << FuncRef->getSourceRange();
  }
}
} // namespace clang::tidy::gjb8114