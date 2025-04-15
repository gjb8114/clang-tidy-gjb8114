#include "VirtualBaseDowncastCheck.h"

using namespace clang::ast_matchers;

namespace clang::tidy::gjb8114 {
static bool hasVirtualBaseClass(const CXXRecordDecl *DerivedClass,
                                const CXXRecordDecl *BaseClass) {
  for (const auto &VBase : DerivedClass->vbases()) {
    const Type *BaseType = VBase.getType().getTypePtr();
    if (!BaseType)
      continue;

    // Check if this virtual base type is the one we're looking for
    if (const auto *RT = BaseType->getAs<RecordType>()) {
      if (RT->getDecl() == BaseClass)
        return true;
    }

    // Recursively check base classes of this virtual base
    if (const auto *RecordDecl = BaseType->getAsCXXRecordDecl()) {
      if (hasVirtualBaseClass(RecordDecl, BaseClass))
        return true;
    }
  }

  // Check non-virtual inheritance path as well for transitive virtual
  // inheritance
  for (const auto &Base : DerivedClass->bases()) {
    if (Base.isVirtual())
      continue; // Already checked virtual bases

    const Type *BaseType = Base.getType().getTypePtr();
    if (!BaseType)
      continue;

    if (const auto *RecordDecl = BaseType->getAsCXXRecordDecl()) {
      if (hasVirtualBaseClass(RecordDecl, BaseClass))
        return true;
    }
  }

  return false;
}

void VirtualBaseDowncastCheck::registerMatchers(MatchFinder *Finder) {
  Finder->addMatcher(
      cxxReinterpretCastExpr(
          anyOf(hasDestinationType(pointerType(
                    pointee(hasDeclaration(cxxRecordDecl().bind("dstClass"))))),
                hasDestinationType(referenceType(pointee(
                    hasDeclaration(cxxRecordDecl().bind("dstClass")))))),
          hasSourceExpression(anyOf(
              // reference or pointer deref
              hasType(cxxRecordDecl().bind("srcClass")),
              // pointer to class
              hasType(pointerType(
                  pointee(hasDeclaration(cxxRecordDecl().bind("srcClass"))))))))
          .bind("reinterpretCast"),
      this);
}

void VirtualBaseDowncastCheck::check(const MatchFinder::MatchResult &Result) {
  const auto *CastExpr =
      Result.Nodes.getNodeAs<CXXReinterpretCastExpr>("reinterpretCast");
  const auto *DstClass = Result.Nodes.getNodeAs<CXXRecordDecl>("dstClass");
  const auto *SrcClass = Result.Nodes.getNodeAs<CXXRecordDecl>("srcClass");
  if (!CastExpr || !DstClass || !SrcClass)
    return;

  if (!DstClass->isDerivedFrom(SrcClass))
    return;

  if (hasVirtualBaseClass(DstClass, SrcClass))
    diag(CastExpr->getBeginLoc(), "禁止使用 reinterpret_cast 对虚基类向下转型");
}
} // namespace clang::tidy::gjb8114