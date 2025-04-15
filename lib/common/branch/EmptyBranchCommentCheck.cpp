#include "EmptyBranchCommentCheck.h"
#include "matchers/predicate.h"

using namespace clang::ast_matchers;

namespace clang::tidy::gjb8114 {
void EmptyBranchCommentCheck::registerMatchers(MatchFinder *Finder) {
  auto PureNullCompoundStmt = []() {
    return compoundStmt(has(nullStmt().bind("nullStmt")),
                        unless(has(stmt(unless(nullStmt())))));
  };
  Finder->addMatcher(
      ifStmt(unless(isExpansionInSystemHeader()),
             hasThen(anyOf(nullStmt().bind("emptyThen"),
                           compoundStmt(statementCountIs(0)).bind("emptyThen"),
                           PureNullCompoundStmt().bind("emptyThen")))),
      this);
  Finder->addMatcher(
      ifStmt(unless(isExpansionInSystemHeader()),
             hasElse(anyOf(nullStmt().bind("emptyElse"),
                           compoundStmt(statementCountIs(0)).bind("emptyElse"),
                           PureNullCompoundStmt().bind("emptyElse")))),
      this);
}

void EmptyBranchCommentCheck::check(const MatchFinder::MatchResult &Result) {
  const auto *EmptyElse = Result.Nodes.getNodeAs<Stmt>("emptyElse");
  const auto *EmptyThen = Result.Nodes.getNodeAs<Stmt>("emptyThen");

  SourceManager &SM = *Result.SourceManager;
  const LangOptions &LangOpts = Result.Context->getLangOpts();
  for (const auto *EmptyBranch : {EmptyThen, EmptyElse})
    if (EmptyBranch) {
      const Stmt *NS = dyn_cast<NullStmt>(EmptyBranch);
      if (NS == nullptr)
        NS = Result.Nodes.getNodeAs<Stmt>("nullStmt");
      if (NS && predicate::hasComment(EmptyBranch, SM, LangOpts,
                                      "/* no deal with */"))
        return;
      auto L = NS ? NS->getBeginLoc() : EmptyBranch->getBeginLoc();
      diag(L, "禁止条件分支为空");
      const char *NoteMessage =
          "空分支必须以单独一行的分号空语句并随后加注释: '/* no deal with */'";
      diag(L, NoteMessage, DiagnosticIDs::Note);
    }
}
} // namespace clang::tidy::gjb8114