#include "AccessDeallocCheck.h"

#include "utils/RawPointerUseTracker.h"

using namespace clang::ast_matchers;

namespace clang::tidy::gjb8114 {

void AccessDeallocCheck::registerMatchers(MatchFinder *Finder) {
  auto PointerVarDecl =
      varDecl(hasType(isAnyPointer()),
              hasAncestor(functionDecl(unless(isExpansionInSystemHeader()),
                                       isDefinition())
                              .bind("func")))
          .bind("ptrVar");
  auto PointerDeclRef = declRefExpr(to(PointerVarDecl));
  Finder->addMatcher(callExpr(callee(functionDecl(hasName("free"))),
                              hasArgument(0, PointerDeclRef))
                         .bind("free"),
                     this);
  Finder->addMatcher(
      cxxDeleteExpr(has(ignoringParenImpCasts(PointerDeclRef))).bind("delete"),
      this);
}

void AccessDeallocCheck::check(const MatchFinder::MatchResult &Result) {
  const Stmt *Free = Result.Nodes.getNodeAs<CallExpr>("free");
  const Stmt *Delete = Result.Nodes.getNodeAs<CXXDeleteExpr>("delete");
  const auto *Func = Result.Nodes.getNodeAs<FunctionDecl>("func");
  auto *Block = Func->getBody();
  auto *VD = Result.Nodes.getNodeAs<VarDecl>("ptrVar");
  llvm::SmallPtrSet<const Stmt *, 8> Usages;
  for (const auto *Trigger : {Free, Delete}) {
    if (Trigger) {
      utils::RawPointerUseTracker Tracker{Result.Context};
      Tracker.findUseAfter(Usages, Block, Trigger, VD);
    }
  }
  for (const auto *Usage : Usages)
    emitDiagnostics(Usage);
}

void AccessDeallocCheck::emitDiagnostics(const Stmt *Usage) {
  if (Reported.count(Usage))
    return;
  Reported.insert(Usage);
  const char *Description = "禁止使用已释放的内存";
  diag(Usage->getBeginLoc(), Description);
}
} // namespace clang::tidy::gjb8114
