#include "UnreachableStatementCheck.h"
#include "clang/Analysis/Analyses/ReachableCode.h"
#include "clang/Analysis/CFG.h"
#include "llvm/ADT/BitVector.h"
#include <ranges>

using namespace clang::ast_matchers;

namespace clang::tidy::gjb8114 {

void UnreachableStatementCheck::registerMatchers(MatchFinder *Finder) {
  Finder->addMatcher(
      functionDecl(isDefinition(), unless(isExpansionInSystemHeader()))
          .bind("func"),
      this);
}

namespace {
bool IsCompoundIfStmt(const CFGBlock *Block, ASTContext &Context) {
  if (Block->empty())
    return false;

  if (auto Element = Block->front().getAs<CFGStmt>()) {
    auto *Stmt = Element->getStmt();
    auto Parents = Context.getParents(*Stmt);
    for (const auto &Parent : Parents) {
      if (const auto *Comp = Parent.get<CompoundStmt>()) {
        for (const auto GrandParent : Context.getParents(*Comp)) {
          if (const auto *IfS = GrandParent.get<IfStmt>()) {
            if (IfS->getThen() == Comp || IfS->getElse() == Comp) {
              // This CFGBlock corresponds to the compound statement of an
              // if/else.
              return true;
            }
          }
        }
      }
    }
  }
  return false;
}

} // namespace
void UnreachableStatementCheck::check(const MatchFinder::MatchResult &Result) {
  const auto *FD = Result.Nodes.getNodeAs<FunctionDecl>("func");
  if (!FD || !FD->hasBody())
    return;

  auto TheCFG = CFG::buildCFG(FD, FD->getBody(), Result.Context, {});
  if (!TheCFG || TheCFG->size() == 0)
    return;

  llvm::BitVector ReachableBlocks(TheCFG->getNumBlockIDs(), false);
  reachable_code::ScanReachableFromBlock(&TheCFG->getEntry(), ReachableBlocks);

  auto IsUnreachable = [&](const CFGBlock *Block) {
    return Block != &TheCFG->getEntry() && Block != &TheCFG->getExit() &&
           !Block->empty() && !ReachableBlocks[Block->getBlockID()] &&
           // r-1-8-2 responsible to cover whole unreachable if-else branch
           !IsCompoundIfStmt(Block, *Result.Context);
  };
  auto IsStmt = [](auto &E) { return E.getKind() == CFGElement::Statement; };
  using std::views::filter;
  for (const auto *Unreachable : *TheCFG | filter(IsUnreachable)) {
    for (const auto &E : *Unreachable | filter(IsStmt)) {
      const Stmt *Statement = ((const CFGStmt &)E).getStmt();
      if (Statement && Statement->getBeginLoc().isValid()) {
        diag(Statement->getBeginLoc(), "禁止不可达语句");
        break; // Only report the first statement in each unreachable block
      }
    }
  }
}
} // namespace clang::tidy::gjb8114