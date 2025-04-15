#pragma once

#include "matcher.h"

#include "clang-tidy/utils/Matchers.h"

namespace clang::tidy::gjb8114::matchers {
inline auto hasMallocInitializer() {
  using namespace clang::ast_matchers;
  return hasInitializer(
      cStyleCastExpr(has(callExpr(callee(functionDecl(hasName("malloc")))))));
}

inline auto derefPointerExpr() {
  using namespace clang::ast_matchers;
  return unaryOperator(
      hasOperatorName("*"),
      hasUnaryOperand(ignoringParenImpCasts(expr(unless(
          hasDescendant(callExpr(callee(functionDecl(hasName("malloc"))))))))));
}

inline auto subscriptPointerExpr() {
  using namespace clang::ast_matchers;
  return arraySubscriptExpr(hasBase(ignoringParenImpCasts(expr(unless(
      hasDescendant(callExpr(callee(functionDecl(hasName("malloc"))))))))));
}

inline auto
derefPointerExpr(const ast_matchers::internal::Matcher<Expr> &DeclRefMatcher) {
  using namespace clang::ast_matchers;
  return unaryOperator(hasOperatorName("*"),
                       hasUnaryOperand(ignoringParenImpCasts(DeclRefMatcher)));
}

inline auto subscriptPointerExpr(
    const ast_matchers::internal::Matcher<Expr> &DeclRefMatcher) {
  using namespace clang::ast_matchers;
  return arraySubscriptExpr(hasBase(ignoringParenImpCasts(DeclRefMatcher)));
}

inline auto
mutableDeclRef(const ast_matchers::StatementMatcher &DeclRefMatcher) {
  using namespace clang::ast_matchers;
  auto ConstAddrOf = hasParent(unaryOperator(
      hasOperatorName("&"), hasUnaryOperand(DeclRefMatcher),
      hasParent(castExpr(hasType(pointerType(pointee(isConstQualified())))))));
  auto ConstRef = hasParent(castExpr(hasType(qualType(isConstQualified()))));
  auto RightRef = hasParent(castExpr(hasCastKind(CK_LValueToRValue)));
  return expr(DeclRefMatcher, isLValue(),
              unless(anyOf(RightRef, ConstRef, ConstAddrOf)));
}
} // namespace clang::tidy::gjb8114::matchers