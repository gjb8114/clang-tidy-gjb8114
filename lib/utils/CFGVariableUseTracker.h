#pragma once

#include "clang-tidy/utils/ExprSequence.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/Analysis/CFG.h"
#include <memory>

namespace clang::tidy::gjb8114::utils {
class CFGVariableUseTracker {
public:
  explicit CFGVariableUseTracker(ASTContext *Context);
  virtual ~CFGVariableUseTracker() = default;

  void findUseAfter(llvm::SmallPtrSetImpl<const Stmt *> &Results,
                    Stmt *CodeBlock, const Stmt *TriggerExpr,
                    const ValueDecl *TrackedVar, CFG *TheCFG = nullptr) {
    findUse(Results, CodeBlock, TriggerExpr, TrackedVar, Dir::Forward, TheCFG);
  }
  void findUseBefore(llvm::SmallPtrSetImpl<const Stmt *> &Results,
                     Stmt *CodeBlock, const Stmt *TriggerExpr,
                     const ValueDecl *TrackedVar, CFG *TheCFG = nullptr) {
    findUse(Results, CodeBlock, TriggerExpr, TrackedVar, Dir::Backward, TheCFG);
  }
  enum class Dir {
    Forward,
    Backward,
  };
  void findUse(llvm::SmallPtrSetImpl<const Stmt *> &Results, Stmt *CodeBlock,
               const Stmt *TriggerExpr, const ValueDecl *TrackedVar,
               Dir Direction, CFG *TheCFG = nullptr);

protected:
  virtual ast_matchers::internal::BindableMatcher<Stmt>
  useMatcher(const ast_matchers::StatementMatcher &DeclRefMatcher) {
    return ast_matchers::stmt(DeclRefMatcher);
  }
  virtual ast_matchers::internal::BindableMatcher<Stmt>
  reinitMatcher(const ast_matchers::StatementMatcher &DeclRefMatcher) {
    using namespace ast_matchers;
    return binaryOperator(hasOperatorName("="), hasLHS(DeclRefMatcher));
  }

  ASTContext *Context;

  struct Reinit {
    const Stmt *Stmt;
    const DeclRefExpr *DeclRef;
  };

private:
  const CFGBlock *prepare(const Stmt *CodeBlock, const Stmt *TriggerExpr,
                          CFG &TheCFG);
  void findInternal(llvm::SmallPtrSetImpl<const Stmt *> &Result,
                    const CFGBlock *Block, const Stmt *TriggerExpr,
                    const ValueDecl *TrackedVar, Dir Direction);

  std::vector<const DeclRefExpr *> getDeclRefs(const CFGBlock *Block,
                                               const Decl *TrackedVar);

  std::vector<Reinit> getReinits(const CFGBlock *Block,
                                 const ValueDecl *TrackedVar);

  std::unique_ptr<clang::tidy::utils::ExprSequence> Sequence;
  std::unique_ptr<clang::tidy::utils::StmtToBlockMap> BlockMap;
  llvm::SmallPtrSet<const CFGBlock *, 8> Visited;
};
} // namespace clang::tidy::gjb8114::utils