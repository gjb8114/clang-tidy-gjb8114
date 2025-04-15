#include "BitFieldAlignCheck.h"
#include "clang/AST/ASTContext.h"

#include <ranges>

using namespace clang::ast_matchers;

namespace clang::tidy::gjb8114 {
namespace {
template <std::ranges::input_range Range>
auto checkSameTypeLength(Range &&Fields) {
  auto FirstField = Fields.front();
  auto FirstCanonicalType = FirstField->getType().getCanonicalType();
  auto FirstSize = FirstField->getASTContext().getTypeSize(FirstCanonicalType);

  std::vector<const FieldDecl *> Violates;
  auto Remains = Fields | std::views::drop(1);
  std::ranges::copy_if(
      Remains, std::back_inserter(Violates), [&](const FieldDecl *Field) {
        auto CanonicalType = Field->getType().getCanonicalType();
        auto Size = Field->getASTContext().getTypeSize(CanonicalType);
        return Size != FirstSize;
      });
  return Violates;
}

template <std::ranges::input_range Range>
auto checkBitFieldAlign(Range &&Fields, uint64_t Align) {
  std::vector<const FieldDecl *> Violates;
  auto Remain = Align;
  for (const auto *Field : Fields) {
    const auto *BitWidth = Field->getBitWidth();
    Expr::EvalResult ER;
    const auto &Context = Field->getASTContext();
    if (!BitWidth->EvaluateAsInt(ER, Context))
      continue;
    const auto BitWidthValue = ER.Val.getInt().getZExtValue();
    if (BitWidthValue > Remain)
      Violates.push_back(Field);

    auto Take = BitWidthValue % Align;
    if (Take < Remain)
      Remain -= Take;
    else if (Take > Remain)
      Remain = Remain + Align - Take;
    else
      Remain = Align;
  }
  return Violates;
}
} // namespace

void BitFieldAlignCheck::registerMatchers(MatchFinder *Finder) {
  Finder->addMatcher(recordDecl(has(fieldDecl(isBitField()))).bind("record"),
                     this);
}

void BitFieldAlignCheck::check(const MatchFinder::MatchResult &Result) {
  const auto *Record = Result.Nodes.getNodeAs<RecordDecl>("record");
  if (!Record)
    return;

  std::vector<const FieldDecl *> BitFields;
  std::ranges::copy_if(
      Record->fields(), std::back_inserter(BitFields),
      [](const FieldDecl *Field) { return Field->isBitField(); });
  if (BitFields.empty())
    return;

  const char *Description = "禁止位域基类型大小不一致或跨越基类型边界";
  if (BitFields.size() > 1) {
    auto Violates = checkSameTypeLength(BitFields);
    for (const auto *Field : Violates) {
      diag(Field->getTypeSpecStartLoc(), Description)
          << Field->getSourceRange();
      diag(BitFields.front()->getTypeSpecStartLoc(),
           "与之前的位域基类型大小不一致", DiagnosticIDs::Note);
    }
  }

  const auto *First = BitFields.front();
  auto AlignSize = First->getASTContext().getTypeSize(First->getType());
  auto AlignViolates = checkBitFieldAlign(BitFields, AlignSize);
  for (const auto *Field : AlignViolates) {
    diag(Field->getLocation(), Description) << Field->getSourceRange();
    diag(Field->getBitWidth()->getExprLoc(),
         std::string("跨越了基类型大小: %0 bits"), DiagnosticIDs::Note)
        << AlignSize;
  }
}
} // namespace clang::tidy::gjb8114