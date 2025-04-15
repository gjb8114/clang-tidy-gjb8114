#include "ArrayExplicitBoundsCheck.h"
#include <clang/Lex/Lexer.h>

using namespace clang::ast_matchers;

namespace clang::tidy::gjb8114 {
namespace {
AST_MATCHER(ArrayType, hasExplicitBounds) { return Node.isConstantSizeType(); }
} // namespace

ArrayExplicitBoundsCheck::ArrayExplicitBoundsCheck(StringRef Name,
                                                   ClangTidyContext *Context)
    : ClangTidyCheck(Name, Context) {}

void ArrayExplicitBoundsCheck::registerMatchers(MatchFinder *Finder) {
  Finder->addMatcher(
      varDecl(hasType(constantArrayType(hasExplicitBounds()))).bind("array"),
      this);
}

void ArrayExplicitBoundsCheck::check(const MatchFinder::MatchResult &Result) {
  const auto *Array = Result.Nodes.getNodeAs<VarDecl>("array");
  if (!Array)
    return;
  const auto *ArrayType = Array->getType()->getAsArrayTypeUnsafe();
  if (!ArrayType)
    return;
  auto TL = Array->getTypeSourceInfo()->getTypeLoc();
  if (!TL)
    return;
  auto CATL = TL.getAs<ConstantArrayTypeLoc>();
  if (!CATL) {
    diag(TL.getEndLoc(), "禁止数组定义没有显式边界") << TL.getSourceRange();
  }
}
} // namespace clang::tidy::gjb8114