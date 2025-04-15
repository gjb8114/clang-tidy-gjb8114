#pragma once

#include "clang/ASTMatchers/ASTMatchers.h"

#include "predicate.h"

namespace clang::tidy::gjb8114 {
AST_MATCHER(Expr, isNull) { return predicate::isNull(Node); }

AST_MATCHER(FunctionDecl, isMemcpy) {
  auto Id = Node.getBuiltinID();
  return Id == Builtin::BImemcpy || Id == Builtin::BI__builtin_memcpy_inline ||
         Id == Builtin::BI__builtin_memcpy ||
         Id == Builtin::BI__builtin___memcpy_chk;
}

AST_MATCHER(FunctionDecl, isStrncpy) {
  auto Id = Node.getBuiltinID();
  return Id == Builtin::BIstrncpy || Id == Builtin::BI__builtin_strncpy ||
         Id == Builtin::BI__builtin___strncpy_chk;
}

AST_MATCHER(FunctionDecl, isStrncat) {
  auto Id = Node.getBuiltinID();
  return Id == Builtin::BIstrncat || Id == Builtin::BI__builtin_strncat ||
         Id == Builtin::BI__builtin___strncat_chk;
}

AST_MATCHER(Expr, isLValue) { return Node.isLValue(); }
} // namespace clang::tidy::gjb8114