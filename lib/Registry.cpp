#include "Module.h"

#include "clang-tidy/ClangTidyModuleRegistry.h"

namespace clang::tidy {
namespace gjb8114 {
static ClangTidyModuleRegistry::Add<Module> X("gjb8114-module",
                                              "Adds GJB8114-2013 checks.");
} // namespace gjb8114

// This anchor is used to force the linker to link in the generated object file
// and thus register the module.
[[maybe_unused]] volatile int Gjb8114ModuleAnchorSource = 0;
} // namespace clang::tidy
