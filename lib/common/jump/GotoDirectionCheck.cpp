#include "GotoDirectionCheck.h"
#include "matchers/matcher.h"

using namespace clang::ast_matchers;

namespace clang::tidy::gjb8114 {

void GotoDirectionCheck::registerMatchers(MatchFinder *Finder) {
  Finder->addMatcher(gotoStmt(unless(isExpansionInSystemHeader())).bind("goto"),
                     this);
}

void GotoDirectionCheck::check(const MatchFinder::MatchResult &Result) {
  const GotoStmt *Goto = Result.Nodes.getNodeAs<GotoStmt>("goto");
  if (!Goto)
    return;
  const LabelStmt *Label = Goto->getLabel()->getStmt();
  if (!Label)
    return;

  const char *Description = "禁止向内或向后 goto";
  auto &SM = *Result.SourceManager;
  bool IsBackward =
      SM.isBeforeInTranslationUnit(Label->getBeginLoc(), Goto->getBeginLoc());
  if (IsBackward) {
    diag(Goto->getLabelLoc(), Description);
    diag(Label->getBeginLoc(), "goto 到后方标签：", DiagnosticIDs::Note);
    return;
  }

  std::vector<const CompoundStmt *> GotoCompounds;
  const Stmt *GotoParent = Goto;
  while (GotoParent) {
    if (const auto *CS = dyn_cast<CompoundStmt>(GotoParent))
      GotoCompounds.push_back(CS);
    const auto Parents = Result.Context->getParents(*GotoParent);
    GotoParent = Parents.empty() ? nullptr : Parents[0].get<Stmt>();
  }

  // Check if the label is inside a compound statement that doesn't contain the
  // goto
  const Stmt *LabelParent = Label;
  while (LabelParent) {
    if (const auto *CS = dyn_cast<CompoundStmt>(LabelParent)) {
      if (std::find(GotoCompounds.begin(), GotoCompounds.end(), CS) ==
          GotoCompounds.end()) {
        diag(Goto->getLabelLoc(), Description);
        diag(Label->getBeginLoc(), "goto 到内层标签：", DiagnosticIDs::Note);
        return;
      }
    }
    const auto Parents = Result.Context->getParents(*LabelParent);
    LabelParent = Parents.empty() ? nullptr : Parents[0].get<Stmt>();
  }
}
} // namespace clang::tidy::gjb8114