#include "StdlibIdentifierCheck.h"

using namespace clang::ast_matchers;

namespace clang::tidy::gjb8114 {
void StdlibIdentifierCheck::registerMatchers(MatchFinder *Finder) {
  Finder->addMatcher(
      varDecl(hasAnyName("stdin", "stdout", "stderr", "EOF", "FILE", "cout",
                         "cin", "cerr", "clog", "endl", "wcout", "wcin",
                         "wcout", "wclog"))
          .bind("var"),
      this);
}

void StdlibIdentifierCheck::check(const MatchFinder::MatchResult &Result) {
  const auto *VD = Result.Nodes.getNodeAs<VarDecl>("var");
  if (!VD)
    return;

  if (Result.SourceManager->isInSystemHeader(VD->getLocation()))
    return;

  diag(VD->getLocation(), "禁止遮蔽标准库标识符");
}
} // namespace clang::tidy::gjb8114