#pragma once
#include "clang-tidy/ClangTidyModule.h"
#include "clang-tidy/ClangTidyModuleRegistry.h"
#include "common/branch/BoolSwitchCheck.h"
#include "common/branch/CaseLevelCheck.h"
#include "common/branch/DefaultOnlySwitchCheck.h"
#include "common/branch/EmptyBranchCommentCheck.h"
#include "common/branch/EmptySwitchCheck.h"
#include "common/branch/IfElseIfBranchCheck.h"
#include "common/branch/SwitchCaseBreakCheck.h"
#include "common/branch/SwitchDefaultCheck.h"
#include "common/cast/DoubleToFloatAssignCheck.h"
#include "common/cast/FloatToIntVarAssignCheck.h"
#include "common/cast/LongToShortIntAssignCheck.h"
#include "common/cast/PointerAssignTypeCheck.h"
#include "common/cast/PointerNonPointerAssignCheck.h"
#include "common/cast/UselessCastCheck.h"
#include "common/comparison/BoolComparisonCheck.h"
#include "common/comparison/FloatComparisonCheck.h"
#include "common/comparison/IntegerComparisonCheck.h"
#include "common/comparison/PointerComparisonCheck.h"
#include "common/comparison/UnsignedComparisonCheck.h"
#include "common/declaration/AnonRecordCheck.h"
#include "common/declaration/ArrayExplicitBoundsCheck.h"
#include "common/declaration/BitFieldAlignCheck.h"
#include "common/declaration/BitFieldExplictSignedCheck.h"
#include "common/declaration/CharVariableSignCheck.h"
#include "common/declaration/DuplicateIncludeCheck.h"
#include "common/declaration/ExternDeclInFunctionCheck.h"
#include "common/declaration/ExternDeclInitCheck.h"
#include "common/declaration/FunctionDeclConsistencyCheck.h"
#include "common/declaration/FunctionDeclParamCheck.h"
#include "common/declaration/FunctionNoParamCheck.h"
#include "common/declaration/FunctionParamTypeCheck.h"
#include "common/declaration/IncludePathCheck.h"
#include "common/declaration/MacroKeywordCheck.h"
#include "common/declaration/MacroPairingCheck.h"
#include "common/declaration/MacroParenthesesCheck.h"
#include "common/declaration/MacroRedefCheck.h"
#include "common/declaration/MacroReplacementCheck.h"
#include "common/declaration/NestedStructCheck.h"
#include "common/declaration/SignedBitFieldCheck.h"
#include "common/declaration/StdlibIdentifierCheck.h"
#include "common/declaration/TypedefRedefCheck.h"
#include "common/formatting/AmbiguousComparisonCheck.h"
#include "common/formatting/AmbiguousLogicCheck.h"
#include "common/formatting/IfElseBracesCheck.h"
#include "common/formatting/IncludeLocationCheck.h"
#include "common/formatting/LoopBracesCheck.h"
#include "common/formatting/NestedCommentsCheck.h"
#include "common/function/FunctionArgumentTypeCheck.h"
#include "common/function/FunctionCallArgMatchCheck.h"
#include "common/function/FunctionReturnTypeCheck.h"
#include "common/function/KRStyleFunctionCheck.h"
#include "common/function/MissingReturnCheck.h"
#include "common/function/OrderDependentCallsCheck.h"
#include "common/function/ParamTypeMatchCheck.h"
#include "common/function/RecursiveFunctionCheck.h"
#include "common/function/ReturnTypeCheck.h"
#include "common/function/ReturnValueCheck.h"
#include "common/function/StdFuncOverrideCheck.h"
#include "common/function/UnusedReturnCheck.h"
#include "common/function/UnusedStaticFunctionCheck.h"
#include "common/function/VariadicFunctionCheck.h"
#include "common/function/VoidFunctionReturnCheck.h"
#include "common/function/VoidRedundantCastCheck.h"
#include "common/init/EnumInitializationCheck.h"
#include "common/init/StructInitializationCheck.h"
#include "common/init/SystemInitializedVariableCheck.h"
#include "common/init/UninitializedVariableCheck.h"
#include "common/jump/GotoDirectionCheck.h"
#include "common/jump/SetjmpLongjmpCheck.h"
#include "common/loop/ForScopeCheck.h"
#include "common/loop/ForTypeCheck.h"
#include "common/loop/ForVarMutateCheck.h"
#include "common/loop/InfiniteForCheck.h"
#include "common/operation/AccessDeallocCheck.h"
#include "common/operation/ArrayBoundsCheck.h"
#include "common/operation/ArrayIndexCheck.h"
#include "common/operation/BufferOverflowCheck.h"
#include "common/operation/DivisionByZeroCheck.h"
#include "common/operation/EnumUsageCheck.h"
#include "common/operation/FloatToIntAssignCheck.h"
#include "common/operation/FreeDeallocCheck.h"
#include "common/operation/GetsFunctionCheck.h"
#include "common/operation/IntegerOutOfBoundsCheck.h"
#include "common/operation/LogicalAssignmentCheck.h"
#include "common/operation/LogicalBitwiseCheck.h"
#include "common/operation/LogicalNotConstCheck.h"
#include "common/operation/NegativeShiftCheck.h"
#include "common/operation/ShiftLengthCheck.h"
#include "common/operation/SizeofAssignmentCheck.h"
#include "common/operation/StringBufferOverflowCheck.h"
#include "common/operation/UnaryOpInExprCheck.h"
#include "common/pointer/FilePointerCloseCheck.h"
#include "common/pointer/FreePointerNullCheck.h"
#include "common/pointer/FunctionPointerAddressCheck.h"
#include "common/pointer/InitPointerNullCheck.h"
#include "common/pointer/InvalidPointerUseCheck.h"
#include "common/pointer/LocalVariableAddressReturnCheck.h"
#include "common/pointer/MallocNullCheck.h"
#include "common/pointer/ParameterPointerAssignCheck.h"
#include "common/pointer/PointerLevelCheck.h"
#include "common/pointer/ZeroPointerCheck.h"
#include "common/statement/LowercaseSuffixCheck.h"
#include "common/statement/OctalNumberCheck.h"
#include "common/statement/UnreachableBranchCheck.h"
#include "common/statement/UnreachableStatementCheck.h"
#include "common/statement/UselessStatementCheck.h"
#include "common/variable/EnumVarNameConflictCheck.h"
#include "common/variable/GlobalVarNameConflictCheck.h"
#include "common/variable/IdentifierNameConflictCheck.h"
#include "common/variable/NullAsIntegerCheck.h"
#include "common/variable/NullTermStringCheck.h"
#include "common/variable/ParamNameConflictCheck.h"
#include "common/variable/SingleLetterVarNameCheck.h"
#include "common/variable/TypedefVarNameConflictCheck.h"
#include "common/variable/UnsignedNegAssignCheck.h"
#include "common/variable/VarFunctionNameConflictCheck.h"
#include "common/variable/VarNameCaseDistinctCheck.h"
#include "common/variable/VarNameLDistinctCheck.h"
#include "common/variable/VarNameODistinctCheck.h"
#include "common/variable/VariableShadowingCheck.h"
#include "common/variable/VolatileStmtCheck.h"
#include "cpp/class/MissingCopyCtorAndAssignCheck.h"
#include "cpp/class/VirtualBaseDowncastCheck.h"

namespace clang::tidy::gjb8114 {
class Module : public ClangTidyModule {
  void addCheckFactories(ClangTidyCheckFactories &Factories) override {
    Factories.registerCheck<MacroKeywordCheck>("gjb8114-r-1-1-1");
    Factories.registerCheck<MacroReplacementCheck>("gjb8114-r-1-1-2");
    Factories.registerCheck<TypedefRedefCheck>("gjb8114-r-1-1-3");
    Factories.registerCheck<StdlibIdentifierCheck>("gjb8114-r-1-1-4");
    Factories.registerCheck<MacroRedefCheck>("gjb8114-r-1-1-5");
    Factories.registerCheck<MacroPairingCheck>("gjb8114-r-1-1-6");
    Factories.registerCheck<MacroParenthesesCheck>("gjb8114-r-1-1-7");
    Factories.registerCheck<AnonRecordCheck>("gjb8114-r-1-1-8");
    Factories.registerCheck<NestedStructCheck>("gjb8114-r-1-1-9");
    Factories.registerCheck<SignedBitFieldCheck>("gjb8114-r-1-1-10");
    Factories.registerCheck<BitFieldExplictSignedCheck>("gjb8114-r-1-1-11");
    Factories.registerCheck<BitFieldAlignCheck>("gjb8114-r-1-1-12");
    Factories.registerCheck<FunctionDeclParamCheck>("gjb8114-r-1-1-13");
    Factories.registerCheck<FunctionDeclConsistencyCheck>("gjb8114-r-1-1-14");
    Factories.registerCheck<FunctionParamTypeCheck>("gjb8114-r-1-1-15");
    Factories.registerCheck<ExternDeclInFunctionCheck>("gjb8114-r-1-1-17");
    Factories.registerCheck<ArrayExplicitBoundsCheck>("gjb8114-r-1-1-18");
    Factories.registerCheck<ExternDeclInitCheck>("gjb8114-r-1-1-19");
    Factories.registerCheck<CharVariableSignCheck>("gjb8114-r-1-1-20");
    Factories.registerCheck<IncludePathCheck>("gjb8114-r-1-1-21");
    Factories.registerCheck<DuplicateIncludeCheck>("gjb8114-r-1-1-22");
    Factories.registerCheck<FunctionNoParamCheck>("gjb8114-r-1-1-23");
    Factories.registerCheck<LoopBracesCheck>("gjb8114-r-1-2-1");
    Factories.registerCheck<IfElseBracesCheck>("gjb8114-r-1-2-2");
    Factories.registerCheck<IncludeLocationCheck>("gjb8114-r-1-2-3");
    Factories.registerCheck<AmbiguousLogicCheck>("gjb8114-r-1-2-4");
    Factories.registerCheck<AmbiguousComparisonCheck>("gjb8114-r-1-2-5");
    Factories.registerCheck<NestedCommentsCheck>("gjb8114-r-1-2-6");
    Factories.registerCheck<PointerLevelCheck>("gjb8114-r-1-3-1");
    Factories.registerCheck<FunctionPointerAddressCheck>("gjb8114-r-1-3-2");
    Factories.registerCheck<ParameterPointerAssignCheck>("gjb8114-r-1-3-3");
    Factories.registerCheck<LocalVariableAddressReturnCheck>("gjb8114-r-1-3-4");
    Factories.registerCheck<InvalidPointerUseCheck>("gjb8114-r-1-3-5");
    Factories.registerCheck<FreePointerNullCheck>("gjb8114-r-1-3-6");
    Factories.registerCheck<InitPointerNullCheck>("gjb8114-r-1-3-7");
    Factories.registerCheck<MallocNullCheck>("gjb8114-r-1-3-8");
    Factories.registerCheck<ZeroPointerCheck>("gjb8114-r-1-3-9");
    Factories.registerCheck<FilePointerCloseCheck>("gjb8114-r-1-3-10");
    Factories.registerCheck<IfElseIfBranchCheck>("gjb8114-r-1-4-1");
    Factories.registerCheck<EmptyBranchCommentCheck>("gjb8114-r-1-4-2");
    Factories.registerCheck<EmptySwitchCheck>("gjb8114-r-1-4-3");
    Factories.registerCheck<BoolSwitchCheck>("gjb8114-r-1-4-4");
    Factories.registerCheck<DefaultOnlySwitchCheck>("gjb8114-r-1-4-5");
    Factories.registerCheck<SwitchDefaultCheck>("gjb8114-r-1-4-6");
    Factories.registerCheck<SwitchCaseBreakCheck>("gjb8114-r-1-4-7");
    Factories.registerCheck<CaseLevelCheck>("gjb8114-r-1-4-8");
    Factories.registerCheck<GotoDirectionCheck>("gjb8114-r-1-5-1");
    Factories.registerCheck<SetjmpLongjmpCheck>("gjb8114-r-1-5-2");
    Factories.registerCheck<FloatToIntAssignCheck>("gjb8114-r-1-6-1");
    Factories.registerCheck<IntegerOutOfBoundsCheck>("gjb8114-r-1-6-2");
    Factories.registerCheck<LogicalAssignmentCheck>("gjb8114-r-1-6-3");
    Factories.registerCheck<LogicalBitwiseCheck>("gjb8114-r-1-6-4");
    Factories.registerCheck<UnaryOpInExprCheck>("gjb8114-r-1-6-5");
    Factories.registerCheck<ShiftLengthCheck>("gjb8114-r-1-6-6");
    Factories.registerCheck<NegativeShiftCheck>("gjb8114-r-1-6-7");
    Factories.registerCheck<ArrayBoundsCheck>("gjb8114-r-1-6-8");
    Factories.registerCheck<ArrayIndexCheck>("gjb8114-r-1-6-9");
    Factories.registerCheck<LogicalNotConstCheck>("gjb8114-r-1-6-10");
    Factories.registerCheck<EnumUsageCheck>("gjb8114-r-1-6-11");
    Factories.registerCheck<DivisionByZeroCheck>("gjb8114-r-1-6-12");
    Factories.registerCheck<SizeofAssignmentCheck>("gjb8114-r-1-6-13");
    Factories.registerCheck<BufferReadCheck>("gjb8114-r-1-6-14");
    Factories.registerCheck<BufferWriteCheck>("gjb8114-r-1-6-15");
    Factories.registerCheck<AccessDeallocCheck>("gjb8114-r-1-6-16");
    Factories.registerCheck<FreeDeallocCheck>("gjb8114-r-1-6-17");
    Factories.registerCheck<GetsFunctionCheck>("gjb8114-r-1-6-18");
    Factories.registerCheck<StringBufferOverflowCheck>("gjb8114-r-1-6-19");
    Factories.registerCheck<StdFuncOverrideCheck>("gjb8114-r-1-7-1");
    Factories.registerCheck<FunctionArgumentTypeCheck>("gjb8114-r-1-7-2");
    Factories.registerCheck<FunctionCallArgMatchCheck>("gjb8114-r-1-7-3");
    Factories.registerCheck<KRStyleFunctionCheck>("gjb8114-r-1-7-4");
    Factories.registerCheck<ParamTypeMatchCheck>("gjb8114-r-1-7-5");
    Factories.registerCheck<FunctionReturnTypeCheck>("gjb8114-r-1-7-6");
    Factories.registerCheck<MissingReturnCheck>("gjb8114-r-1-7-7");
    Factories.registerCheck<VoidFunctionReturnCheck>("gjb8114-r-1-7-8");
    Factories.registerCheck<ReturnValueCheck>("gjb8114-r-1-7-9");
    Factories.registerCheck<ReturnTypeCheck>("gjb8114-r-1-7-10");
    Factories.registerCheck<UnusedReturnCheck>("gjb8114-r-1-7-11");
    Factories.registerCheck<VoidRedundantCastCheck>("gjb8114-r-1-7-12");
    Factories.registerCheck<UnusedStaticFunctionCheck>("gjb8114-r-1-7-13");
    Factories.registerCheck<OrderDependentCallsCheck>("gjb8114-r-1-7-14");
    Factories.registerCheck<VariadicFunctonCheck>("gjb8114-r-1-7-15");
    Factories.registerCheck<RecursiveFunctionCheck>("gjb8114-r-1-7-16");
    Factories.registerCheck<UnreachableStatementCheck>("gjb8114-r-1-8-1");
    Factories.registerCheck<UnreachableBranchCheck>("gjb8114-r-1-8-2");
    Factories.registerCheck<UselessStatementCheck>("gjb8114-r-1-8-3");
    Factories.registerCheck<OctalNumberCheck>("gjb8114-r-1-8-4");
    Factories.registerCheck<LowercaseSuffixCheck>("gjb8114-r-1-8-5");
    Factories.registerCheck<ForScopeCheck>("gjb8114-r-1-9-1");
    Factories.registerCheck<ForTypeCheck>("gjb8114-r-1-9-2");
    Factories.registerCheck<ForVarMutateCheck>("gjb8114-r-1-9-3");
    Factories.registerCheck<InfiniteForCheck>("gjb8114-r-1-9-4");
    Factories.registerCheck<FloatToIntVarAssignCheck>("gjb8114-r-1-10-1");
    Factories.registerCheck<LongToShortIntAssignCheck>("gjb8114-r-1-10-2");
    Factories.registerCheck<DoubleToFloatAssignCheck>("gjb8114-r-1-10-3");
    Factories.registerCheck<PointerAssignTypeCheck>("gjb8114-r-1-10-4");
    Factories.registerCheck<PointerNonPointerAssignCheck>("gjb8114-r-1-10-5");
    Factories.registerCheck<UselessCastCheck>("gjb8114-r-1-10-6");
    Factories.registerCheck<UninitializedVariableCheck>("gjb8114-r-1-11-1");
    Factories.registerCheck<SystemInitializedVariableCheck>("gjb8114-r-1-11-2");
    Factories.registerCheck<StructInitializationCheck>("gjb8114-r-1-11-3");
    Factories.registerCheck<EnumInitializationCheck>("gjb8114-r-1-11-4");
    Factories.registerCheck<BoolComparisonCheck>("gjb8114-r-1-12-1");
    Factories.registerCheck<PointerComparisonCheck>("gjb8114-r-1-12-2");
    Factories.registerCheck<FloatComparisonCheck>("gjb8114-r-1-12-3");
    Factories.registerCheck<UnsignedComparisonCheck>("gjb8114-r-1-12-4");
    Factories.registerCheck<IntegerComparisonCheck>("gjb8114-r-1-12-5");
    Factories.registerCheck<GlobalVarNameConflictCheck>("gjb8114-r-1-13-1");
    Factories.registerCheck<ParamNameConflictCheck>("gjb8114-r-1-13-2");
    Factories.registerCheck<VarFunctionNameConflictCheck>("gjb8114-r-1-13-3");
    Factories.registerCheck<IdentifierNameConflictCheck>("gjb8114-r-1-13-4");
    Factories.registerCheck<EnumVarNameConflictCheck>("gjb8114-r-1-13-5");
    Factories.registerCheck<TypedefVarNameConflictCheck>("gjb8114-r-1-13-6");
    Factories.registerCheck<VariableShadowingCheck>("gjb8114-r-1-13-7");
    Factories.registerCheck<VarNameCaseDistinctCheck>("gjb8114-r-1-13-8");
    Factories.registerCheck<VarNameLDistinctCheck>("gjb8114-r-1-13-9");
    Factories.registerCheck<VarNameODistinctCheck>("gjb8114-r-1-13-10");
    Factories.registerCheck<SingleLetterVarNameCheck>("gjb8114-r-1-13-11");
    Factories.registerCheck<VolatileStmtCheck>("gjb8114-r-1-13-13");
    Factories.registerCheck<NullAsIntegerCheck>("gjb8114-r-1-13-14");
    Factories.registerCheck<UnsignedNegAssignCheck>("gjb8114-r-1-13-15");
    Factories.registerCheck<NullTermStringCheck>("gjb8114-r-1-13-16");

    // C++ specific checks
    Factories.registerCheck<MissingCopyCtorAndAssignCheck>("gjb8114-r-2-1-1");
    Factories.registerCheck<VirtualBaseDowncastCheck>("gjb8114-r-2-1-2");
  }
};
} // namespace clang::tidy::gjb8114
