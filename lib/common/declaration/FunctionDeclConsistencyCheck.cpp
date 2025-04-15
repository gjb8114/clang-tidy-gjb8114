#include "FunctionDeclConsistencyCheck.h"

#include <ranges>

using namespace clang::ast_matchers;

namespace clang::tidy::gjb8114 {
void FunctionDeclConsistencyCheck::registerMatchers(MatchFinder *Finder) {
  Finder->addMatcher(functionDecl(unless(isDefinition())).bind("decl"), this);
  Finder->addMatcher(functionDecl(isDefinition()).bind("def"), this);
}

void FunctionDeclConsistencyCheck::check(
    const MatchFinder::MatchResult &Result) {
  if (const auto *FD = Result.Nodes.getNodeAs<FunctionDecl>("decl")) {
    if (!Result.SourceManager->isInSystemHeader(FD->getLocation()))
      Declarations[FD->getNameAsString()].push_back(FD);
    return;
  }

  if (const auto *Def = Result.Nodes.getNodeAs<FunctionDecl>("def"))
    if (!Result.SourceManager->isInSystemHeader(Def->getLocation()))
      checkDeclsByDefine(*Result.Context, Def);
}

void FunctionDeclConsistencyCheck::checkDeclsByDefine(const ASTContext &Context,
                                                      const FunctionDecl *Def) {
  auto DeclIt = Declarations.find(Def->getNameAsString());
  if (DeclIt == Declarations.end())
    return;
  std::erase_if(DeclIt->second, [&](const auto *Decl) {
    return diagDeclConsistent(Context, Decl, Def);
  });
}

namespace {
auto compareParamsInType(const ASTContext &Context, const FunctionDecl *LHS,
                         const FunctionDecl *RHS) {
  auto SameNumParams = LHS->getNumParams() == RHS->getNumParams();
  if (!SameNumParams)
    return 0u;

  for (unsigned I = 0; I < LHS->getNumParams(); ++I) {
    if (!Context.hasSameType(LHS->getParamDecl(I)->getType(),
                             RHS->getParamDecl(I)->getType()))
      return I;
  }
  return LHS->getNumParams();
}
} // namespace

bool FunctionDeclConsistencyCheck::diagDeclConsistent(const ASTContext &Context,
                                                      const FunctionDecl *Decl,
                                                      const FunctionDecl *Def) {
  auto Opt = getReportOpt();
  const auto *Description = getDescription();

  bool SameNumParams = Decl->getNumParams() == Def->getNumParams();
  auto DiffParamI = compareParamsInType(Context, Decl, Def);
  auto SameParamsType = SameNumParams && DiffParamI == Decl->getNumParams();
  auto &Lang = Context.getLangOpts();
  if (bool NotOverload = !Lang.CPlusPlus || Lang.CPlusPlus && SameParamsType) {
    if (!Context.hasSameType(Decl->getReturnType(), Def->getReturnType())) {
      if (Opt.contains(Report::ReturnType)) {
        diag(Decl->getTypeSpecStartLoc(), Description)
            << Decl->getSourceRange();
        diag(Def->getTypeSpecStartLoc(), "返回值类型不同于此处的函数定义",
             DiagnosticIDs::Note);
        return true;
      }
    }
  }

  if (!Lang.CPlusPlus && !SameNumParams) {
    if (Opt.contains(Report::NumParameter)) {
      diag(Decl->getLocation(), Description) << Decl->getSourceRange();
      diag(Def->getLocation(), "参数数量不同于此处的函数定义",
           DiagnosticIDs::Note);
    }
    // fast fail here because the having same number of parameters is requisite
    // for the following rules
    return true;
  }

  if (!Lang.CPlusPlus && !SameParamsType) {
    if (Opt.contains(Report::ParameterType) && SameNumParams) {
      const auto *DeclParam = Decl->getParamDecl(DiffParamI);
      const auto *DefParam = Def->getParamDecl(DiffParamI);
      diag(DeclParam->getTypeSpecStartLoc(), Description)
          << Decl->getSourceRange();
      diag(DefParam->getTypeSpecStartLoc(), "参数类型不同于此处的函数定义",
           DiagnosticIDs::Note);
    }
    // fast fail here because the having same parameter types is requisite for
    // the following rules
    return true;
  }

  if (Opt.contains(Report::ParameterName) && SameNumParams) {
    for (unsigned I = 0; I < Decl->getNumParams(); ++I) {
      const auto *DeclParam = Decl->getParamDecl(I);
      const auto *DefParam = Def->getParamDecl(I);
      if (!DeclParam->getName().empty() && !DefParam->getName().empty() &&
          DeclParam->getName() != DefParam->getName()) {
        diag(DeclParam->getLocation(), Description) << Decl->getSourceRange();
        diag(DefParam->getLocation(), "参数名称不同于此处的函数定义",
             DiagnosticIDs::Note);
        return true;
      }
    }
  }
  return false;
}
} // namespace clang::tidy::gjb8114