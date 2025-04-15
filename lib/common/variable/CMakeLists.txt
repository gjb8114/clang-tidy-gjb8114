target_sources(clang-tidy-gjb8114 PRIVATE
        GlobalVarNameConflictCheck.cpp
        ParamNameConflictCheck.cpp
        VarFunctionNameConflictCheck.cpp
        IdentifierNameConflictCheck.cpp
        EnumVarNameConflictCheck.cpp
        TypedefVarNameConflictCheck.cpp
        UnsignedNegAssignCheck.cpp
        NullAsIntegerCheck.cpp
        VariableShadowingCheck.cpp
        VarNameDistinctCheck.cpp
        SingleLetterVarNameCheck.cpp
        VolatileStmtCheck.cpp
        NullTermStringCheck.cpp)
