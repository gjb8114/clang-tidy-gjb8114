#include "StdFuncOverrideCheck.h"

using namespace clang::ast_matchers;

namespace clang::tidy::gjb8114 {
void StdFuncOverrideCheck::registerMatchers(MatchFinder *Finder) {
  Finder->addMatcher(
      functionDecl(
          unless(isExpansionInSystemHeader()), isDefinition(),
          anyOf(
              // <stdio.h>
              hasName("::remove"), hasName("::rename"), hasName("::tmpfile"),
              hasName("::tmpnam"), hasName("::fclose"), hasName("::fflush"),
              hasName("::fopen"), hasName("::freopen"), hasName("::setbuf"),
              hasName("::setvbuf"), hasName("::fprintf"), hasName("::fscanf"),
              hasName("::printf"), hasName("::scanf"),
              hasName("::snprintf"), // C99
              hasName("::sprintf"), hasName("::sscanf"), hasName("::vfprintf"),
              hasName("::vfscanf"), hasName("::vprintf"), hasName("::vscanf"),
              hasName("::vsnprintf"), // C99
              hasName("::vsprintf"), hasName("::vsscanf"), hasName("::fgetc"),
              hasName("::fgets"), hasName("::fputc"), hasName("::fputs"),
              hasName("::getc"), hasName("::getchar"),
              hasName("::gets"), // Deprecated/Removed
              hasName("::putc"), hasName("::putchar"), hasName("::puts"),
              hasName("::ungetc"), hasName("::fread"), hasName("::fwrite"),
              hasName("::fgetpos"), hasName("::fseek"), hasName("::fsetpos"),
              hasName("::ftell"), hasName("::rewind"), hasName("::clearerr"),
              hasName("::feof"), hasName("::ferror"), hasName("::perror"),
              // <stdlib.h>
              hasName("::atof"), hasName("::atoi"), hasName("::atol"),
              hasName("::atoll"), // C99
              hasName("::strtod"),
              hasName("::strtof"),  // C99
              hasName("::strtold"), // C99
              hasName("::strtol"),
              hasName("::strtoll"), // C99
              hasName("::strtoul"),
              hasName("::strtoull"), // C99
              hasName("::rand"), hasName("::srand"),
              hasName("::aligned_alloc"), // C11
              hasName("::calloc"), hasName("::free"), hasName("::malloc"),
              hasName("::realloc"), hasName("::abort"), hasName("::atexit"),
              hasName("::at_quick_exit"), // C11
              hasName("::exit"), hasName("::getenv"),
              hasName("::quick_exit"), // C11
              hasName("::system"),
              hasName("::_Exit"), // C99
              hasName("::bsearch"), hasName("::qsort"), hasName("::abs"),
              hasName("::labs"),
              hasName("::llabs"), // C99
              hasName("::div"), hasName("::ldiv"),
              hasName("::lldiv"), // C99
              hasName("::mblen"), hasName("::mbtowc"), hasName("::wctomb"),
              hasName("::mbstowcs"), hasName("::wcstombs"),
              // <string.h>
              hasName("::strcpy"), hasName("::strncpy"), hasName("::strcat"),
              hasName("::strncat"), hasName("::strcmp"), hasName("::strncmp"),
              hasName("::strcoll"), hasName("::strxfrm"), hasName("::strchr"),
              hasName("::strrchr"), hasName("::strcspn"), hasName("::strspn"),
              hasName("::strpbrk"), hasName("::strstr"), hasName("::strtok"),
              hasName("::memcpy"), hasName("::memmove"), hasName("::memcmp"),
              hasName("::memchr"), hasName("::memset"), hasName("::strerror"),
              hasName("::strlen"),
              // <math.h> (Common functions, excluding many C99/C11 additions
              // for brevity, focusing on double versions) Note: Many C99
              // additions like fabsf, sinf, sinl etc. exist
              hasName("::sin"), hasName("::cos"), hasName("::tan"),
              hasName("::asin"), hasName("::acos"), hasName("::atan"),
              hasName("::atan2"), hasName("::sinh"), hasName("::cosh"),
              hasName("::tanh"),
              hasName("::asinh"), // C99
              hasName("::acosh"), // C99
              hasName("::atanh"), // C99
              hasName("::exp"),
              hasName("::exp2"),  // C99
              hasName("::expm1"), // C99
              hasName("::frexp"), hasName("::ldexp"), hasName("::log"),
              hasName("::log10"),
              hasName("::log2"),  // C99
              hasName("::log1p"), // C99
              hasName("::ilogb"), // C99
              hasName("::logb"),  // C99
              hasName("::modf"),
              hasName("::scalbn"),  // C99
              hasName("::scalbln"), // C99
              hasName("::pow"), hasName("::sqrt"),
              hasName("::cbrt"),   // C99
              hasName("::hypot"),  // C99
              hasName("::erf"),    // C99
              hasName("::erfc"),   // C99
              hasName("::tgamma"), // C99
              hasName("::lgamma"), // C99
              hasName("::ceil"), hasName("::floor"), hasName("::fabs"),
              hasName("::fmod"),
              hasName("::trunc"),      // C99
              hasName("::round"),      // C99
              hasName("::lround"),     // C99
              hasName("::llround"),    // C99
              hasName("::nearbyint"),  // C99
              hasName("::rint"),       // C99
              hasName("::lrint"),      // C99
              hasName("::llrint"),     // C99
              hasName("::remainder"),  // C99
              hasName("::remquo"),     // C99
              hasName("::copysign"),   // C99
              hasName("::nan"),        // C99
              hasName("::nextafter"),  // C99
              hasName("::nexttoward"), // C99
              hasName("::fdim"),       // C99
              hasName("::fmax"),       // C99
              hasName("::fmin"),       // C99
              hasName("::fma"),        // C99
              // Note: fpclassify, isfinite, isinf, isnan, isnormal, signbit are
              // often macros but must have function equivalents
              hasName("::fpclassify"), // C99
              hasName("::isfinite"),   // C99
              hasName("::isinf"),      // C99
              hasName("::isnan"),      // C99
              hasName("::isnormal"),   // C99
              hasName("::signbit"),    // C99
              // <ctype.h> (These can often be macros too, but must have
              // function equivalents)
              hasName("::isalnum"), hasName("::isalpha"),
              hasName("::isblank"), // C99
              hasName("::iscntrl"), hasName("::isdigit"), hasName("::isgraph"),
              hasName("::islower"), hasName("::isprint"), hasName("::ispunct"),
              hasName("::isspace"), hasName("::isupper"), hasName("::isxdigit"),
              hasName("::tolower"), hasName("::toupper"),
              // <time.h>
              hasName("::clock"), hasName("::difftime"), hasName("::mktime"),
              hasName("::time"), hasName("::asctime"), hasName("::ctime"),
              hasName("::gmtime"), hasName("::localtime"),
              hasName("::strftime"),
              hasName("::timespec_get"), // C11
              // <locale.h>
              hasName("::setlocale"), hasName("::localeconv"),
              // <setjmp.h>
              // Note: setjmp is often a macro but behaves like a function call
              // hasName("::setjmp"), hasName("::longjmp"),
              // <signal.h>
              hasName("::signal"), hasName("::raise"),
              // <complex.h> (C99 - double versions listed)
              hasName("::cabs"), hasName("::cacos"), hasName("::cacosh"),
              hasName("::carg"), hasName("::casin"), hasName("::casinh"),
              hasName("::catan"), hasName("::catanh"), hasName("::ccos"),
              hasName("::ccosh"), hasName("::cexp"), hasName("::cimag"),
              hasName("::clog"), hasName("::conj"), hasName("::cpow"),
              hasName("::cproj"), hasName("::creal"), hasName("::csin"),
              hasName("::csinh"), hasName("::csqrt"), hasName("::ctan"),
              hasName("::ctanh"),
              // <fenv.h> (C99)
              hasName("::feclearexcept"), hasName("::fegetexceptflag"),
              hasName("::feraiseexcept"), hasName("::fesetexceptflag"),
              hasName("::fetestexcept"), hasName("::fegetround"),
              hasName("::fesetround"), hasName("::fegetenv"),
              hasName("::feholdexcept"), hasName("::fesetenv"),
              hasName("::feupdateenv"),
              // <inttypes.h> (C99)
              hasName("::imaxabs"), hasName("::imaxdiv"),
              hasName("::strtoimax"), hasName("::strtoumax"),
              hasName("::wcstoimax"), hasName("::wcstoumax"),
              // <wchar.h> (C99 - Selected common functions)
              hasName("::btowc"), hasName("::fgetwc"), hasName("::fgetws"),
              hasName("::fputwc"), hasName("::fputws"), hasName("::fwide"),
              hasName("::fwprintf"), hasName("::fwscanf"), hasName("::getwc"),
              hasName("::getwchar"), hasName("::mbrlen"), hasName("::mbrtowc"),
              hasName("::mbsinit"), hasName("::mbsrtowcs"), hasName("::putwc"),
              hasName("::putwchar"), hasName("::swprintf"),
              hasName("::swscanf"), hasName("::ungetwc"),
              hasName("::vfwprintf"), hasName("::vfwscanf"),
              hasName("::vswprintf"), hasName("::vswscanf"),
              hasName("::vwprintf"), hasName("::vwscanf"), hasName("::wcrtomb"),
              hasName("::wcscat"), hasName("::wcschr"), hasName("::wcscmp"),
              hasName("::wcscoll"), hasName("::wcscpy"), hasName("::wcscspn"),
              hasName("::wcsftime"), hasName("::wcslen"), hasName("::wcsncat"),
              hasName("::wcsncmp"), hasName("::wcsncpy"), hasName("::wcspbrk"),
              hasName("::wcsrchr"), hasName("::wcsrtombs"), hasName("::wcsspn"),
              hasName("::wcsstr"), hasName("::wcstod"),
              hasName("::wcstof"),  // C99
              hasName("::wcstold"), // C99
              hasName("::wcstok"), hasName("::wcstol"),
              hasName("::wcstoll"), // C99
              hasName("::wcstoul"),
              hasName("::wcstoull"), // C99
              hasName("::wcsxfrm"), hasName("::wctob"), hasName("::wmemchr"),
              hasName("::wmemcmp"), hasName("::wmemcpy"), hasName("::wmemmove"),
              hasName("::wmemset"), hasName("::wprintf"), hasName("::wscanf"),
              // <wctype.h> (C99)
              hasName("::iswalnum"), hasName("::iswalpha"),
              hasName("::iswblank"), hasName("::iswcntrl"),
              hasName("::iswctype"), hasName("::iswdigit"),
              hasName("::iswgraph"), hasName("::iswlower"),
              hasName("::iswprint"), hasName("::iswpunct"),
              hasName("::iswspace"), hasName("::iswupper"),
              hasName("::iswxdigit"), hasName("::towctrans"),
              hasName("::towlower"), hasName("::towupper"),
              hasName("::wctrans"), hasName("::wctype"),
              // <stdatomic.h> (C11)
              hasName("::atomic_thread_fence"),
              hasName("::atomic_signal_fence"),
              hasName("::atomic_is_lock_free"), hasName("::atomic_store"),
              hasName("::atomic_store_explicit"), hasName("::atomic_load"),
              hasName("::atomic_load_explicit"), hasName("::atomic_exchange"),
              hasName("::atomic_exchange_explicit"),
              hasName("::atomic_compare_exchange_strong"),
              hasName("::atomic_compare_exchange_strong_explicit"),
              hasName("::atomic_compare_exchange_weak"),
              hasName("::atomic_compare_exchange_weak_explicit"),
              hasName("::atomic_fetch_add"),
              hasName("::atomic_fetch_add_explicit"),
              hasName("::atomic_fetch_sub"),
              hasName("::atomic_fetch_sub_explicit"),
              hasName("::atomic_fetch_or"),
              hasName("::atomic_fetch_or_explicit"),
              hasName("::atomic_fetch_xor"),
              hasName("::atomic_fetch_xor_explicit"),
              hasName("::atomic_fetch_and"),
              hasName("::atomic_fetch_and_explicit"),
              // Atomic flag functions
              hasName("::atomic_flag_test_and_set"),
              hasName("::atomic_flag_test_and_set_explicit"),
              hasName("::atomic_flag_clear"),
              hasName("::atomic_flag_clear_explicit"),
              // Note: atomic_init is deprecated in C++20, potentially later C
              // revisions
              // <threads.h> (C11)
              hasName("::thrd_create"), hasName("::thrd_equal"),
              hasName("::thrd_current"), hasName("::thrd_sleep"),
              hasName("::thrd_yield"), hasName("::thrd_exit"),
              hasName("::thrd_detach"), hasName("::thrd_join"),
              hasName("::mtx_init"), hasName("::mtx_lock"),
              hasName("::mtx_timedlock"), hasName("::mtx_trylock"),
              hasName("::mtx_unlock"), hasName("::mtx_destroy"),
              hasName("::call_once"), hasName("::cnd_init"),
              hasName("::cnd_signal"), hasName("::cnd_broadcast"),
              hasName("::cnd_wait"), hasName("::cnd_timedwait"),
              hasName("::cnd_destroy"), hasName("::tss_create"),
              hasName("::tss_get"), hasName("::tss_set"),
              hasName("::tss_delete"),
              // <uchar.h> (C11)
              hasName("::mbrtoc16"), hasName("::c16rtomb"),
              hasName("::mbrtoc32"), hasName("::c32rtomb")),
          unless(isImplicit()))
          .bind("std_func"),
      this);
}

void StdFuncOverrideCheck::check(const MatchFinder::MatchResult &Result) {
  const auto *MatchedDecl = Result.Nodes.getNodeAs<FunctionDecl>("std_func");
  if (!MatchedDecl)
    return;
  diag(MatchedDecl->getLocation(), "禁止覆盖标准库函数");
}
} // namespace clang::tidy::gjb8114