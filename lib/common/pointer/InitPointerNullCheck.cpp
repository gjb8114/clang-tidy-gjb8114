#include "InitPointerNullCheck.h"
#include "matchers/matcher.h"

using namespace clang::ast_matchers;

namespace clang::tidy::gjb8114 {
void InitPointerNullCheck::registerMatchers(MatchFinder *Finder) {
  Finder->addMatcher(
      varDecl(hasType(pointerType()), unless(isExpansionInSystemHeader()),
              unless(hasInitializer(anything())), hasLocalStorage())
          .bind("uninitPointer"),
      this);
}

void InitPointerNullCheck::check(const MatchFinder::MatchResult &Result) {
  if (const auto *VD = Result.Nodes.getNodeAs<VarDecl>("uninitPointer")) {
    if (VD->getName().empty())
      return;
    diag(VD->getLocation(), "禁止动态分配的指针变量未初始化");
  }
}
} // namespace clang::tidy::gjb8114