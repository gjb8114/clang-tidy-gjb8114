#include "LocalVariableAddressReturnCheck.h"

using namespace clang::ast_matchers;

namespace clang::tidy::gjb8114 {
void LocalVariableAddressReturnCheck::registerMatchers(MatchFinder *Finder) {
  Finder->addMatcher(
      returnStmt(unless(isExpansionInSystemHeader()),
                 hasReturnValue(unaryOperator(hasOperatorName("&"),
                                              hasUnaryOperand(declRefExpr(to(
                                                  varDecl(hasLocalStorage())))))
                                    .bind("addrOf")))
          .bind("returnStmt"),
      this);
}

void LocalVariableAddressReturnCheck::check(
    const MatchFinder::MatchResult &Result) {
  const auto *AddrOf = Result.Nodes.getNodeAs<UnaryOperator>("addrOf");
  if (!AddrOf)
    return;

  diag(AddrOf->getBeginLoc(), "禁止返回局部变量地址")
      << AddrOf->getSourceRange();
}
} // namespace clang::tidy::gjb8114