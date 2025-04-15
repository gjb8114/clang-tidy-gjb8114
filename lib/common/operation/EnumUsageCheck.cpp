#include "EnumUsageCheck.h"
using namespace clang::ast_matchers;

namespace clang::tidy::gjb8114 {
void EnumUsageCheck::registerMatchers(MatchFinder *Finder) {
  Finder->addMatcher(
      varDecl(unless(isExpansionInSystemHeader()), unless(hasType(enumType())),
              unless(hasType(elaboratedType(namesType(enumType())))),
              hasInitializer(ignoringImplicit(
                  declRefExpr(to(enumConstantDecl().bind("var"))))))
          .bind("enumInit"),
      this);
  auto NotEnumVar =
      varDecl(allOf(unless(hasType(enumType())),
                    unless(hasType(elaboratedType(namesType(enumType()))))));
  Finder->addMatcher(
      binaryOperator(
          unless(isExpansionInSystemHeader()), isAssignmentOperator(),
          hasLHS(declRefExpr(to(NotEnumVar.bind("var")))),
          hasRHS(ignoringImplicit(declRefExpr(to(enumConstantDecl())))))
          .bind("enumAssign"),
      this);
}

void EnumUsageCheck::check(const MatchFinder::MatchResult &Result) {
  const char *Description = "禁止非枚举类型变量赋值枚举值";
  if (const auto *EnumAssign =
          Result.Nodes.getNodeAs<BinaryOperator>("enumAssign"))
    diag(EnumAssign->getRHS()->getBeginLoc(), Description);
  if (const auto *EnumInit = Result.Nodes.getNodeAs<VarDecl>("enumInit"))
    diag(EnumInit->getInit()->getExprLoc(), Description);
}
} // namespace clang::tidy::gjb8114