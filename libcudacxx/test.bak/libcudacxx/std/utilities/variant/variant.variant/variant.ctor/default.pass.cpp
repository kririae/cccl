//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11
// UNSUPPORTED: msvc-19.16

// Throwing bad_variant_access is supported starting in macosx10.13
// XFAIL: use_system_cxx_lib && target={{.+}}-apple-macosx10.{{9|10|11|12}} && !no-exceptions

// <cuda/std/variant>

// template <class ...Types> class variant;

// constexpr variant() noexcept(see below);

#include <cuda/std/cassert>
#include <cuda/std/type_traits>
#include <cuda/std/variant>

#include "test_macros.h"
#include "variant_test_helpers.h"

struct NonDefaultConstructible {
  __host__ __device__
  constexpr NonDefaultConstructible(int) {}
};

struct NotNoexcept {
  __host__ __device__
  NotNoexcept() noexcept(false) {}
};

#ifndef TEST_HAS_NO_EXCEPTIONS
struct DefaultCtorThrows {
  __host__ __device__
  DefaultCtorThrows() { throw 42; }
};
#endif

__host__ __device__
void test_default_ctor_sfinae() {
  {
    using V = cuda::std::variant<cuda::std::monostate, int>;
    static_assert(cuda::std::is_default_constructible<V>::value, "");
  }
  {
    using V = cuda::std::variant<NonDefaultConstructible, int>;
    static_assert(!cuda::std::is_default_constructible<V>::value, "");
  }
#if !defined(TEST_VARIANT_HAS_NO_REFERENCES)
  {
    using V = cuda::std::variant<int &, int>;
    static_assert(!cuda::std::is_default_constructible<V>::value, "");
  }
#endif
}

__host__ __device__
void test_default_ctor_noexcept() {
  {
    using V = cuda::std::variant<int>;
    static_assert(cuda::std::is_nothrow_default_constructible<V>::value, "");
  }
  {
    using V = cuda::std::variant<NotNoexcept>;
    static_assert(!cuda::std::is_nothrow_default_constructible<V>::value, "");
  }
}

__host__ __device__
void test_default_ctor_throws() {
#ifndef TEST_HAS_NO_EXCEPTIONS
  using V = cuda::std::variant<DefaultCtorThrows, int>;
  try {
    V v;
    assert(false);
  } catch (const int &ex) {
    assert(ex == 42);
  } catch (...) {
    assert(false);
  }
#endif
}

__host__ __device__
void test_default_ctor_basic() {
  {
    cuda::std::variant<int> v;
    assert(v.index() == 0);
    assert(cuda::std::get<0>(v) == 0);
  }
  {
    cuda::std::variant<int, long> v;
    assert(v.index() == 0);
    assert(cuda::std::get<0>(v) == 0);
  }
  {
    cuda::std::variant<int, NonDefaultConstructible> v;
    assert(v.index() == 0);
    assert(cuda::std::get<0>(v) == 0);
  }
  {
    using V = cuda::std::variant<int, long>;
    constexpr V v;
    static_assert(v.index() == 0, "");
    static_assert(cuda::std::get<0>(v) == 0, "");
  }
  {
    using V = cuda::std::variant<int, long>;
    constexpr V v;
    static_assert(v.index() == 0, "");
    static_assert(cuda::std::get<0>(v) == 0, "");
  }
  {
    using V = cuda::std::variant<int, NonDefaultConstructible>;
    constexpr V v;
    static_assert(v.index() == 0, "");
    static_assert(cuda::std::get<0>(v) == 0, "");
  }
}

int main(int, char**) {
  test_default_ctor_basic();
  test_default_ctor_sfinae();
  test_default_ctor_noexcept();
  test_default_ctor_throws();

  return 0;
}
