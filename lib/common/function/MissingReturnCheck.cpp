#include "MissingReturnCheck.h"
#include "clang/Analysis/CFG.h"

using namespace clang::ast_matchers;

namespace clang::tidy::gjb8114 {
void MissingReturnCheck::registerMatchers(MatchFinder *Finder) {
  Finder->addMatcher(functionDecl(unless(isImplicit()),
                                  unless(isExpansionInSystemHeader()),
                                  unless(hasType(voidType())),
                                  has(compoundStmt().bind("body")))
                         .bind("func"),
                     this);
}

void MissingReturnCheck::check(const MatchFinder::MatchResult &Result) {
  const auto *FuncDecl = Result.Nodes.getNodeAs<FunctionDecl>("func");
  const auto *Body = Result.Nodes.getNodeAs<CompoundStmt>("body");

  if (!FuncDecl || !Body || !FuncDecl->hasBody())
    return;

  CFG::BuildOptions Options;
  Options.AddImplicitDtors = true;
  std::unique_ptr<CFG> Cfg =
      CFG::buildCFG(FuncDecl, FuncDecl->getBody(), Result.Context, Options);
  if (!Cfg)
    return;
  for (const auto Block : Cfg->getExit().preds()) {
    if (Block->empty())
      continue;

    const auto *LastStmt = Block->back().getAs<CFGStmt>()->getStmt();
    if (isa<ReturnStmt>(LastStmt))
      continue;
    const auto *Parent = Result.Context->getParents(*LastStmt)[0].get<Stmt>();
    diag(Parent->getEndLoc(), "禁止非 void 函数省略 return 语句");
  }
}
} // namespace clang::tidy::gjb8114