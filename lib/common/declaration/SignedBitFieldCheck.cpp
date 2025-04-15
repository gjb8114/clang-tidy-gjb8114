#include "SignedBitFieldCheck.h"

using namespace clang::ast_matchers;

namespace clang::tidy::gjb8114 {
void SignedBitFieldCheck::registerMatchers(MatchFinder *Finder) {
  Finder->addMatcher(fieldDecl(hasBitWidth(1), hasType(isSignedInteger()))
                         .bind("singleSignedBitField"),
                     this);
}

void SignedBitFieldCheck::check(const MatchFinder::MatchResult &Result) {
  const auto *FD =
      Result.Nodes.getNodeAs<clang::FieldDecl>("singleSignedBitField");
  if (!FD)
    return;

  auto Loc = FD->getLocation();
  const auto &SM = Result.Context->getSourceManager();
  if (Loc.isInvalid() || SM.isInSystemHeader(Loc))
    return;

  diag(FD->getBitWidth()->getExprLoc(), "禁止有符号位域的位宽等于 1");
}
} // namespace clang::tidy::gjb8114