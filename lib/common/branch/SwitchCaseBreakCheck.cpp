#include "SwitchCaseBreakCheck.h"
#include "matchers/predicate.h"
#include "clang/Lex/Lexer.h"

#include <ranges>

using namespace clang::ast_matchers;

namespace clang::tidy::gjb8114 {
void SwitchCaseBreakCheck::registerMatchers(MatchFinder *Finder) {
  Finder->addMatcher(
      switchStmt(unless(isExpansionInSystemHeader())).bind("switch"), this);
}

void SwitchCaseBreakCheck::check(const MatchFinder::MatchResult &Result) {
  auto *Switch = Result.Nodes.getNodeAs<SwitchStmt>("switch");
  if (!Switch)
    return;
  auto *Body = dyn_cast<CompoundStmt>(Switch->getBody());
  if (!Body)
    return;

  std::vector<SwitchCase *> Cases;
  std::vector<Stmt *> Ops;
  for (auto *Child : Body->body()) {
    if (isa<SwitchCase>(Child) && !Ops.empty()) {
      Ops.clear();
      if (!Cases.empty()) {
        const auto *Previous = Cases.back();
        Cases.pop_back();
        checkFallThrough(Result, Previous);
      }
    }
    if (auto *Default = dyn_cast<DefaultStmt>(Child)) {
      Cases.push_back(Default);
    } else if (auto *Case = dyn_cast<CaseStmt>(Child)) {
      auto *Nested = dyn_cast<CaseStmt>(Case->getSubStmt());
      while (Nested) {
        checkFallThrough(Result, Case);
        Case = Nested;
        Nested = dyn_cast<CaseStmt>(Case->getSubStmt());
      }
      Cases.push_back(Case);
    } else if (isa<BreakStmt>(Child)) {
      Ops.clear();
      if (!Cases.empty()) {
        Cases.pop_back();
      }
    } else {
      Ops.push_back(Child);
    }
  }

  for (auto *Case : Cases) {
    auto *CaseBody = Case->getSubStmt();
    if (isa<BreakStmt>(CaseBody) || isa<ReturnStmt>(CaseBody))
      continue;
    if (auto *Compound = dyn_cast<CompoundStmt>(CaseBody)) {
      if (!Compound->body_empty()) {
        const auto *Last = Compound->body_back();
        if (isa<BreakStmt>(Last) || isa<ReturnStmt>(Last))
          continue;
      }
    }
    checkFallThrough(Result, Case);
  }
}

void SwitchCaseBreakCheck::checkFallThrough(
    const MatchFinder::MatchResult &Result, const SwitchCase *Case) {
  if (Reported.contains(Case))
    return;

  const char *Description =
      "禁止 switch 中 case 和 default 未以 break 或 return 终止";
  const char *Note = "共用 case 必须随后加以 \"/* shared */\" 注释说明";
  bool HasSharedComment =
      predicate::hasComment(Case, *Result.SourceManager,
                            Result.Context->getLangOpts(), "/* shared */");
  if (!HasSharedComment) {
    Reported.insert(Case);
    diag(Case->getBeginLoc(), Description);
    diag(Case->getColonLoc(), Note, DiagnosticIDs::Note);
  }
}
} // namespace clang::tidy::gjb8114