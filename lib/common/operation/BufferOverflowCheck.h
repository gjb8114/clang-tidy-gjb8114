#pragma once
#include "clang-tidy/ClangTidyCheck.h"

namespace clang::tidy::gjb8114 {
class BufferOverflowCheck : public ClangTidyCheck {
public:
  BufferOverflowCheck(llvm::StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}
  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;

protected:
  virtual bool shouldCheckRead() const { return true; };
  virtual bool shouldCheckWrite() const { return true; };
};

class BufferReadCheck : public BufferOverflowCheck {
public:
  BufferReadCheck(llvm::StringRef Name, ClangTidyContext *Context)
      : BufferOverflowCheck(Name, Context) {}
  bool shouldCheckWrite() const override { return false; }
};

class BufferWriteCheck : public BufferOverflowCheck {
public:
  BufferWriteCheck(llvm::StringRef Name, ClangTidyContext *Context)
      : BufferOverflowCheck(Name, Context) {}
  bool shouldCheckRead() const override { return false; }
};
} // namespace clang::tidy::gjb8114