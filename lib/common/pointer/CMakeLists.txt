target_sources(clang-tidy-gjb8114 PRIVATE
        PointerLevelCheck.cpp
        FunctionPointerAddressCheck.cpp
        ParameterPointerAssignCheck.cpp
        LocalVariableAddressReturnCheck.cpp
        InvalidPointerUseCheck.cpp
        FreePointerNullCheck.cpp
        InitPointerNullCheck.cpp
        MallocNullCheck.cpp
        ZeroPointerCheck.cpp
        FilePointerCloseCheck.cpp)