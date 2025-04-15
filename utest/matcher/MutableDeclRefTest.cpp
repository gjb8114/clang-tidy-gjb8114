#include <gmock/gmock.h>

#include "helper/run.h"
#include "matchers/compose.h"

namespace clang::tidy::gjb8114::matchers {

auto mutableDeclRef() {
  using namespace ast_matchers;
  auto DeclRefMatcher =
      declRefExpr(to(varDecl(unless(hasType(qualType(isConstQualified()))))));
  return mutableDeclRef(DeclRefMatcher);
}
using namespace ::testing;

TEST(MutableDeclRefMatcherTest, MatchesMutableRef) {
  const auto *Code = R"cpp(
    void foo(int &);
    int main() {
      int y = 0;
      foo(y);
      return 0;
    })cpp";
  auto Matcher = mutableDeclRef().bind("mutableDeclRef");

  auto Matches = runMatcherOnCode(Code, Matcher);

  EXPECT_THAT(Matches, SizeIs(1));
}

TEST(MutableDeclRefMatcherTest, NotMatchesRefWithConst) {
  const auto *Code = R"cpp(
     void foo(const int &);
     int main() {
       int y = 0;
       foo(y);
       return 0;
     })cpp";
  auto Matcher = mutableDeclRef().bind("mutableDeclRef");

  auto Matches = runMatcherOnCode(Code, Matcher);

  EXPECT_THAT(Matches, IsEmpty());
}

TEST(MutableDeclRefMatcherTest, NotMatchesPassByValue) {
  const auto *Code = R"cpp(
      void foo(int);
      int main() {
        int y = 0;
        foo(y);
        return 0;
      })cpp";
  auto Matcher = mutableDeclRef().bind("mutableDeclRef");

  auto Matches = runMatcherOnCode(Code, Matcher);

  EXPECT_THAT(Matches, IsEmpty());
}

TEST(MutableDeclRefMatcherTest, MatchesMutablePointer) {
  const auto *Code = R"cpp(
    void foo(int *);
    int main() {
      int y = 0;
      foo(&y);
      return 0;
    })cpp";
  auto Matcher = mutableDeclRef().bind("mutableDeclRef");

  auto Matches = runMatcherOnCode(Code, Matcher);

  EXPECT_THAT(Matches, SizeIs(1));
}

TEST(MutableDeclRefMatcherTest, NotMatchesConstPointer) {
  const auto *Code = R"cpp(
      void foo(const int *);
      int main() {
        int y = 0;
        foo(&y);
        return 0;
      })cpp";
  auto Matcher = mutableDeclRef().bind("mutableDeclRef");

  auto Matches = runMatcherOnCode(Code, Matcher);

  EXPECT_THAT(Matches, IsEmpty());
}

TEST(MutableDeclRefMatcherTest, MatchesAssign) {
  const auto *Code = R"cpp(
        int main() {
        int y = 0;
        y = 1;
        return 0;
        }
        )cpp";
  auto Matcher = mutableDeclRef().bind("mutableDeclRef");

  auto Matches = runMatcherOnCode(Code, Matcher);

  EXPECT_THAT(Matches, SizeIs(1));
}

TEST(MutableDeclRefMatcherTest, NotMatchesCompare) {
  const auto *Code = R"cpp(
        int main() {
          int y = 0;
          if (1 == y) {
            return 0;
          }
          return 1;
        })cpp";
  auto Matcher = mutableDeclRef().bind("mutableDeclRef");

  auto Matches = runMatcherOnCode(Code, Matcher);

  EXPECT_THAT(Matches, IsEmpty());
}
} // namespace clang::tidy::gjb8114::matchers
