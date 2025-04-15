#include "UselessCastCheck.h"

using namespace clang::ast_matchers;

namespace clang::tidy::gjb8114 {
void UselessCastCheck::registerMatchers(MatchFinder *Finder) {
  Finder->addMatcher(
      explicitCastExpr(
          hasSourceExpression(expr(hasType(qualType().bind("sameType")))),
          hasDestinationType(qualType(equalsBoundNode("sameType"))))
          .bind("identicalCast"),
      this);
  auto PromotionArithmeticOperator =
      anyOf(hasOperatorName("+"), hasOperatorName("-"), hasOperatorName("*"),
            hasOperatorName("/"), hasOperatorName("%"));
  Finder->addMatcher(
      binaryOperator(
          PromotionArithmeticOperator,
          hasOperands(
              ignoringParenImpCasts(explicitCastExpr(
                  hasSourceExpression(
                      expr(hasType(qualType(anyOf(realFloatingPointType(),
                                                  isInteger()))
                                       .bind("srcType")))
                          .bind("srcExpr")),
                  hasDestinationType(qualType().bind("destType")))),
              // explicit cast on the other side may cause implicit cast on this
              // side
              ignoringParenImpCasts(expr().bind("otherExpr"))))
          .bind("binOp"),
      this);
}

namespace {
// If LHS > RHS, returns 1. If LHS == RHS, returns 0. If LHS RHS, return -1.
int getNumberTypeOrder(ASTContext &Context, QualType LHS, QualType RHS) {
  if (LHS->isIntegerType() && RHS->isIntegerType())
    return Context.getIntegerTypeOrder(LHS, RHS);
  if (LHS->isRealFloatingType() && RHS->isRealFloatingType())
    return Context.getFloatingTypeOrder(LHS, RHS);
  if (LHS->isIntegerType() && RHS->isRealFloatingType())
    return -1;
  if (LHS->isRealFloatingType() && RHS->isIntegerType())
    return 1;
  return 0;
}

QualType getPromotedType(ASTContext &Context, QualType LHS, QualType RHS) {
  auto O = getNumberTypeOrder(Context, LHS, RHS);
  return O >= 0 ? LHS : RHS;
}
} // namespace

void UselessCastCheck::check(const MatchFinder::MatchResult &Result) {
  const char *Description = "禁止无实质作用的类型转换";
  if (auto *Cast = Result.Nodes.getNodeAs<ExplicitCastExpr>("identicalCast")) {
    diag(Cast->getBeginLoc(), Description) << Cast->getSourceRange();
    return;
  }

  if (const auto *BinOp = Result.Nodes.getNodeAs<BinaryOperator>("binOp")) {
    auto *SrcExpr = Result.Nodes.getNodeAs<Expr>("srcExpr");
    auto SrcType = *Result.Nodes.getNodeAs<QualType>("srcType");
    auto DstType = *Result.Nodes.getNodeAs<QualType>("destType");
    if (getNumberTypeOrder(*Result.Context, SrcType, DstType) >= 0)
      return;

    auto OtherType = Result.Nodes.getNodeAs<Expr>("otherExpr")->getType();
    auto PromotedType = getPromotedType(*Result.Context, SrcType, OtherType);

    if (getNumberTypeOrder(*Result.Context, PromotedType, DstType) >= 0) {
      diag(BinOp->getBeginLoc(), Description) << BinOp->getSourceRange();
      diag(SrcExpr->getBeginLoc(), "类型会自动提升为 %0", DiagnosticIDs::Note)
          << SrcExpr->getSourceRange() << PromotedType.getAsString();
      return;
    }
  }
}
} // namespace clang::tidy::gjb8114