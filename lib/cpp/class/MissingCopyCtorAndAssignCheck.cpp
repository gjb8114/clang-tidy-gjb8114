#include "MissingCopyCtorAndAssignCheck.h"
#include <ranges>

using namespace clang::ast_matchers;

namespace clang::tidy::gjb8114 {
void MissingCopyCtorAndAssignCheck::registerMatchers(MatchFinder *Finder) {
  Finder->addMatcher(
      cxxConstructorDecl(
          unless(isCopyConstructor()),
          anyOf(
              // allocate via an initializer list
              hasAnyConstructorInitializer(allOf(
                  forField(fieldDecl(hasType(isAnyPointer())).bind("ptrField")),
                  withInitializer(cxxNewExpr().bind("newExpr")))),
              // allocate via assign in the body
              hasBody(hasDescendant(binaryOperator(
                  isAssignmentOperator(),
                  hasLHS(memberExpr(
                      hasType(isAnyPointer()),
                      hasDeclaration(fieldDecl(hasType(isAnyPointer()))
                                         .bind("ptrField")))),
                  hasRHS(cxxNewExpr().bind("newExpr")))))))
          .bind("ctorWithNew"),
      this);
}

void MissingCopyCtorAndAssignCheck::check(
    const MatchFinder::MatchResult &Result) {
  const auto *CtorWithNew =
      Result.Nodes.getNodeAs<CXXConstructorDecl>("ctorWithNew");
  if (!CtorWithNew)
    return;

  const auto *Class = CtorWithNew->getParent();
  using std::ranges::any_of;
  bool HasCopyCtor = any_of(Class->ctors(), [](const auto *C) {
    return C->isCopyConstructor() && !C->isDefaulted();
  });
  bool HasCopyAssign = any_of(Class->methods(), [](const auto *M) {
    return M->isCopyAssignmentOperator() && !M->isDefaulted();
  });

  if (!HasCopyCtor || !HasCopyAssign) {
    diag(Class->getLocation(),
         "禁止含动态分配成员的类省略拷贝构造函数和赋值操作符");
    const auto *Field = Result.Nodes.getNodeAs<FieldDecl>("ptrField");
    diag(Field->getLocation(), "含动态分配成员: '%0'", DiagnosticIDs::Note)
        << Field->getName();
  }
}
} // namespace clang::tidy::gjb8114