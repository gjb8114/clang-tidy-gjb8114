#include "NestedStructCheck.h"
#include "clang/AST/Decl.h"

using namespace clang::ast_matchers;

namespace clang::tidy::gjb8114 {
namespace {
bool isUsedInFields(const RecordDecl *Record,
                    const RecordDecl::field_range Fields) {
  return std::ranges::any_of(Fields, [=](const FieldDecl *Field) {
    auto *RecordOfFiled = Field->getType()->getAsRecordDecl();
    return RecordOfFiled && RecordOfFiled == Record;
  });
}

template <std::ranges::input_range Range>
std::optional<SourceLocation>
checkNestedStructsHasDeclAsField(const RecordDecl *Parent, Range &&Nesteds) {
  auto F = std::ranges::find_if(Nesteds, [=](const auto *Record) {
    return !isUsedInFields(Record, Parent->fields());
  });
  return F == Nesteds.end() ? std::nullopt
                            : std::make_optional((*F)->getLocation());
}
} // namespace

void NestedStructCheck::registerMatchers(MatchFinder *Finder) {
  auto NestedStructMatcher =
      recordDecl(isStruct(), hasDescendant(recordDecl(isStruct())))
          .bind("nestedStruct");
  Finder->addMatcher(NestedStructMatcher, this);
}

void NestedStructCheck::check(const MatchFinder::MatchResult &Result) {
  const auto *Struct = Result.Nodes.getNodeAs<RecordDecl>("nestedStruct");
  if (!Struct)
    return;

  std::vector<const RecordDecl *> Nesteds;
  for (const auto *D : Struct->decls()) {
    if (const auto *R = dyn_cast<RecordDecl>(D))
      Nesteds.push_back(R);
  }
  if (auto Loc = checkNestedStructsHasDeclAsField(Struct, Nesteds))
    diag(*Loc, "禁止结构体定义中含有匿名结构体");
}
} // namespace clang::tidy::gjb8114