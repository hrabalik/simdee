//
// common SIMD type tests
//
// following macros are expected to be defined
// SIMD_TYPE -- name of the SIMD type as a string
// SIMD_TEST_TAG -- catch tests tag(s) as a string
// SIMD_WIDTH -- expected SIMD width
//

#include <numeric>

#define VAL(TYPE) std::declval<TYPE>()

#define HAS_METHOD(WHO, NAME_ARGS, RETURN_TYPE)                                                    \
    (std::is_same<std::remove_cv<std::remove_reference<decltype(VAL(WHO).NAME_ARGS)>::type>::type, \
                  RETURN_TYPE>::value)

#define ASSERT(COND) static_assert(COND, SIMD_TYPE " compile-time check")

#if !defined(__clang__) && defined(__GNUC__) && __GNUC__ >= 6
// GCC attempts to be helpful here by warning that some attributes of types such as __m128 are not
// preserved when passsed as a template parameter.
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wignored-attributes"
#endif

ASSERT(B::width == SIMD_WIDTH);
ASSERT(F::width == SIMD_WIDTH);
ASSERT(U::width == SIMD_WIDTH);
ASSERT(S::width == SIMD_WIDTH);
ASSERT((std::is_same<B::scalar_t, sd::bool32_t>::value));
ASSERT((std::is_same<F::scalar_t, float>::value));
ASSERT((std::is_same<U::scalar_t, uint32_t>::value));
ASSERT((std::is_same<S::scalar_t, int32_t>::value));
ASSERT((std::is_same<B::mask_t, sd::impl::mask<((1 << SIMD_WIDTH) - 1)>>::value));
ASSERT((std::is_same<B::storage_t, sd::storage<B>>::value));
ASSERT((std::is_same<F::storage_t, sd::storage<F>>::value));
ASSERT((std::is_same<U::storage_t, sd::storage<U>>::value));
ASSERT((std::is_same<S::storage_t, sd::storage<S>>::value));
ASSERT(sizeof(B) == sizeof(B::vector_t));
ASSERT(sizeof(F) == sizeof(F::vector_t));
ASSERT(sizeof(U) == sizeof(U::vector_t));
ASSERT(sizeof(S) == sizeof(S::vector_t));
ASSERT(alignof(B) == alignof(B::vector_t));
ASSERT(alignof(F) == alignof(F::vector_t));
ASSERT(alignof(U) == alignof(U::vector_t));
ASSERT(alignof(S) == alignof(S::vector_t));
ASSERT(sizeof(B::scalar_t) * B::width == sizeof(B));
ASSERT(sizeof(F::scalar_t) * F::width == sizeof(F));
ASSERT(sizeof(U::scalar_t) * U::width == sizeof(U));
ASSERT(sizeof(S::scalar_t) * S::width == sizeof(S));
ASSERT((std::is_trivially_copyable<B>::value));
ASSERT((std::is_trivially_copyable<F>::value));
ASSERT((std::is_trivially_copyable<U>::value));
ASSERT((std::is_trivially_copyable<S>::value));
ASSERT((sd::is_simd_vector<B>::value));
ASSERT((sd::is_simd_vector<F>::value));
ASSERT((sd::is_simd_vector<U>::value));
ASSERT((sd::is_simd_vector<S>::value));
ASSERT(HAS_METHOD(const B, eval(), B));
ASSERT(HAS_METHOD(const F, eval(), F));
ASSERT(HAS_METHOD(const U, eval(), U));
ASSERT(HAS_METHOD(const S, eval(), S));
ASSERT(HAS_METHOD(const B, data(), B::vector_t));
ASSERT(HAS_METHOD(const F, data(), F::vector_t));
ASSERT(HAS_METHOD(const U, data(), U::vector_t));
ASSERT(HAS_METHOD(const S, data(), S::vector_t));
ASSERT(HAS_METHOD(B, aligned_load(VAL(B::scalar_t*)), void));
ASSERT(HAS_METHOD(F, aligned_load(VAL(F::scalar_t*)), void));
ASSERT(HAS_METHOD(U, aligned_load(VAL(U::scalar_t*)), void));
ASSERT(HAS_METHOD(S, aligned_load(VAL(S::scalar_t*)), void));
ASSERT(HAS_METHOD(const B, aligned_store(VAL(B::scalar_t*)), void));
ASSERT(HAS_METHOD(const F, aligned_store(VAL(F::scalar_t*)), void));
ASSERT(HAS_METHOD(const U, aligned_store(VAL(U::scalar_t*)), void));
ASSERT(HAS_METHOD(const S, aligned_store(VAL(S::scalar_t*)), void));
ASSERT(HAS_METHOD(B, unaligned_load(VAL(B::scalar_t*)), void));
ASSERT(HAS_METHOD(F, unaligned_load(VAL(F::scalar_t*)), void));
ASSERT(HAS_METHOD(U, unaligned_load(VAL(U::scalar_t*)), void));
ASSERT(HAS_METHOD(S, unaligned_load(VAL(S::scalar_t*)), void));
ASSERT(HAS_METHOD(const B, unaligned_store(VAL(B::scalar_t*)), void));
ASSERT(HAS_METHOD(const F, unaligned_store(VAL(F::scalar_t*)), void));
ASSERT(HAS_METHOD(const U, unaligned_store(VAL(U::scalar_t*)), void));
ASSERT(HAS_METHOD(const S, unaligned_store(VAL(S::scalar_t*)), void));

#if !defined(__clang__) && defined(__GNUC__) && __GNUC__ >= 6
#pragma GCC diagnostic pop
#endif

const B::storage_t bufAB{SIMDEE_DATA_BUFAB};
const B::storage_t bufBB{SIMDEE_DATA_BUFBB};
const B::storage_t bufZB{SIMDEE_DATA_BUFZB};
const F::storage_t bufAF{SIMDEE_DATA_BUFAF};
const F::storage_t bufBF{SIMDEE_DATA_BUFBF};
const F::storage_t bufZF{SIMDEE_DATA_BUFZF};
const U::storage_t bufAU{SIMDEE_DATA_BUFAU};
const U::storage_t bufBU{SIMDEE_DATA_BUFBU};
const U::storage_t bufZU{SIMDEE_DATA_BUFZU};
const S::storage_t bufAS{SIMDEE_DATA_BUFAS};
const S::storage_t bufBS{SIMDEE_DATA_BUFBS};
const S::storage_t bufZS{SIMDEE_DATA_BUFZS};

TEST_CASE(SIMD_TYPE " explicit construction", SIMD_TEST_TAG) {
    B::storage_t rb = bufZB;
    F::storage_t rf = bufZF;
    U::storage_t ru = bufZU;
    S::storage_t rs = bufZS;

    auto tor = [&rb, &rf, &ru, &rs](const B& tb, const F& tf, const U& tu, const S& ts) {
        rb = tb;
        rf = tf;
        ru = tu;
        rs = ts;
    };

    SECTION("from scalar_t") {
        B tb(true);
        F tf(1.2345678f);
        U tu(123456789U);
        S ts(-123456789);
        tor(tb, tf, tu, ts);
        for (auto val : rb) REQUIRE(val == true);
        for (auto val : rf) REQUIRE(val == 1.2345678f);
        for (auto val : ru) REQUIRE(val == 123456789U);
        for (auto val : rs) REQUIRE(val == -123456789);
    }
    SECTION("from Width x scalar_t (parentheses)") {
        B tb(SIMDEE_DATA_BUFAB);
        F tf(SIMDEE_DATA_BUFAF);
        U tu(SIMDEE_DATA_BUFAU);
        S ts(SIMDEE_DATA_BUFAS);
        tor(tb, tf, tu, ts);
        REQUIRE(rb == bufAB);
        REQUIRE(rf == bufAF);
        REQUIRE(ru == bufAU);
        REQUIRE(rs == bufAS);
    }
    SECTION("from Width x scalar_t (braces)") {
        B tb{SIMDEE_DATA_BUFBB};
        F tf{SIMDEE_DATA_BUFBF};
        U tu{SIMDEE_DATA_BUFBU};
        S ts{SIMDEE_DATA_BUFBS};
        tor(tb, tf, tu, ts);
        REQUIRE(rb == bufBB);
        REQUIRE(rf == bufBF);
        REQUIRE(ru == bufBU);
        REQUIRE(rs == bufBS);
    }
    SECTION("from vector_t") {
        B tb(B::vector_t{});
        F tf(F::vector_t{});
        U tu(U::vector_t{});
        S ts(S::vector_t{});
        tor(tb, tf, tu, ts);
        //
        // no requirements
        //
    }
    SECTION("from aligned pointer") {
        B tb(sd::aligned(bufAB.data()));
        F tf(sd::aligned(bufAF.data()));
        U tu(sd::aligned(bufAU.data()));
        S ts(sd::aligned(bufAS.data()));
        tor(tb, tf, tu, ts);
        REQUIRE(rb == bufAB);
        REQUIRE(rf == bufAF);
        REQUIRE(ru == bufAU);
        REQUIRE(rs == bufAS);
    }
    SECTION("from unaligned pointer") {
        B tb(sd::unaligned(bufAB.data()));
        F tf(sd::unaligned(bufAF.data()));
        U tu(sd::unaligned(bufAU.data()));
        S ts(sd::unaligned(bufAS.data()));
        tor(tb, tf, tu, ts);
        REQUIRE(rb == bufAB);
        REQUIRE(rf == bufAF);
        REQUIRE(ru == bufAU);
        REQUIRE(rs == bufAS);
    }
    SECTION("from storage_t") {
        B tb(bufAB);
        F tf(bufAF);
        U tu(bufAU);
        S ts(bufAS);
        tor(tb, tf, tu, ts);
        REQUIRE(rb == bufAB);
        REQUIRE(rf == bufAF);
        REQUIRE(ru == bufAU);
        REQUIRE(rs == bufAS);
    }
    SECTION("from zero, all_bits, sign_bit, etc. (sd::init family)") {
        {
            B tb(sd::zero());
            F tf(sd::zero());
            U tu(sd::zero());
            S ts(sd::zero());
            tor(tb, tf, tu, ts);
            for (auto val : rb) REQUIRE(sd::dirty::as_u(val) == 0x00000000);
            for (auto val : rf) REQUIRE(sd::dirty::as_u(val) == 0x00000000);
            for (auto val : ru) REQUIRE(sd::dirty::as_u(val) == 0x00000000);
            for (auto val : rs) REQUIRE(sd::dirty::as_u(val) == 0x00000000);
        }
        {
            B tb(sd::all_bits());
            F tf(sd::all_bits());
            U tu(sd::all_bits());
            S ts(sd::all_bits());
            tor(tb, tf, tu, ts);
            for (auto val : rb) REQUIRE(sd::dirty::as_u(val) == 0xffffffff);
            for (auto val : rf) REQUIRE(sd::dirty::as_u(val) == 0xffffffff);
            for (auto val : ru) REQUIRE(sd::dirty::as_u(val) == 0xffffffff);
            for (auto val : rs) REQUIRE(sd::dirty::as_u(val) == 0xffffffff);
        }
        {
            B tb(sd::sign_bit());
            F tf(sd::sign_bit());
            U tu(sd::sign_bit());
            S ts(sd::sign_bit());
            tor(tb, tf, tu, ts);
            for (auto val : rb) REQUIRE(sd::dirty::as_u(val) == 0x80000000);
            for (auto val : rf) REQUIRE(sd::dirty::as_u(val) == 0x80000000);
            for (auto val : ru) REQUIRE(sd::dirty::as_u(val) == 0x80000000);
            for (auto val : rs) REQUIRE(sd::dirty::as_u(val) == 0x80000000);
        }
        {
            B tb(sd::abs_mask());
            F tf(sd::abs_mask());
            U tu(sd::abs_mask());
            S ts(sd::abs_mask());
            tor(tb, tf, tu, ts);
            for (auto val : rb) REQUIRE(sd::dirty::as_u(val) == 0x7fffffff);
            for (auto val : rf) REQUIRE(sd::dirty::as_u(val) == 0x7fffffff);
            for (auto val : ru) REQUIRE(sd::dirty::as_u(val) == 0x7fffffff);
            for (auto val : rs) REQUIRE(sd::dirty::as_u(val) == 0x7fffffff);
        }
    }
}

TEST_CASE(SIMD_TYPE " implicit construction", SIMD_TEST_TAG) {
    B::storage_t rb = bufZB;
    F::storage_t rf = bufZF;
    U::storage_t ru = bufZU;
    S::storage_t rs = bufZS;

    auto implicit_test = [&rb, &rf, &ru, &rs](const B& tb, const F& tf, const U& tu, const S& ts) {
        rb = tb;
        rf = tf;
        ru = tu;
        rs = ts;
    };

    SECTION("from scalar_t") {
        implicit_test(B::scalar_t(true), 1.2345678f, 123456789U, -123456789);
        for (auto val : rb) REQUIRE(val == true);
        for (auto val : rf) REQUIRE(val == 1.2345678f);
        for (auto val : ru) REQUIRE(val == 123456789U);
        for (auto val : rs) REQUIRE(val == -123456789);
    }
    SECTION("from vector_t") {
        implicit_test(B::vector_t{}, F::vector_t{}, U::vector_t{}, S::vector_t{});
        //
        // no requirements
        //
    }
    SECTION("from aligned pointer") {
        implicit_test(sd::aligned(bufAB.data()), sd::aligned(bufAF.data()),
                      sd::aligned(bufAU.data()), sd::aligned(bufAS.data()));
        REQUIRE(rb == bufAB);
        REQUIRE(rf == bufAF);
        REQUIRE(ru == bufAU);
        REQUIRE(rs == bufAS);
    }
    SECTION("from unaligned pointer") {
        implicit_test(sd::unaligned(bufAB.data()), sd::unaligned(bufAF.data()),
                      sd::unaligned(bufAU.data()), sd::unaligned(bufAS.data()));
        REQUIRE(rb == bufAB);
        REQUIRE(rf == bufAF);
        REQUIRE(ru == bufAU);
        REQUIRE(rs == bufAS);
    }
    SECTION("from storage_t") {
        implicit_test(bufAB, bufAF, bufAU, bufAS);
        REQUIRE(rb == bufAB);
        REQUIRE(rf == bufAF);
        REQUIRE(ru == bufAU);
        REQUIRE(rs == bufAS);
    }
    SECTION("from zero, all_bits, sign_bit, etc. (sd::init family)") {
        implicit_test(sd::zero(), sd::zero(), sd::zero(), sd::zero());
        for (auto val : rb) REQUIRE(sd::dirty::as_u(val) == 0x00000000);
        for (auto val : rf) REQUIRE(sd::dirty::as_u(val) == 0x00000000);
        for (auto val : ru) REQUIRE(sd::dirty::as_u(val) == 0x00000000);
        for (auto val : rs) REQUIRE(sd::dirty::as_u(val) == 0x00000000);
        implicit_test(sd::all_bits(), sd::all_bits(), sd::all_bits(), sd::all_bits());
        for (auto val : rb) REQUIRE(sd::dirty::as_u(val) == 0xffffffff);
        for (auto val : rf) REQUIRE(sd::dirty::as_u(val) == 0xffffffff);
        for (auto val : ru) REQUIRE(sd::dirty::as_u(val) == 0xffffffff);
        for (auto val : rs) REQUIRE(sd::dirty::as_u(val) == 0xffffffff);
        implicit_test(sd::sign_bit(), sd::sign_bit(), sd::sign_bit(), sd::sign_bit());
        for (auto val : rb) REQUIRE(sd::dirty::as_u(val) == 0x80000000);
        for (auto val : rf) REQUIRE(sd::dirty::as_u(val) == 0x80000000);
        for (auto val : ru) REQUIRE(sd::dirty::as_u(val) == 0x80000000);
        for (auto val : rs) REQUIRE(sd::dirty::as_u(val) == 0x80000000);
        implicit_test(sd::abs_mask(), sd::abs_mask(), sd::abs_mask(), sd::abs_mask());
        for (auto val : rb) REQUIRE(sd::dirty::as_u(val) == 0x7fffffff);
        for (auto val : rf) REQUIRE(sd::dirty::as_u(val) == 0x7fffffff);
        for (auto val : ru) REQUIRE(sd::dirty::as_u(val) == 0x7fffffff);
        for (auto val : rs) REQUIRE(sd::dirty::as_u(val) == 0x7fffffff);
    }
}

TEST_CASE(SIMD_TYPE " assignment", SIMD_TEST_TAG) {
    B tb;
    F tf;
    U tu;
    S ts;
    B::storage_t rb = bufZB;
    F::storage_t rf = bufZF;
    U::storage_t ru = bufZU;
    S::storage_t rs = bufZS;

    SECTION("from...") {
        auto tor = [&]() {
            rb = tb;
            rf = tf;
            ru = tu;
            rs = ts;
        };

        SECTION("from scalar_t") {
            tb = B::scalar_t(true);
            tf = 1.2345678f;
            tu = 123456789U;
            ts = -123456789;
            tor();
            for (auto val : rb) REQUIRE(val == true);
            for (auto val : rf) REQUIRE(val == 1.2345678f);
            for (auto val : ru) REQUIRE(val == 123456789U);
            for (auto val : rs) REQUIRE(val == -123456789);
        }
        SECTION("from vector_t") {
            tb = B::vector_t{};
            tf = F::vector_t{};
            tu = U::vector_t{};
            ts = S::vector_t{};
            tor();
            //
            // no requirements
            //
        }
        SECTION("from aligned pointer") {
            tb = sd::aligned(bufAB.data());
            tf = sd::aligned(bufAF.data());
            tu = sd::aligned(bufAU.data());
            ts = sd::aligned(bufAS.data());
            tor();
            REQUIRE(rb == bufAB);
            REQUIRE(rf == bufAF);
            REQUIRE(ru == bufAU);
            REQUIRE(rs == bufAS);
        }
        SECTION("from unaligned pointer") {
            tb = sd::unaligned(bufAB.data());
            tf = sd::unaligned(bufAF.data());
            tu = sd::unaligned(bufAU.data());
            ts = sd::unaligned(bufAS.data());
            tor();
            REQUIRE(rb == bufAB);
            REQUIRE(rf == bufAF);
            REQUIRE(ru == bufAU);
            REQUIRE(rs == bufAS);
        }
        SECTION("from storage_t") {
            tb = bufAB;
            tf = bufAF;
            tu = bufAU;
            ts = bufAS;
            tor();
            REQUIRE(rb == bufAB);
            REQUIRE(rf == bufAF);
            REQUIRE(ru == bufAU);
            REQUIRE(rs == bufAS);
        }
        SECTION("from zero, all_bits, sign_bit, etc. (sd::init family)") {
            tb = sd::zero();
            tf = sd::zero();
            tu = sd::zero();
            ts = sd::zero();
            tor();
            for (auto val : rb) REQUIRE(sd::dirty::as_u(val) == 0x00000000);
            for (auto val : rf) REQUIRE(sd::dirty::as_u(val) == 0x00000000);
            for (auto val : ru) REQUIRE(sd::dirty::as_u(val) == 0x00000000);
            for (auto val : rs) REQUIRE(sd::dirty::as_u(val) == 0x00000000);
            tb = sd::all_bits();
            tf = sd::all_bits();
            tu = sd::all_bits();
            ts = sd::all_bits();
            tor();
            for (auto val : rb) REQUIRE(sd::dirty::as_u(val) == 0xffffffff);
            for (auto val : rf) REQUIRE(sd::dirty::as_u(val) == 0xffffffff);
            for (auto val : ru) REQUIRE(sd::dirty::as_u(val) == 0xffffffff);
            for (auto val : rs) REQUIRE(sd::dirty::as_u(val) == 0xffffffff);
            tb = sd::sign_bit();
            tf = sd::sign_bit();
            tu = sd::sign_bit();
            ts = sd::sign_bit();
            tor();
            for (auto val : rb) REQUIRE(sd::dirty::as_u(val) == 0x80000000);
            for (auto val : rf) REQUIRE(sd::dirty::as_u(val) == 0x80000000);
            for (auto val : ru) REQUIRE(sd::dirty::as_u(val) == 0x80000000);
            for (auto val : rs) REQUIRE(sd::dirty::as_u(val) == 0x80000000);
            tb = sd::abs_mask();
            tf = sd::abs_mask();
            tu = sd::abs_mask();
            ts = sd::abs_mask();
            tor();
            for (auto val : rb) REQUIRE(sd::dirty::as_u(val) == 0x7fffffff);
            for (auto val : rf) REQUIRE(sd::dirty::as_u(val) == 0x7fffffff);
            for (auto val : ru) REQUIRE(sd::dirty::as_u(val) == 0x7fffffff);
            for (auto val : rs) REQUIRE(sd::dirty::as_u(val) == 0x7fffffff);
        }
    }
    SECTION("to...") {
        tb = bufAB;
        tf = bufAF;
        tu = bufAU;
        ts = bufAS;

        SECTION("to aligned pointer") {
            sd::aligned(rb.data()) = tb;
            sd::aligned(rf.data()) = tf;
            sd::aligned(ru.data()) = tu;
            sd::aligned(rs.data()) = ts;
            REQUIRE(rb == bufAB);
            REQUIRE(rf == bufAF);
            REQUIRE(ru == bufAU);
            REQUIRE(rs == bufAS);
        }
        SECTION("to unaligned pointer") {
            sd::unaligned(rb.data()) = tb;
            sd::unaligned(rf.data()) = tf;
            sd::unaligned(ru.data()) = tu;
            sd::unaligned(rs.data()) = ts;
            REQUIRE(rb == bufAB);
            REQUIRE(rf == bufAF);
            REQUIRE(ru == bufAU);
            REQUIRE(rs == bufAS);
        }
        SECTION("to storage_t") {
            rb = tb;
            rf = tf;
            ru = tu;
            rs = ts;
            REQUIRE(rb == bufAB);
            REQUIRE(rf == bufAF);
            REQUIRE(ru == bufAU);
            REQUIRE(rs == bufAS);
        }
    }
}

TEST_CASE(SIMD_TYPE " type conversion", SIMD_TEST_TAG) {
    SECTION("int to float") {
        F::storage_t expected, result;
        std::transform(begin(bufAS), end(bufAS), begin(expected),
                       [](S::scalar_t a) { return static_cast<F::scalar_t>(a); });
        S in = bufAS;
        result = F(in);
        REQUIRE(result == expected);
    }
    SECTION("float to int") {
        S::storage_t expected, result;
        std::transform(begin(bufAF), end(bufAF), begin(expected),
                       [](F::scalar_t a) { return static_cast<S::scalar_t>(a); });
        F in = bufAF;
        result = S(in);
        REQUIRE(result == expected);
    }
    SECTION("int to uint") {
        U::storage_t expected, result;
        std::transform(begin(bufAS), end(bufAS), begin(expected),
                       [](S::scalar_t a) { return static_cast<U::scalar_t>(a); });
        S in = bufAS;
        result = U(in);
        REQUIRE(result == expected);
    }
    SECTION("uint to int") {
        S::storage_t expected, result;
        std::transform(begin(bufAU), end(bufAU), begin(expected),
                       [](U::scalar_t a) { return static_cast<S::scalar_t>(a); });
        U in = bufAU;
        result = S(in);
        REQUIRE(result == expected);
    }
    SECTION("bool to uint") {
        U::storage_t expected, result;
        std::transform(begin(bufAB), end(bufAB), begin(expected),
                       [](B::scalar_t a) { return static_cast<U::scalar_t>(a); });
        B in = bufAB;
        result = U(in);
        REQUIRE(result == expected);
    }
}

TEST_CASE(SIMD_TYPE " bool arithmetic", SIMD_TEST_TAG) {
    using scalar_t = B::scalar_t;
    B::storage_t r, e;
    B va = bufAB;
    B vb = bufBB;

    auto expect1 = [&e](scalar_t (*f)(scalar_t)) {
        std::transform(begin(bufAB), end(bufAB), begin(e), f);
    };
    auto expect = [&e](scalar_t (*f)(scalar_t, scalar_t)) {
        std::transform(begin(bufAB), end(bufAB), begin(bufBB), begin(e), f);
    };

    SECTION("log not") {
        expect1([](scalar_t a) { return !a; });
        r = !va;
        REQUIRE(r == e);
    }
    SECTION("log and") {
        expect([](scalar_t a, scalar_t b) { return a && b; });
        r = va && vb;
        REQUIRE(r == e);
    }
    SECTION("log or") {
        expect([](scalar_t a, scalar_t b) { return a || b; });
        r = va || vb;
        REQUIRE(r == e);
    }
    SECTION("complex expr") {
        expect([](scalar_t a, scalar_t b) { return !((!a && !b) || (!a || !b)); });
        r = !((!va && !vb) || (!va || !vb));
        REQUIRE(r == e);
    }
}

TEST_CASE(SIMD_TYPE " float arithmetic", SIMD_TEST_TAG) {
    using scalar_t = F::scalar_t;
    F::storage_t r, e;
    F va = bufAF;
    F vb = bufBF;

    auto expect1 = [&e](scalar_t (*f)(scalar_t)) {
        std::transform(begin(bufAF), end(bufAF), begin(e), f);
    };
    auto expect = [&e](scalar_t (*f)(scalar_t, scalar_t)) {
        std::transform(begin(bufAF), end(bufAF), begin(bufBF), begin(e), f);
    };

    SECTION("unary plus") {
        expect1([](scalar_t a) { return +a; });
        r = +va;
        REQUIRE(r == e);
    }
    SECTION("unary minus") {
        expect1([](scalar_t a) { return -a; });
        r = -va;
        REQUIRE(r == e);
    }
    SECTION("plus") {
        expect([](scalar_t a, scalar_t b) { return a + b; });
        r = va + vb;
        REQUIRE(r == e);
        va += vb;
        r = va;
        REQUIRE(r == e);
    }
    SECTION("minus") {
        expect([](scalar_t a, scalar_t b) { return a - b; });
        r = va - vb;
        REQUIRE(r == e);
        va -= vb;
        r = va;
        REQUIRE(r == e);
    }
    SECTION("multiplies") {
        expect([](scalar_t a, scalar_t b) { return a * b; });
        r = va * vb;
        REQUIRE(r == e);
        va *= vb;
        r = va;
        REQUIRE(r == e);
    }
    SECTION("divides") {
        expect([](scalar_t a, scalar_t b) { return a / b; });
        r = va / vb;
        REQUIRE(r == e);
        va /= vb;
        r = va;
        REQUIRE(r == e);
    }
    SECTION("minimum") {
        expect([](scalar_t a, scalar_t b) { return std::min(a, b); });
        r = min(va, vb);
        REQUIRE(r == e);
    }
    SECTION("maximum") {
        expect([](scalar_t a, scalar_t b) { return std::max(a, b); });
        r = max(va, vb);
        REQUIRE(r == e);
    }
    SECTION("absolute value") {
        expect1([](scalar_t a) { return std::abs(a); });
        r = abs(va);
        REQUIRE(r == e);
    }
    SECTION("square root") {
        expect1([](scalar_t a) { return std::sqrt(std::abs(a)); });
        r = sqrt(abs(va));
        for (auto i = 0U; i < F::width; ++i) REQUIRE(r[i] == Approx(e[i]));
    }
    SECTION("fast reciprocal") {
        expect1([](scalar_t a) { return 1 / a; });
        r = rcp(va);
        for (auto i = 0U; i < F::width; ++i) REQUIRE(r[i] == Approx(e[i]).epsilon(0.005));
    }
    SECTION("fast reciprocal square root") {
        expect1([](scalar_t a) { return 1 / std::sqrt(std::abs(a)); });
        r = rsqrt(abs(va));
        for (auto i = 0U; i < F::width; ++i) REQUIRE(r[i] == Approx(e[i]).epsilon(0.005));
    }
    SECTION("rhs of compound can be implicitly constructed") {
        va = vb;
        va += 1.23f;
        vb = vb + 1.23f;
        REQUIRE(all(va == vb));
        va -= 2.34f;
        vb = vb - 2.34f;
        REQUIRE(all(va == vb));
        va *= 3.45f;
        vb = vb * 3.45f;
        REQUIRE(all(va == vb));
        va /= 4.56f;
        vb = vb / 4.56f;
        REQUIRE(all(va == vb));
    }
}

TEST_CASE(SIMD_TYPE " uint arithmetic", SIMD_TEST_TAG) {
    using scalar_t = U::scalar_t;
    U::storage_t r, e;
    U va = bufAU;
    U vb = bufBU;

    auto expect1 = [&e](scalar_t (*f)(scalar_t)) {
        std::transform(begin(bufAU), end(bufAU), begin(e), f);
    };
    auto expect = [&e](scalar_t (*f)(scalar_t, scalar_t)) {
        std::transform(begin(bufAU), end(bufAU), begin(bufBU), begin(e), f);
    };

    SECTION("bit not") {
        expect1([](scalar_t a) { return ~a; });
        r = ~va;
        REQUIRE(r == e);
    }
    SECTION("bit and") {
        expect([](scalar_t a, scalar_t b) { return a & b; });
        r = va & vb;
        REQUIRE(r == e);
        va &= vb;
        r = va;
        REQUIRE(r == e);
    }
    SECTION("bit or") {
        expect([](scalar_t a, scalar_t b) { return a | b; });
        r = va | vb;
        REQUIRE(r == e);
        va |= vb;
        r = va;
        REQUIRE(r == e);
    }
    SECTION("bit xor") {
        expect([](scalar_t a, scalar_t b) { return a ^ b; });
        r = va ^ vb;
        REQUIRE(r == e);
        va ^= vb;
        r = va;
        REQUIRE(r == e);
    }
    SECTION("complex bit-wise expr") {
        expect([](scalar_t a, scalar_t b) { return ~((~a & ~b) | (~a ^ ~b)); });
        r = ~((~va & ~vb) | (~va ^ ~vb));
        REQUIRE(r == e);
    }
    SECTION("unary plus") {
        expect1([](scalar_t a) { return +a; });
        r = +va;
        REQUIRE(r == e);
    }
    SECTION("unary minus") {
        expect1([](scalar_t a) { return 0 - a; });
        r = -va;
        REQUIRE(r == e);
    }
    SECTION("plus") {
        expect([](scalar_t a, scalar_t b) { return a + b; });
        r = va + vb;
        REQUIRE(r == e);
        va += vb;
        r = va;
        REQUIRE(r == e);
    }
    SECTION("minus") {
        expect([](scalar_t a, scalar_t b) { return a - b; });
        r = va - vb;
        REQUIRE(r == e);
        va -= vb;
        r = va;
        REQUIRE(r == e);
    }
    SECTION("multiplies") {
        expect([](scalar_t a, scalar_t b) { return a * b; });
        r = va * vb;
        REQUIRE(r == e);
        va *= vb;
        r = va;
        REQUIRE(r == e);
    }
    SECTION("minimum") {
        expect([](scalar_t a, scalar_t b) { return std::min(a, b); });
        r = min(va, vb);
        REQUIRE(r == e);
    }
    SECTION("maximum") {
        expect([](scalar_t a, scalar_t b) { return std::max(a, b); });
        r = max(va, vb);
        REQUIRE(r == e);
    }
    SECTION("rhs of compound can be implicitly constructed") {
        va = vb;
        va &= 0xdeadbeefU;
        vb = vb & 0xdeadbeefU;
        REQUIRE(all(va == vb));
        va |= 0xf0f0f0f0U;
        vb = vb | 0xf0f0f0f0U;
        REQUIRE(all(va == vb));
        va ^= 0x1234abcdU;
        vb = vb ^ 0x1234abcdU;
        REQUIRE(all(va == vb));
    }
}

TEST_CASE(SIMD_TYPE " int arithmetic", SIMD_TEST_TAG) {
    using scalar_t = S::scalar_t;
    S::storage_t r, e;
    S va = bufAS;
    S vb = bufBS;

    auto expect1 = [&e](scalar_t (*f)(scalar_t)) {
        std::transform(begin(bufAS), end(bufAS), begin(e), f);
    };
    auto expect = [&e](scalar_t (*f)(scalar_t, scalar_t)) {
        std::transform(begin(bufAS), end(bufAS), begin(bufBS), begin(e), f);
    };

    SECTION("bit not") {
        expect1([](scalar_t a) { return ~a; });
        r = ~va;
        REQUIRE(r == e);
    }
    SECTION("bit and") {
        expect([](scalar_t a, scalar_t b) { return a & b; });
        r = va & vb;
        REQUIRE(r == e);
        va &= vb;
        r = va;
        REQUIRE(r == e);
    }
    SECTION("bit or") {
        expect([](scalar_t a, scalar_t b) { return a | b; });
        r = va | vb;
        REQUIRE(r == e);
        va |= vb;
        r = va;
        REQUIRE(r == e);
    }
    SECTION("bit xor") {
        expect([](scalar_t a, scalar_t b) { return a ^ b; });
        r = va ^ vb;
        REQUIRE(r == e);
        va ^= vb;
        r = va;
        REQUIRE(r == e);
    }
    SECTION("complex bit-wise expr") {
        expect([](scalar_t a, scalar_t b) { return ~((~a & ~b) | (~a ^ ~b)); });
        r = ~((~va & ~vb) | (~va ^ ~vb));
        REQUIRE(r == e);
    }
    SECTION("unary plus") {
        expect1([](scalar_t a) { return +a; });
        r = +va;
        REQUIRE(r == e);
    }
    SECTION("unary minus") {
        expect1([](scalar_t a) { return -a; });
        r = -va;
        REQUIRE(r == e);
    }
    SECTION("plus") {
        expect([](scalar_t a, scalar_t b) { return a + b; });
        r = va + vb;
        REQUIRE(r == e);
        va += vb;
        r = va;
        REQUIRE(r == e);
    }
    SECTION("minus") {
        expect([](scalar_t a, scalar_t b) { return a - b; });
        r = va - vb;
        REQUIRE(r == e);
        va -= vb;
        r = va;
        REQUIRE(r == e);
    }
    SECTION("multiplies") {
        expect([](scalar_t a, scalar_t b) { return a * b; });
        r = va * vb;
        REQUIRE(r == e);
        va *= vb;
        r = va;
        REQUIRE(r == e);
    }
    SECTION("minimum") {
        expect([](scalar_t a, scalar_t b) { return std::min(a, b); });
        r = min(va, vb);
        REQUIRE(r == e);
    }
    SECTION("maximum") {
        expect([](scalar_t a, scalar_t b) { return std::max(a, b); });
        r = max(va, vb);
        REQUIRE(r == e);
    }
    SECTION("absolute value") {
        expect1([](scalar_t a) { return std::abs(a); });
        r = abs(va);
        REQUIRE(r == e);
    }
    SECTION("rhs of compound can be implicitly constructed") {
        va = vb;
        va += 123;
        vb = vb + 123;
        REQUIRE(all(va == vb));
        va -= 234;
        vb = vb - 234;
        REQUIRE(all(va == vb));
        va *= 345;
        vb = vb * 345;
        REQUIRE(all(va == vb));
    }
}

TEST_CASE(SIMD_TYPE " bool comparison", SIMD_TEST_TAG) {
    using scalar_t = B::scalar_t;
    B::storage_t r, e;
    B va = bufAB;
    B vb = bufBB;

    auto expect0 = [&e](bool v) { std::fill(begin(e), end(e), v); };
    auto expect = [&e](bool (*f)(scalar_t, scalar_t)) {
        std::transform(begin(bufAB), end(bufAB), begin(bufBB), begin(e), f);
    };

    SECTION("equal to") {
        expect([](scalar_t a, scalar_t b) { return a == b; });
        r = va == vb;
        REQUIRE(r == e);
        expect0(true);
        r = va == va;
        REQUIRE(r == e);
    }
    SECTION("not equal to") {
        expect([](scalar_t a, scalar_t b) { return a != b; });
        r = va != vb;
        REQUIRE(r == e);
        expect0(false);
        r = va != va;
        REQUIRE(r == e);
    }
}

TEST_CASE(SIMD_TYPE " float comparison", SIMD_TEST_TAG) {
    using scalar_t = F::scalar_t;
    B::storage_t r, e;
    F va = bufAF;
    F vb = bufBF;

    auto expect0 = [&e](bool v) { std::fill(begin(e), end(e), v); };
    auto expect = [&e](bool (*f)(scalar_t, scalar_t)) {
        std::transform(begin(bufAF), end(bufAF), begin(bufBF), begin(e), f);
    };

    SECTION("equal to") {
        expect([](scalar_t a, scalar_t b) { return a == b; });
        r = va == vb;
        REQUIRE(r == e);
        expect0(true);
        r = va == va;
        REQUIRE(r == e);
    }
    SECTION("not equal to") {
        expect([](scalar_t a, scalar_t b) { return a != b; });
        r = va != vb;
        REQUIRE(r == e);
        expect0(false);
        r = va != va;
        REQUIRE(r == e);
    }
    SECTION("greater") {
        expect([](scalar_t a, scalar_t b) { return a > b; });
        r = va > vb;
        REQUIRE(r == e);
        expect0(false);
        r = va > va;
        REQUIRE(r == e);
    }
    SECTION("less") {
        expect([](scalar_t a, scalar_t b) { return a < b; });
        r = va < vb;
        REQUIRE(r == e);
        expect0(false);
        r = va < va;
        REQUIRE(r == e);
    }
    SECTION("greater equal") {
        expect([](scalar_t a, scalar_t b) { return a >= b; });
        r = va >= vb;
        REQUIRE(r == e);
        expect0(true);
        r = va >= va;
        REQUIRE(r == e);
    }
    SECTION("less equal") {
        expect([](scalar_t a, scalar_t b) { return a <= b; });
        r = va <= vb;
        REQUIRE(r == e);
        expect0(true);
        r = va <= va;
        REQUIRE(r == e);
    }
}

TEST_CASE(SIMD_TYPE " uint comparison", SIMD_TEST_TAG) {
    using scalar_t = U::scalar_t;
    B::storage_t r, e;
    U va = bufAU;
    U vb = bufBU;

    auto expect0 = [&e](bool v) { std::fill(begin(e), end(e), v); };
    auto expect = [&e](bool (*f)(scalar_t, scalar_t)) {
        std::transform(begin(bufAU), end(bufAU), begin(bufBU), begin(e), f);
    };

    SECTION("equal to") {
        expect([](scalar_t a, scalar_t b) { return a == b; });
        r = va == vb;
        REQUIRE(r == e);
        expect0(true);
        r = va == va;
        REQUIRE(r == e);
    }
    SECTION("not equal to") {
        expect([](scalar_t a, scalar_t b) { return a != b; });
        r = va != vb;
        REQUIRE(r == e);
        expect0(false);
        r = va != va;
        REQUIRE(r == e);
    }
    SECTION("greater") {
        expect([](scalar_t a, scalar_t b) { return a > b; });
        r = va > vb;
        REQUIRE(r == e);
        expect0(false);
        r = va > va;
        REQUIRE(r == e);
    }
    SECTION("less") {
        expect([](scalar_t a, scalar_t b) { return a < b; });
        r = va < vb;
        REQUIRE(r == e);
        expect0(false);
        r = va < va;
        REQUIRE(r == e);
    }
    SECTION("greater equal") {
        expect([](scalar_t a, scalar_t b) { return a >= b; });
        r = va >= vb;
        REQUIRE(r == e);
        expect0(true);
        r = va >= va;
        REQUIRE(r == e);
    }
    SECTION("less equal") {
        expect([](scalar_t a, scalar_t b) { return a <= b; });
        r = va <= vb;
        REQUIRE(r == e);
        expect0(true);
        r = va <= va;
        REQUIRE(r == e);
    }
}

TEST_CASE(SIMD_TYPE " int comparison", SIMD_TEST_TAG) {
    using scalar_t = S::scalar_t;
    B::storage_t r, e;
    S va = bufAS;
    S vb = bufBS;

    auto expect0 = [&e](bool v) { std::fill(begin(e), end(e), v); };
    auto expect = [&e](bool (*f)(scalar_t, scalar_t)) {
        std::transform(begin(bufAS), end(bufAS), begin(bufBS), begin(e), f);
    };

    SECTION("equal to") {
        expect([](scalar_t a, scalar_t b) { return a == b; });
        r = va == vb;
        REQUIRE(r == e);
        expect0(true);
        r = va == va;
        REQUIRE(r == e);
    }
    SECTION("not equal to") {
        expect([](scalar_t a, scalar_t b) { return a != b; });
        r = va != vb;
        REQUIRE(r == e);
        expect0(false);
        r = va != va;
        REQUIRE(r == e);
    }
    SECTION("greater") {
        expect([](scalar_t a, scalar_t b) { return a > b; });
        r = va > vb;
        REQUIRE(r == e);
        expect0(false);
        r = va > va;
        REQUIRE(r == e);
    }
    SECTION("less") {
        expect([](scalar_t a, scalar_t b) { return a < b; });
        r = va < vb;
        REQUIRE(r == e);
        expect0(false);
        r = va < va;
        REQUIRE(r == e);
    }
    SECTION("greater equal") {
        expect([](scalar_t a, scalar_t b) { return a >= b; });
        r = va >= vb;
        REQUIRE(r == e);
        expect0(true);
        r = va >= va;
        REQUIRE(r == e);
    }
    SECTION("less equal") {
        expect([](scalar_t a, scalar_t b) { return a <= b; });
        r = va <= vb;
        REQUIRE(r == e);
        expect0(true);
        r = va <= va;
        REQUIRE(r == e);
    }
}

TEST_CASE(SIMD_TYPE " horizontal operations", SIMD_TEST_TAG) {
    SECTION("on bools") {
        using scalar_t = B::scalar_t;
        B a = bufAB;
        scalar_t e;
        B::storage_t v;

        SECTION("log and") {
            auto and_ = [](scalar_t l, scalar_t r) { return l && r; };
            e = std::accumulate(begin(bufAB), end(bufAB), true, and_);
            v = reduce(a, sd::op_logand{});
            for (scalar_t vr : v) { REQUIRE(vr == e); }
        }
        SECTION("log or") {
            auto or_ = [](scalar_t l, scalar_t r) { return l || r; };
            e = std::accumulate(begin(bufAB), end(bufAB), false, or_);
            v = reduce(a, sd::op_logor{});
            for (scalar_t vr : v) { REQUIRE(vr == e); }
        }
    }
    SECTION("on floats") {
        using scalar_t = F::scalar_t;
        F a = bufAF;
        scalar_t e;
        F::storage_t v;

        SECTION("max") {
            constexpr scalar_t inf = std::numeric_limits<scalar_t>::infinity();
            auto max_ = [](scalar_t l, scalar_t r) { return std::max(l, r); };
            e = std::accumulate(begin(bufAF), end(bufAF), -inf, max_);
            v = reduce(a, sd::op_max{});
            for (scalar_t vr : v) { REQUIRE(vr == e); }
        }
        SECTION("min") {
            constexpr scalar_t inf = std::numeric_limits<scalar_t>::infinity();
            auto min_ = [](scalar_t l, scalar_t r) { return std::min(l, r); };
            e = std::accumulate(begin(bufAF), end(bufAF), inf, min_);
            v = reduce(a, sd::op_min{});
            for (scalar_t vr : v) { REQUIRE(vr == e); }
        }
        SECTION("sum") {
            e = std::accumulate(begin(bufAF), end(bufAF), scalar_t(0));
            v = reduce(a, sd::op_add{});
            for (scalar_t vr : v) { REQUIRE(vr == Approx(e)); }
        }
        SECTION("product") {
            auto prod = std::multiplies<scalar_t>();
            e = std::accumulate(begin(bufAF), end(bufAF), scalar_t(1), prod);
            v = reduce(a, sd::op_mul{});
            for (scalar_t vr : v) { REQUIRE(vr == Approx(e)); }
        }
    }
    SECTION("on uints") {
        using scalar_t = U::scalar_t;
        U a = bufAU;
        scalar_t e;
        U::storage_t v;

        SECTION("bit and") {
            auto and_ = [](scalar_t l, scalar_t r) { return l & r; };
            e = std::accumulate(begin(bufAU), end(bufAU), scalar_t(-1), and_);
            v = reduce(a, sd::op_bitand{});
            for (scalar_t vr : v) { REQUIRE(vr == e); }
        }
        SECTION("bit or") {
            auto or_ = [](scalar_t l, scalar_t r) { return l | r; };
            e = std::accumulate(begin(bufAU), end(bufAU), scalar_t(0), or_);
            v = reduce(a, sd::op_bitor{});
            for (scalar_t vr : v) { REQUIRE(vr == e); }
        }
        SECTION("bit xor") {
            auto or_ = [](scalar_t l, scalar_t r) { return l ^ r; };
            e = std::accumulate(begin(bufAU), end(bufAU), scalar_t(0), or_);
            v = reduce(a, sd::op_bitxor{});
            for (scalar_t vr : v) { REQUIRE(vr == e); }
        }
        SECTION("sum") {
            e = std::accumulate(begin(bufAU), end(bufAU), scalar_t(0));
            v = reduce(a, sd::op_add{});
            for (scalar_t vr : v) { REQUIRE(vr == Approx(e)); }
        }
        SECTION("product") {
            auto prod = std::multiplies<scalar_t>();
            e = std::accumulate(begin(bufAU), end(bufAU), scalar_t(1), prod);
            v = reduce(a, sd::op_mul{});
            for (scalar_t vr : v) { REQUIRE(vr == Approx(e)); }
        }
        SECTION("min") {
            constexpr scalar_t seed = std::numeric_limits<scalar_t>::max();
            auto min_ = [](scalar_t l, scalar_t r) { return std::min(l, r); };
            e = std::accumulate(begin(bufAU), end(bufAU), seed, min_);
            v = reduce(a, sd::op_min{});
            for (scalar_t vr : v) { REQUIRE(vr == e); }
        }
        SECTION("max") {
            constexpr scalar_t seed = std::numeric_limits<scalar_t>::min();
            auto max_ = [](scalar_t l, scalar_t r) { return std::max(l, r); };
            e = std::accumulate(begin(bufAU), end(bufAU), seed, max_);
            v = reduce(a, sd::op_max{});
            for (scalar_t vr : v) { REQUIRE(vr == e); }
        }
    }
}

TEST_CASE(SIMD_TYPE " conditional", SIMD_TEST_TAG) {
    B ab = bufAB;
    F af = bufAF;
    U au = bufAU;
    S as = bufAS;
    B bb = bufBB;
    F bf = bufBF;
    U bu = bufBU;
    S bs = bufBS;

    B sel = af >= bf;
    auto sel_mask = mask(sel);

    B::storage_t rb(cond(sel, ab, bb));
    F::storage_t rf(cond(sel, af, bf));
    U::storage_t ru(cond(sel, au, bu));
    S::storage_t rs(cond(sel, as, bs));

    for (auto i = 0U; i < F::width; ++i) {
        REQUIRE((rb[i]) == (sel_mask[int(i)] ? bufAB[i] : bufBB[i]));
        REQUIRE((rf[i]) == (sel_mask[int(i)] ? bufAF[i] : bufBF[i]));
        REQUIRE((ru[i]) == (sel_mask[int(i)] ? bufAU[i] : bufBU[i]));
        REQUIRE((rs[i]) == (sel_mask[int(i)] ? bufAS[i] : bufBS[i]));
    }
}

TEST_CASE(SIMD_TYPE " mask", SIMD_TEST_TAG) {
    SECTION("mask itself") {
        auto expected = [](const B::storage_t& s) {
            B::mask_t res(0U);
            for (auto i = 0U; i < s.size(); ++i) {
                if (sd::cast_u(s[i]) & (1U << 31)) { res |= B::mask_t(1U << i); }
            }
            return res;
        };

        REQUIRE(expected(bufAB) == mask(B(bufAB)));
        REQUIRE(expected(bufBB) == mask(B(bufBB)));
    }
    SECTION("any, all") {
        B a = bufAB;
        B b = bufBB;
        REQUIRE(any(a) == any(mask(a)));
        REQUIRE(any(b) == any(mask(b)));
        REQUIRE(all(a) == all(mask(a)));
        REQUIRE(all(b) == all(mask(b)));
    }
}

TEST_CASE(SIMD_TYPE " first_scalar", SIMD_TEST_TAG) {
    B b = bufAB;
    F f = bufAF;
    U u = bufAU;
    S s = bufAS;

    REQUIRE(bufAB[0] == first_scalar(b));
    REQUIRE(bufAF[0] == first_scalar(f));
    REQUIRE(bufAU[0] == first_scalar(u));
    REQUIRE(bufAS[0] == first_scalar(s));
}

TEST_CASE(SIMD_TYPE " broadcast", SIMD_TEST_TAG) {
    B b = bufAB;
    F f = bufAF;
    U u = bufAU;
    S s = bufAS;

#if SIMD_WIDTH >= 1
    REQUIRE(all(b.broadcast<0>() == B(bufAB[0])));
    REQUIRE(all(f.broadcast<0>() == F(bufAF[0])));
    REQUIRE(all(u.broadcast<0>() == U(bufAU[0])));
    REQUIRE(all(s.broadcast<0>() == S(bufAS[0])));
#endif

#if SIMD_WIDTH >= 4
    REQUIRE(all(b.broadcast<1>() == B(bufAB[1])));
    REQUIRE(all(f.broadcast<1>() == F(bufAF[1])));
    REQUIRE(all(u.broadcast<1>() == U(bufAU[1])));
    REQUIRE(all(s.broadcast<1>() == S(bufAS[1])));

    REQUIRE(all(b.broadcast<2>() == B(bufAB[2])));
    REQUIRE(all(f.broadcast<2>() == F(bufAF[2])));
    REQUIRE(all(u.broadcast<2>() == U(bufAU[2])));
    REQUIRE(all(s.broadcast<2>() == S(bufAS[2])));

    REQUIRE(all(b.broadcast<3>() == B(bufAB[3])));
    REQUIRE(all(f.broadcast<3>() == F(bufAF[3])));
    REQUIRE(all(u.broadcast<3>() == U(bufAU[3])));
    REQUIRE(all(s.broadcast<3>() == S(bufAS[3])));
#endif

#if SIMD_WIDTH >= 8
    REQUIRE(all(b.broadcast<4>() == B(bufAB[4])));
    REQUIRE(all(f.broadcast<4>() == F(bufAF[4])));
    REQUIRE(all(u.broadcast<4>() == U(bufAU[4])));
    REQUIRE(all(s.broadcast<4>() == S(bufAS[4])));

    REQUIRE(all(b.broadcast<5>() == B(bufAB[5])));
    REQUIRE(all(f.broadcast<5>() == F(bufAF[5])));
    REQUIRE(all(u.broadcast<5>() == U(bufAU[5])));
    REQUIRE(all(s.broadcast<5>() == S(bufAS[5])));

    REQUIRE(all(b.broadcast<6>() == B(bufAB[6])));
    REQUIRE(all(f.broadcast<6>() == F(bufAF[6])));
    REQUIRE(all(u.broadcast<6>() == U(bufAU[6])));
    REQUIRE(all(s.broadcast<6>() == S(bufAS[6])));

    REQUIRE(all(b.broadcast<7>() == B(bufAB[7])));
    REQUIRE(all(f.broadcast<7>() == F(bufAF[7])));
    REQUIRE(all(u.broadcast<7>() == U(bufAU[7])));
    REQUIRE(all(s.broadcast<7>() == S(bufAS[7])));
#endif
}
