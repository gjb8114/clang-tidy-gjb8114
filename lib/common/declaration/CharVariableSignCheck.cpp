#include "CharVariableSignCheck.h"
#include <clang/Lex/Lexer.h>

using namespace clang::ast_matchers;

namespace clang::tidy::gjb8114 {
namespace {
AST_MATCHER(QualType, isCharType) { return Node->isCharType(); }

AST_MATCHER(Expr, isArithmeticOperation) {
  if (const auto *BO = dyn_cast<BinaryOperator>(&Node)) {
    return BO->getOpcode() == BO_Add || BO->getOpcode() == BO_Sub ||
           BO->getOpcode() == BO_Mul || BO->getOpcode() == BO_Div ||
           BO->getOpcode() == BO_Rem;
  }
  return false;
}
} // namespace

CharVariableSignCheck::CharVariableSignCheck(StringRef Name,
                                             ClangTidyContext *Context)
    : ClangTidyCheck(Name, Context) {}

void CharVariableSignCheck::registerMatchers(MatchFinder *Finder) {
  Finder->addMatcher(varDecl(hasType(isCharType())).bind("charVar"), this);
  Finder->addMatcher(
      binaryOperator(isArithmeticOperation(),
                     hasEitherOperand(ignoringParenImpCasts(declRefExpr(to(
                         varDecl(hasType(isCharType())).bind("charVarExpr"))))))
          .bind("arithExpr"),
      this);
  Finder->addMatcher(
      unaryOperator(anyOf(hasOperatorName("++"), hasOperatorName("--")),
                    hasUnaryOperand(declRefExpr(to(
                        varDecl(hasType(isCharType())).bind("charVarExpr")))))
          .bind("arithExpr"),
      this);
}

void CharVariableSignCheck::check(const MatchFinder::MatchResult &Result) {
  if (const auto *CharVar = Result.Nodes.getNodeAs<VarDecl>("charVar")) {
    QualType Type = CharVar->getType();
    auto TName = Type.getAsString();
    if (TName != "char")
      return;
    auto *TSI = CharVar->getTypeSourceInfo();
    if (!TSI)
      return;
    auto TL = TSI->getTypeLoc();
    auto Range = CharSourceRange::getTokenRange(TL.getSourceRange());
    auto Text = Lexer::getSourceText(Range, *Result.SourceManager,
                                     Result.Context->getLangOpts());
    if (!Text.starts_with("signed") && !Text.starts_with("unsigned"))
      PotentialViolations.insert(CharVar);
    return;
  }

  SourceLocation OpLoc;
  if (auto *BOE = Result.Nodes.getNodeAs<BinaryOperator>("arithExpr")) {
    OpLoc = BOE->getOperatorLoc();
  } else if (auto *UOE = Result.Nodes.getNodeAs<UnaryOperator>("arithExpr")) {
    OpLoc = UOE->getOperatorLoc();
  }
  if (OpLoc.isInvalid())
    return;

  const auto *VD = Result.Nodes.getNodeAs<VarDecl>("charVarExpr");
  if (!VD)
    return;
  if (PotentialViolations.count(VD) == 0)
    return;
  PotentialViolations.erase(VD);
  diag(VD->getTypeSpecStartLoc(), "禁止用于数值计算的字符型变量未定义符号性")
      << VD->getSourceRange();
  diag(OpLoc, "此处进行了数值计算", DiagnosticIDs::Note);
}
} // namespace clang::tidy::gjb8114