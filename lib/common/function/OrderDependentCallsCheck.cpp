#include "OrderDependentCallsCheck.h"
#include "matchers/matcher.h"
#include "clang/AST/ASTContext.h"

using namespace clang::ast_matchers;

namespace clang::tidy::gjb8114 {
namespace {
AST_MATCHER(LValueReferenceType, isMutable) {
  return !Node.getPointeeType().isConstQualified();
}

auto mutableLValueRefType() { return lValueReferenceType(isMutable()); }

auto nonConstPointerType() {
  return pointerType(pointee(unless(isConstQualified())));
}

bool isMutable(const ParmVarDecl *Param) {
  auto Type = Param->getType();
  if (const auto *Ptr = Type->getAs<PointerType>())
    return !Ptr->getPointeeType().isConstQualified();
  if (const auto *Ref = Type->getAs<LValueReferenceType>())
    return !Ref->getPointeeType().isConstQualified();
  return false;
}

const DeclRefExpr *unwrapArg(const Expr *Arg) {
  if (const auto *DRE = dyn_cast<DeclRefExpr>(Arg->IgnoreParenImpCasts()))
    return DRE;

  if (const auto *AddrOf =
          dyn_cast<UnaryOperator>(Arg->IgnoreParenImpCasts())) {
    if (AddrOf->getOpcode() == UO_AddrOf)
      return dyn_cast<DeclRefExpr>(AddrOf->getSubExpr()->IgnoreParenImpCasts());
  }
  return nullptr;
}

auto getDeclRefArgs(const CallExpr *Call) {
  std::set<const DeclRefExpr *> ReferredArgs;
  for (const auto *Arg : Call->arguments()) {
    if (const auto *DRE = unwrapArg(Arg)) {
      ReferredArgs.insert(DRE);
    }
  }
  return ReferredArgs;
}

std::set<const DeclRefExpr *>
getMutableArgVars(const CallExpr *TheCall, const FunctionDecl *CalleeDecl) {
  auto NumParams = CalleeDecl->getNumParams();
  auto NumArgs = TheCall->getNumArgs();
  auto N = std::min(NumParams, NumArgs);

  std::set<const DeclRefExpr *> ModifiedArgs;
  for (auto I = 0u; I < N; ++I) {
    const auto *Param = CalleeDecl->getParamDecl(I);
    if (!isMutable(Param))
      continue;

    const auto *Arg = TheCall->getArg(I);
    if (const auto *DRE = unwrapArg(Arg))
      ModifiedArgs.insert(DRE);
  }
  return ModifiedArgs;
}

} // namespace
void OrderDependentCallsCheck::registerMatchers(
    ast_matchers::MatchFinder *Finder) {
  const auto mutableParamMatcher =
      anyOf(parmVarDecl(hasType(nonConstPointerType())),
            parmVarDecl(hasType(mutableLValueRefType())));
  auto mutableParamFuncDecl =
      functionDecl(hasAnyParameter(mutableParamMatcher));

  Finder->addMatcher(
      binaryOperator(
          unless(isExpansionInSystemHeader()),
          hasOperands(
              // at least one side has side effect function call
              callExpr(callee(mutableParamFuncDecl)).bind("sideEffectCall"),
              // other side may or may not have a side effect
              callExpr(hasAnyArgument(hasDescendant(
                           declRefExpr(to(varDecl().bind("var"))))))
                  .bind("callWithVar")))
          .bind("binop"),
      this);
}

void OrderDependentCallsCheck::check(const MatchFinder::MatchResult &Result) {
  const auto *BinOp = Result.Nodes.getNodeAs<BinaryOperator>("binop");
  if (!BinOp)
    return;

  const auto *ThisCall = Result.Nodes.getNodeAs<CallExpr>("sideEffectCall");
  const auto *ThatCall = Result.Nodes.getNodeAs<CallExpr>("callWithVar");
  auto ThisMutates = getMutableArgVars(ThisCall, ThisCall->getDirectCallee());
  auto ThisRefers = getDeclRefArgs(ThisCall);
  auto ThatMutates = getMutableArgVars(ThatCall, ThatCall->getDirectCallee());
  auto ThatRefers = getDeclRefArgs(ThatCall);
  std::vector<const DeclRefExpr *> Inters;
  auto comp = [](auto *L, auto *R) { return L->getDecl() < R->getDecl(); };
  using std::ranges::set_intersection;
  set_intersection(ThisMutates, ThatRefers, std::back_inserter(Inters), comp);
  set_intersection(ThatMutates, ThisRefers, std::back_inserter(Inters), comp);
  auto Ambiguous = !Inters.empty();
  if (!Ambiguous)
    return;
  diag(BinOp->getOperatorLoc(), "禁止同一表达式中调用多个相关函数")
      << BinOp->getSourceRange();
  for (const auto *Arg : Inters) {
    diag(Arg->getBeginLoc(), "'%0' 可能被多处修改和引用，发生顺序不可预期",
         DiagnosticIDs::Note)
        << Arg->getDecl()->getName();
  }
}
} // namespace clang::tidy::gjb8114