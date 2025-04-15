#include "StructInitializationCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/AST/RecursiveASTVisitor.h"

using namespace clang::ast_matchers;

namespace clang::tidy::gjb8114 {

void StructInitializationCheck::registerMatchers(MatchFinder *Finder) {
  Finder->addMatcher(varDecl(hasType(recordDecl()),
                             hasInitializer(initListExpr().bind("initList")))
                         .bind("structVar"),
                     this);
}

void StructInitializationCheck::check(const MatchFinder::MatchResult &Result) {
  const auto *VD = Result.Nodes.getNodeAs<VarDecl>("structVar");
  const auto *InitList = Result.Nodes.getNodeAs<InitListExpr>("initList");

  if (!VD || !InitList)
    return;

  QualType VarType = VD->getType();
  const RecordDecl *RD = VarType->getAsRecordDecl();
  if (!RD)
    return;

  const auto *SyntacticInitList = InitList->getSyntacticForm();
  InitList = SyntacticInitList ? SyntacticInitList : InitList;

  for (const FieldDecl *Field : RD->fields()) {
    QualType FieldType = Field->getType();
    if (!FieldType->isRecordType())
      continue;

    unsigned FieldCount = Field->getFieldIndex();
    if (FieldCount > InitList->getNumInits())
      break;

    const Expr *FieldInit = InitList->getInit(FieldCount);
    if (!isa<InitListExpr>(FieldInit->IgnoreImplicit())) {
      diag(FieldInit->getBeginLoc(),
           "禁止结构体初始化列表的嵌套结构与定义不一致");
      diag(RD->getLocation(), "'%0' 定义在此", DiagnosticIDs::Note)
          << RD->getName();
    }
  }
}
} // namespace clang::tidy::gjb8114