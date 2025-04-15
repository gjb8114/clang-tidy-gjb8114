target_sources(clang-tidy-gjb8114 PRIVATE
        MissingReturnCheck.cpp
        StdFuncOverrideCheck.cpp
        FunctionArgumentTypeCheck.cpp
        FunctionCallArgMatchCheck.cpp
        KRStyleFunctionCheck.cpp
        VoidFunctionReturnCheck.cpp
        ReturnValueCheck.cpp
        ReturnTypeCheck.cpp
        UnusedReturnCheck.cpp
        UnusedStaticFunctionCheck.cpp
        VoidRedundantCastCheck.cpp
        OrderDependentCallsCheck.cpp
        VariadicFunctonCheck.cpp
        RecursiveFunctionCheck.cpp)