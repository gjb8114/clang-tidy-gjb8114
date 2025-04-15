#include "RecursiveFunctionCheck.h"
#include "clang/Analysis/CallGraph.h"
#include "llvm/ADT/SCCIterator.h"

#include <ranges>

using namespace clang::ast_matchers;

namespace clang::tidy::gjb8114 {
namespace {
using Record = CallGraphNode::CallRecord;

// This will only find *one* such stack, it might not be the smallest one,
// and there may be other loops within the same SCC.
auto formCallStack(ArrayRef<CallGraphNode *> SCC) {
  auto IsPartOfSCC = [Set = std::set<CallGraphNode *>(SCC.begin(), SCC.end())](
                         CallGraphNode *N) { return Set.contains(N); };

  // Start pathfinding arbitrarily from the first node in the SCC.
  // The initial CallExpr is null as this is the entry point for pathfinding.
  Record EntryNode(SCC.front(), nullptr);
  std::vector<Record> CallStack{EntryNode};
  auto *CurrentRecord = &EntryNode;
  std::set<Record> CallStackSet;
  do {
    CallStackSet.insert(*CurrentRecord);
    auto It = &*llvm::find_if(CurrentRecord->Callee->callees(), IsPartOfSCC);
    CurrentRecord = &(*It);
    CallStack.push_back(*CurrentRecord);
  } while (!CallStackSet.contains(*CurrentRecord));
  return CallStack;
}

auto getCyclicCallStack(ArrayRef<CallGraphNode *> SCC) {
  auto CallStack = formCallStack(SCC);
  auto Cyclic = std::ranges::find(CallStack, CallStack.back(), &Record::Callee);
  CallStack.erase(CallStack.begin(), Cyclic);
  return CallStack;
}
} // namespace

void RecursiveFunctionCheck::registerMatchers(MatchFinder *Finder) {
  Finder->addMatcher(translationUnitDecl().bind("tu"), this);
}

void RecursiveFunctionCheck::check(const MatchFinder::MatchResult &Result) {
  const auto *TU = Result.Nodes.getNodeAs<TranslationUnitDecl>("tu");
  CallGraph CG;
  CG.addToCallGraph(const_cast<TranslationUnitDecl *>(TU));
  using namespace llvm;
  for (auto It = scc_begin(&CG), End = scc_end(&CG); It != End; ++It)
    if (It.hasCycle())
      for (const auto &CR : getCyclicCallStack(*It) | std::views::drop(1)) {
        auto *CallExpr = CR.CallExpr;
        if (!CallExpr)
          return;
        auto Loc = CallExpr->getBeginLoc();
        if (Loc.isInvalid())
          return;
        diag(Loc, "禁止递归");
      };
}
} // namespace clang::tidy::gjb8114
