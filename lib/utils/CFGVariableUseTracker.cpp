#include "CFGVariableUseTracker.h"
#include "clang-tidy/utils/Matchers.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang;
using namespace clang::ast_matchers;
using namespace clang::tidy::utils;

// Matches nodes that are
// - Part of a decltype argument or class template argument (we check this by
//   seeing if they are children of a TypeLoc), or
// - Part of a function template argument (we check this by seeing if they are
//   children of a DeclRefExpr that references a function template).
// DeclRefExprs that fulfill these conditions should not be counted as a use or
// move.
static StatementMatcher inDecltypeOrTemplateArg() {
  using clang::tidy::matchers::hasUnevaluatedContext;
  return anyOf(hasAncestor(typeLoc()),
               hasAncestor(declRefExpr(
                   to(functionDecl(ast_matchers::isTemplateInstantiation())))),
               hasAncestor(expr(hasUnevaluatedContext())));
}

namespace clang::tidy::gjb8114::utils {
namespace {
auto buildCFG(ASTContext *Context, Stmt *CodeBlock) {
  CFG::BuildOptions Options;
  Options.AddImplicitDtors = true;
  Options.AddTemporaryDtors = true;
  return CFG::buildCFG(nullptr, CodeBlock, Context, Options);
}
} // namespace

CFGVariableUseTracker::CFGVariableUseTracker(ASTContext *Ctx) : Context(Ctx) {}

const CFGBlock *CFGVariableUseTracker::prepare(const Stmt *CodeBlock,
                                               const Stmt *TriggerExpr,
                                               CFG &TheCFG) {
  Sequence = std::make_unique<ExprSequence>(&TheCFG, CodeBlock, Context);
  BlockMap = std::make_unique<StmtToBlockMap>(&TheCFG, Context);
  Visited.clear();

  const CFGBlock *TriggerBlock = BlockMap->blockContainingStmt(TriggerExpr);
  if (!TriggerBlock)
    TriggerBlock = &TheCFG.getEntry();
  return TriggerBlock;
}

void CFGVariableUseTracker::findUse(
    llvm::SmallPtrSetImpl<const Stmt *> &Results, Stmt *CodeBlock,
    const Stmt *TriggerExpr, const ValueDecl *TrackedVar, Dir Direction,
    CFG *TheCFG) {
  std::unique_ptr<CFG> OwnCFG;
  if (TheCFG == nullptr)
    OwnCFG = buildCFG(Context, CodeBlock);
  auto *UsingCFG = TheCFG ? TheCFG : OwnCFG.get();
  if (UsingCFG) {
    auto *TriggerBlock = prepare(CodeBlock, TriggerExpr, *UsingCFG);
    findInternal(Results, TriggerBlock, TriggerExpr, TrackedVar, Direction);
  }
  return;
}

void CFGVariableUseTracker::findInternal(
    llvm::SmallPtrSetImpl<const Stmt *> &Result, const CFGBlock *Block,
    const Stmt *TriggerExpr, const ValueDecl *TrackedVar, Dir Direction) {
  if (Visited.count(Block))
    return;
  Visited.insert(Block);

  auto Uses = getDeclRefs(Block, TrackedVar);
  auto Reinits = getReinits(Block, TrackedVar);
  std::set<const DeclRefExpr *> ReinitDeclRefs;
  for (auto &Reinit : Reinits)
    ReinitDeclRefs.insert(Reinit.DeclRef);
  std::erase_if(Uses, [&ReinitDeclRefs](auto *DeclRef) {
    return ReinitDeclRefs.count(DeclRef);
  });

  std::erase_if(Reinits, [this, TriggerExpr](auto &Reinit) {
    return TriggerExpr && Reinit.Stmt != TriggerExpr &&
           Sequence->potentiallyAfter(TriggerExpr, Reinit.Stmt);
  });

  auto InRange = [&](const auto *Use) {
    if (TriggerExpr) {
      if (Direction == Dir::Forward
              ? !Sequence->potentiallyAfter(Use, TriggerExpr)
              : !Sequence->potentiallyAfter(TriggerExpr, Use))
        return false;
    }
    bool UseAfterReinit =
        std::ranges::any_of(Reinits, [this, Use](auto &Reinit) {
          return !Sequence->potentiallyAfter(Reinit.Stmt, Use);
        });
    return !UseAfterReinit;
  };
  std::ranges::copy_if(Uses, std::inserter(Result, Result.begin()), InRange);

  if (!Reinits.empty())
    return;

  auto Next = Direction == Dir::Forward ? Block->succs() : Block->preds();
  for (const auto &N : Next)
    if (N)
      findInternal(Result, N, nullptr, TrackedVar, Direction);
}

std::vector<const DeclRefExpr *>
CFGVariableUseTracker::getDeclRefs(const CFGBlock *Block,
                                   const Decl *TrackedVar) {
  std::vector<const DeclRefExpr *> DeclRefs;
  for (const auto &Elem : *Block) {
    std::optional<CFGStmt> CS = Elem.getAs<CFGStmt>();
    if (!CS)
      continue;

    auto AddDeclRefs = [this, Block,
                        &DeclRefs](const ArrayRef<BoundNodes> Matches) {
      for (const auto &Match : Matches) {
        const auto *DRE = Match.getNodeAs<DeclRefExpr>("declref");
        if (DRE && BlockMap->blockContainingStmt(DRE) == Block)
          DeclRefs.push_back(DRE);
      }
    };

    auto DeclRefMatcher = declRefExpr(hasDeclaration(equalsNode(TrackedVar)),
                                      unless(inDecltypeOrTemplateArg()))
                              .bind("declref");
    auto UseMatcher = useMatcher(DeclRefMatcher);
    AddDeclRefs(match(findAll(UseMatcher), *CS->getStmt(), *Context));
  }
  return DeclRefs;
}

std::vector<CFGVariableUseTracker::Reinit>
CFGVariableUseTracker::getReinits(const CFGBlock *Block,
                                  const ValueDecl *TrackedVar) {
  std::vector<CFGVariableUseTracker::Reinit> Result;
  auto DeclRefMatcher =
      declRefExpr(hasDeclaration(equalsNode(TrackedVar))).bind("declref");
  auto ReinitMatcher = reinitMatcher(DeclRefMatcher).bind("reinit");

  for (const auto &Elem : *Block) {
    auto CS = Elem.getAs<CFGStmt>();
    if (!CS)
      continue;

    llvm::SmallVector<BoundNodes, 1> Matches =
        match(findAll(ReinitMatcher), *CS->getStmt(), *Context);
    for (const auto &Match : Matches) {
      const auto *TheStmt = Match.getNodeAs<Stmt>("reinit");
      const auto *DRE = Match.getNodeAs<DeclRefExpr>("declref");
      if (TheStmt && BlockMap->blockContainingStmt(TheStmt) == Block)
        Result.emplace_back(TheStmt, DRE);
    }
  }
  return Result;
}
} // namespace clang::tidy::gjb8114::utils