#include "FunctionArgumentTypeCheck.h"

using namespace clang::ast_matchers;

namespace clang::tidy::gjb8114 {
void FunctionArgumentTypeCheck::registerMatchers(MatchFinder *Finder) {
  Finder->addMatcher(callExpr(unless(isExpansionInSystemHeader()),
                              callee(functionDecl().bind("callee")))
                         .bind("call"),
                     this);
}

void FunctionArgumentTypeCheck::check(const MatchFinder::MatchResult &Result) {
  const auto *Call = Result.Nodes.getNodeAs<CallExpr>("call");
  const auto *Callee = Result.Nodes.getNodeAs<FunctionDecl>("callee");
  if (!Call || !Callee)
    return;

  for (unsigned ArgNo = 0; ArgNo < Call->getNumArgs(); ++ArgNo) {
    const auto *Arg = Call->getArg(ArgNo);
    if (ArgNo >= Callee->getNumParams())
      break;

    auto ArgType = Arg->IgnoreImpCasts()->getType();
    auto ParamType = Callee->getParamDecl(ArgNo)->getType();

    if (ArgType == ParamType)
      continue;
    if (ArgType.getUnqualifiedType() == ParamType.getUnqualifiedType())
      continue;

    if (ArgType->isRecordType() && ParamType->isRecordType())
      continue;

    diag(Arg->getExprLoc(), "禁止实参和形参类型不一致")
        << Arg->getSourceRange();
  }
}
} // namespace clang::tidy::gjb8114