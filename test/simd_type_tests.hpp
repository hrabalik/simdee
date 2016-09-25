//
// common SIMD type tests
//
// following macros are expected to be defined
// SIMD_TYPE -- name of the SIMD type as a string
// SIMD_TEST_TAG -- catch tests tag(s) as a string
// SIMD_WIDTH -- expected W
// SIMD_LOAD_AF -- expression that extracts vector_t from bufAF
// SIMD_LOAD_AU -- expression that extracts vector_t from bufAU
// SIMD_LOAD_AS -- expression that extracts vector_t from bufAS
//

TEST_CASE(SIMD_TYPE " basic guarantees", SIMD_TEST_TAG) {
    REQUIRE(F::W == SIMD_WIDTH);
    REQUIRE(U::W == SIMD_WIDTH);
    REQUIRE(S::W == SIMD_WIDTH);
    REQUIRE((std::is_same<F::scalar_t, float>::value));
    REQUIRE((std::is_same<U::scalar_t, uint32_t>::value));
    REQUIRE((std::is_same<S::scalar_t, int32_t>::value));
    REQUIRE((std::is_same<U::mask_t, simd::mask<U>>::value));
    REQUIRE(sizeof(F) == sizeof(F::vector_t));
    REQUIRE(sizeof(U) == sizeof(U::vector_t));
    REQUIRE(sizeof(S) == sizeof(S::vector_t));
    REQUIRE(alignof(F) == alignof(F::vector_t));
    REQUIRE(alignof(U) == alignof(U::vector_t));
    REQUIRE(alignof(S) == alignof(S::vector_t));
    REQUIRE(sizeof(F::scalar_t) * F::W == sizeof(F));
    REQUIRE(sizeof(U::scalar_t) * U::W == sizeof(U));
    REQUIRE(sizeof(S::scalar_t) * S::W == sizeof(S));
    REQUIRE((std::is_trivially_copyable<F>::value));
    REQUIRE((std::is_trivially_copyable<U>::value));
    REQUIRE((std::is_trivially_copyable<S>::value));
    REQUIRE((std::is_same<decltype(std::declval<U>().mask()), U::mask_t>::value));
    REQUIRE((std::is_same<decltype(std::declval<F>().first_element()), F::scalar_t>::value));
    REQUIRE((std::is_same<decltype(std::declval<U>().first_element()), U::scalar_t>::value));
    REQUIRE((std::is_same<decltype(std::declval<S>().first_element()), S::scalar_t>::value));
}

TEST_CASE(SIMD_TYPE " explicit construction", SIMD_TEST_TAG) {
    simd::storage<F> rf = bufZF;
    simd::storage<U> ru = bufZU;
    simd::storage<S> rs = bufZS;

    auto tor = [&rf, &ru, &rs](const F& tf, const U& tu, const S& ts) {
        rf = tf; ru = tu; rs = ts;
    };

    SECTION("from scalar_t") {
        F tf(1.2345678f);
        U tu(123456789U);
        S ts(-123456789);
        tor(tf, tu, ts);
        for (auto val : rf) REQUIRE(val == 1.2345678f);
        for (auto val : ru) REQUIRE(val == 123456789U);
        for (auto val : rs) REQUIRE(val == -123456789);
    }
    SECTION("from vector_t") {
        F tf(SIMD_LOAD_AF);
        U tu(SIMD_LOAD_AU);
        S ts(SIMD_LOAD_AS);
        tor(tf, tu, ts);
        REQUIRE(rf == bufAF);
        REQUIRE(ru == bufAU);
        REQUIRE(rs == bufAS);
    }
    SECTION("from aligned pointer") {
        F tf(simd::aligned(bufAF.data()));
        U tu(simd::aligned(bufAU.data()));
        S ts(simd::aligned(bufAS.data()));
        tor(tf, tu, ts);
        REQUIRE(rf == bufAF);
        REQUIRE(ru == bufAU);
        REQUIRE(rs == bufAS);
    }
    SECTION("from unaligned pointer") {
        F tf(simd::unaligned(bufAF.data()));
        U tu(simd::unaligned(bufAU.data()));
        S ts(simd::unaligned(bufAS.data()));
        tor(tf, tu, ts);
        REQUIRE(rf == bufAF);
        REQUIRE(ru == bufAU);
        REQUIRE(rs == bufAS);
    }
    SECTION("from simd::storage") {
        simd::storage<F> storf;
        simd::storage<U> storu;
        simd::storage<S> stors;
        storf = bufAF;
        storu = bufAU;
        stors = bufAS;
        F tf(storf);
        U tu(storu);
        S ts(stors);
        tor(tf, tu, ts);
        REQUIRE(rf == bufAF);
        REQUIRE(ru == bufAU);
        REQUIRE(rs == bufAS);
    }
    SECTION("from uval, sval, zero, etc. (simd::init family)") {
        {
            F tf(simd::fval(1.2345678f));
            U tu(simd::fval(1.2345678f));
            S ts(simd::fval(1.2345678f));
            tor(tf, tu, ts);
            for (auto val : rf) REQUIRE(simd::tof(val) == 1.2345678f);
            for (auto val : ru) REQUIRE(simd::tof(val) == 1.2345678f);
            for (auto val : rs) REQUIRE(simd::tof(val) == 1.2345678f);
        }
        {
            F tf(simd::uval(0xdeadbeef));
            U tu(simd::uval(0xdeadbeef));
            S ts(simd::uval(0xdeadbeef));
            tor(tf, tu, ts);
            for (auto val : rf) REQUIRE(simd::tou(val) == 0xdeadbeef);
            for (auto val : ru) REQUIRE(simd::tou(val) == 0xdeadbeef);
            for (auto val : rs) REQUIRE(simd::tou(val) == 0xdeadbeef);
        }
        {
            F tf(simd::sval(-123456789));
            U tu(simd::sval(-123456789));
            S ts(simd::sval(-123456789));
            tor(tf, tu, ts);
            for (auto val : rf) REQUIRE(simd::tos(val) == -123456789);
            for (auto val : ru) REQUIRE(simd::tos(val) == -123456789);
            for (auto val : rs) REQUIRE(simd::tos(val) == -123456789);
        }
        {
            F tf(simd::zero());
            U tu(simd::zero());
            S ts(simd::zero());
            tor(tf, tu, ts);
            for (auto val : rf) REQUIRE(simd::tou(val) == 0x00000000);
            for (auto val : ru) REQUIRE(simd::tou(val) == 0x00000000);
            for (auto val : rs) REQUIRE(simd::tou(val) == 0x00000000);
        }
        {
            F tf(simd::all_bits());
            U tu(simd::all_bits());
            S ts(simd::all_bits());
            tor(tf, tu, ts);
            for (auto val : rf) REQUIRE(simd::tou(val) == 0xffffffff);
            for (auto val : ru) REQUIRE(simd::tou(val) == 0xffffffff);
            for (auto val : rs) REQUIRE(simd::tou(val) == 0xffffffff);
        }
        {
            F tf(simd::sign_bit());
            U tu(simd::sign_bit());
            S ts(simd::sign_bit());
            tor(tf, tu, ts);
            for (auto val : rf) REQUIRE(simd::tou(val) == 0x80000000);
            for (auto val : ru) REQUIRE(simd::tou(val) == 0x80000000);
            for (auto val : rs) REQUIRE(simd::tou(val) == 0x80000000);
        }
        {
            F tf(simd::abs_mask());
            U tu(simd::abs_mask());
            S ts(simd::abs_mask());
            tor(tf, tu, ts);
            for (auto val : rf) REQUIRE(simd::tou(val) == 0x7fffffff);
            for (auto val : ru) REQUIRE(simd::tou(val) == 0x7fffffff);
            for (auto val : rs) REQUIRE(simd::tou(val) == 0x7fffffff);
        }
    }
}


TEST_CASE(SIMD_TYPE " implicit construction", SIMD_TEST_TAG) {
    simd::storage<F> rf = bufZF;
    simd::storage<U> ru = bufZU;
    simd::storage<S> rs = bufZS;

    auto implicit_test = [&rf, &ru, &rs](const F& tf, const U& tu, const S& ts) {
        rf = tf; ru = tu; rs = ts;
    };

    SECTION("from scalar_t") {
        implicit_test(1.2345678f, 123456789U, -123456789);
        for (auto val : rf) REQUIRE(val == 1.2345678f);
        for (auto val : ru) REQUIRE(val == 123456789U);
        for (auto val : rs) REQUIRE(val == -123456789);
    }
    SECTION("from vector_t") {
        implicit_test(
            SIMD_LOAD_AF,
            SIMD_LOAD_AU,
            SIMD_LOAD_AS);
        REQUIRE(rf == bufAF);
        REQUIRE(ru == bufAU);
        REQUIRE(rs == bufAS);
    }
    SECTION("from aligned pointer") {
        implicit_test(
            simd::aligned(bufAF.data()),
            simd::aligned(bufAU.data()),
            simd::aligned(bufAS.data()));
        REQUIRE(rf == bufAF);
        REQUIRE(ru == bufAU);
        REQUIRE(rs == bufAS);
    }
    SECTION("from unaligned pointer") {
        implicit_test(
            simd::unaligned(bufAF.data()),
            simd::unaligned(bufAU.data()),
            simd::unaligned(bufAS.data()));
        REQUIRE(rf == bufAF);
        REQUIRE(ru == bufAU);
        REQUIRE(rs == bufAS);
    }
    SECTION("from simd::storage") {
        simd::storage<F> storf;
        simd::storage<U> storu;
        simd::storage<S> stors;
        storf = bufAF;
        storu = bufAU;
        stors = bufAS;
        implicit_test(storf, storu, stors);
        REQUIRE(rf == bufAF);
        REQUIRE(ru == bufAU);
        REQUIRE(rs == bufAS);
    }
    SECTION("from uval, sval, zero, etc. (simd::init family)") {
        implicit_test(simd::fval(1.2345678f), simd::fval(1.2345678f), simd::fval(1.2345678f));
        for (auto val : rf) REQUIRE(simd::tof(val) == 1.2345678f);
        for (auto val : ru) REQUIRE(simd::tof(val) == 1.2345678f);
        for (auto val : rs) REQUIRE(simd::tof(val) == 1.2345678f);
        implicit_test(simd::uval(0xdeadbeef), simd::uval(0xdeadbeef), simd::uval(0xdeadbeef));
        for (auto val : rf) REQUIRE(simd::tou(val) == 0xdeadbeef);
        for (auto val : ru) REQUIRE(simd::tou(val) == 0xdeadbeef);
        for (auto val : rs) REQUIRE(simd::tou(val) == 0xdeadbeef);
        implicit_test(simd::sval(-123456789), simd::sval(-123456789), simd::sval(-123456789));
        for (auto val : rf) REQUIRE(simd::tos(val) == -123456789);
        for (auto val : ru) REQUIRE(simd::tos(val) == -123456789);
        for (auto val : rs) REQUIRE(simd::tos(val) == -123456789);
        implicit_test(simd::zero(), simd::zero(), simd::zero());
        for (auto val : rf) REQUIRE(simd::tou(val) == 0x00000000);
        for (auto val : ru) REQUIRE(simd::tou(val) == 0x00000000);
        for (auto val : rs) REQUIRE(simd::tou(val) == 0x00000000);
        implicit_test(simd::all_bits(), simd::all_bits(), simd::all_bits());
        for (auto val : rf) REQUIRE(simd::tou(val) == 0xffffffff);
        for (auto val : ru) REQUIRE(simd::tou(val) == 0xffffffff);
        for (auto val : rs) REQUIRE(simd::tou(val) == 0xffffffff);
        implicit_test(simd::sign_bit(), simd::sign_bit(), simd::sign_bit());
        for (auto val : rf) REQUIRE(simd::tou(val) == 0x80000000);
        for (auto val : ru) REQUIRE(simd::tou(val) == 0x80000000);
        for (auto val : rs) REQUIRE(simd::tou(val) == 0x80000000);
        implicit_test(simd::abs_mask(), simd::abs_mask(), simd::abs_mask());
        for (auto val : rf) REQUIRE(simd::tou(val) == 0x7fffffff);
        for (auto val : ru) REQUIRE(simd::tou(val) == 0x7fffffff);
        for (auto val : rs) REQUIRE(simd::tou(val) == 0x7fffffff);
    }
}

TEST_CASE(SIMD_TYPE " assignment", SIMD_TEST_TAG) {
    simd::storage<F> rf = bufZF;
    simd::storage<U> ru = bufZU;
    simd::storage<S> rs = bufZS;
    F tf; U tu; S ts;

    auto tor = [&rf, &tf, &ru, &tu, &rs, &ts]() {
        rf = tf;
        ru = tu;
        rs = ts;
    };

    SECTION("from scalar_t") {
        tf = 1.2345678f;
        tu = 123456789U;
        ts = -123456789;
        tor();
        for (auto val : rf) REQUIRE(val == 1.2345678f);
        for (auto val : ru) REQUIRE(val == 123456789U);
        for (auto val : rs) REQUIRE(val == -123456789);
    }
    SECTION("from vector_t") {
        tf = SIMD_LOAD_AF;
        tu = SIMD_LOAD_AU;
        ts = SIMD_LOAD_AS;
        tor();
        REQUIRE(rf == bufAF);
        REQUIRE(ru == bufAU);
        REQUIRE(rs == bufAS);
    }
    SECTION("from aligned pointer") {
        tf = bufAF;
        tu = bufAU;
        ts = bufAS;
        tor();
        REQUIRE(rf == bufAF);
        REQUIRE(ru == bufAU);
        REQUIRE(rs == bufAS);
    }
    SECTION("from unaligned pointer") {
        tf = bufAF;
        tu = bufAU;
        ts = bufAS;
        tor();
        REQUIRE(rf == bufAF);
        REQUIRE(ru == bufAU);
        REQUIRE(rs == bufAS);
    }
    SECTION("from simd::storage") {
        simd::storage<F> storf;
        simd::storage<U> storu;
        simd::storage<S> stors;
        storf = bufAF;
        storu = bufAU;
        stors = bufAS;
        tf = storf;
        tu = storu;
        ts = stors;
        tor();
        REQUIRE(rf == bufAF);
        REQUIRE(ru == bufAU);
        REQUIRE(rs == bufAS);
    }
    SECTION("from uval, sval, zero, etc. (simd::init family)") {
        tf = simd::fval(1.2345678f);
        tu = simd::fval(1.2345678f);
        ts = simd::fval(1.2345678f);
        tor();
        for (auto val : rf) REQUIRE(simd::tof(val) == 1.2345678f);
        for (auto val : ru) REQUIRE(simd::tof(val) == 1.2345678f);
        for (auto val : rs) REQUIRE(simd::tof(val) == 1.2345678f);
        tf = simd::uval(0xdeadbeef);
        tu = simd::uval(0xdeadbeef);
        ts = simd::uval(0xdeadbeef);
        tor();
        for (auto val : rf) REQUIRE(simd::tou(val) == 0xdeadbeef);
        for (auto val : ru) REQUIRE(simd::tou(val) == 0xdeadbeef);
        for (auto val : rs) REQUIRE(simd::tou(val) == 0xdeadbeef);
        tf = simd::sval(-123456789);
        tu = simd::sval(-123456789);
        ts = simd::sval(-123456789);
        tor();
        for (auto val : rf) REQUIRE(simd::tos(val) == -123456789);
        for (auto val : ru) REQUIRE(simd::tos(val) == -123456789);
        for (auto val : rs) REQUIRE(simd::tos(val) == -123456789);
        tf = simd::zero();
        tu = simd::zero();
        ts = simd::zero();
        tor();
        for (auto val : rf) REQUIRE(simd::tou(val) == 0x00000000);
        for (auto val : ru) REQUIRE(simd::tou(val) == 0x00000000);
        for (auto val : rs) REQUIRE(simd::tou(val) == 0x00000000);
        tf = simd::all_bits();
        tu = simd::all_bits();
        ts = simd::all_bits();
        tor();
        for (auto val : rf) REQUIRE(simd::tou(val) == 0xffffffff);
        for (auto val : ru) REQUIRE(simd::tou(val) == 0xffffffff);
        for (auto val : rs) REQUIRE(simd::tou(val) == 0xffffffff);
        tf = simd::sign_bit();
        tu = simd::sign_bit();
        ts = simd::sign_bit();
        tor();
        for (auto val : rf) REQUIRE(simd::tou(val) == 0x80000000);
        for (auto val : ru) REQUIRE(simd::tou(val) == 0x80000000);
        for (auto val : rs) REQUIRE(simd::tou(val) == 0x80000000);
        tf = simd::abs_mask();
        tu = simd::abs_mask();
        ts = simd::abs_mask();
        tor();
        for (auto val : rf) REQUIRE(simd::tou(val) == 0x7fffffff);
        for (auto val : ru) REQUIRE(simd::tou(val) == 0x7fffffff);
        for (auto val : rs) REQUIRE(simd::tou(val) == 0x7fffffff);
    }
}

TEST_CASE(SIMD_TYPE " type conversion", SIMD_TEST_TAG) {
    SECTION("int to float") {
        simd::storage<F> expected, result;
        std::transform(begin(bufAS), end(bufAS), begin(expected), [](S::scalar_t a) {
            return static_cast<F::scalar_t>(a);
        });
        S in = bufAS;
        result = F(in);
        REQUIRE(result == expected);
    }
    SECTION("float to int") {
        simd::storage<S> expected, result;
        std::transform(begin(bufAF), end(bufAF), begin(expected), [](F::scalar_t a) {
            return simd::round_to_int32(a);
        });
        F in = bufAF;
        result = S(in);
        REQUIRE(result == expected);
    }
    SECTION("int to uint") {
        simd::storage<U> expected, result;
        std::transform(begin(bufAS), end(bufAS), begin(expected), [](S::scalar_t a) {
            return static_cast<U::scalar_t>(a);
        });
        S in = bufAS;
        result = U(in);
        REQUIRE(result == expected);
    }
    SECTION("uint to int") {
        simd::storage<S> expected, result;
        std::transform(begin(bufAU), end(bufAU), begin(expected), [](U::scalar_t a) {
            return simd::round_to_int32(a);
        });
        U in = bufAU;
        result = S(in);
        REQUIRE(result == expected);
    }
}

TEST_CASE(SIMD_TYPE " float arithmetic", SIMD_TEST_TAG) {
    simd::storage<F> r, e;
    F a = bufAF;
    F b = bufBF;

    SECTION("unary plus") {
        std::transform(begin(bufAF), end(bufAF), begin(e), [](F::scalar_t a) { return +a; });
        r = +a;
        REQUIRE(r == e);
    }
    SECTION("unary minus") {
        std::transform(begin(bufAF), end(bufAF), begin(e), std::negate<F::scalar_t>());
        r = -a;
        REQUIRE(r == e);
    }
    SECTION("plus") {
        std::transform(begin(bufAF), end(bufAF), begin(bufBF), begin(e), std::plus<F::scalar_t>());
        r = a + b;
        REQUIRE(r == e);
        a += b;
        r = a;
        REQUIRE(r == e);
    }
    SECTION("minus") {
        std::transform(begin(bufAF), end(bufAF), begin(bufBF), begin(e), std::minus<F::scalar_t>());
        r = a - b;
        REQUIRE(r == e);
        a -= b;
        r = a;
        REQUIRE(r == e);
    }
    SECTION("multiplies") {
        std::transform(begin(bufAF), end(bufAF), begin(bufBF), begin(e), std::multiplies<F::scalar_t>());
        r = a * b;
        REQUIRE(r == e);
        a *= b;
        r = a;
        REQUIRE(r == e);
    }
    SECTION("divides") {
        std::transform(begin(bufAF), end(bufAF), begin(bufBF), begin(e), std::divides<F::scalar_t>());
        r = a / b;
        REQUIRE(r == e);
        a /= b;
        r = a;
        REQUIRE(r == e);
    }
    SECTION("minimum") {
        std::transform(begin(bufAF), end(bufAF), begin(bufBF), begin(e), [](F::scalar_t l, F::scalar_t r) {
            return std::min(l, r);
        });
        r = min(a, b);
        REQUIRE(r == e);
    }
    SECTION("maximum") {
        std::transform(begin(bufAF), end(bufAF), begin(bufBF), begin(e), [](F::scalar_t l, F::scalar_t r) {
            return std::max(l, r);
        });
        r = max(a, b);
        REQUIRE(r == e);
    }
    SECTION("absolute value") {
        std::transform(begin(bufAF), end(bufAF), begin(e), [](F::scalar_t a) { return std::abs(a); });
        r = abs(a);
        REQUIRE(r == e);
    }
    SECTION("signum") {
        std::transform(begin(bufAF), end(bufAF), begin(e), [](F::scalar_t a) { return std::signbit(a) ? F::scalar_t(-1) : F::scalar_t(1); });
        r = signum(a);
        REQUIRE(r == e);
    }
    SECTION("square root") {
        std::transform(begin(bufAF), end(bufAF), begin(e), [](F::scalar_t a) { return std::sqrt(std::abs(a)); });
        r = sqrt(abs(a));
        for (int i = 0; i < F::W; ++i) REQUIRE(r[i] == Approx(e[i]));
    }
    SECTION("fast reciprocal") {
        std::transform(begin(bufAF), end(bufAF), begin(e), [](F::scalar_t a) { return 1 / a; });
        r = rcp(a);
        for (int i = 0; i < F::W; ++i) REQUIRE(r[i] == Approx(e[i]).epsilon(0.001));
    }
    SECTION("fast reciprocal square root") {
        std::transform(begin(bufAF), end(bufAF), begin(e), [](F::scalar_t a) { return 1 / std::sqrt(std::abs(a)); });
        r = rsqrt(abs(a));
        for (int i = 0; i < F::W; ++i) REQUIRE(r[i] == Approx(e[i]).epsilon(0.001));
    }
}

TEST_CASE(SIMD_TYPE " int arithmetic", SIMD_TEST_TAG) {
    simd::storage<S> r, e;
    S a = bufAS;
    S b = bufBS;

    SECTION("unary plus") {
        std::transform(begin(bufAS), end(bufAS), begin(e), [](S::scalar_t a) { return +a; });
        r = +a;
        REQUIRE(r == e);
    }
    SECTION("unary minus") {
        std::transform(begin(bufAS), end(bufAS), begin(e), std::negate<S::scalar_t>());
        r = -a;
        REQUIRE(r == e);
    }
    SECTION("plus") {
        std::transform(begin(bufAS), end(bufAS), begin(bufBS), begin(e), std::plus<S::scalar_t>());
        r = a + b;
        REQUIRE(r == e);
        a += b;
        r = a;
        REQUIRE(r == e);
    }
    SECTION("minus") {
        std::transform(begin(bufAS), end(bufAS), begin(bufBS), begin(e), std::minus<S::scalar_t>());
        r = a - b;
        REQUIRE(r == e);
        a -= b;
        r = a;
        REQUIRE(r == e);
    }
    SECTION("multiplies") {
        std::transform(begin(bufAS), end(bufAS), begin(bufBS), begin(e), std::multiplies<S::scalar_t>());
        r = a * b;
        REQUIRE(r == e);
        a *= b;
        r = a;
        REQUIRE(r == e);
    }
    SECTION("minimum") {
        std::transform(begin(bufAS), end(bufAS), begin(bufBS), begin(e), [](S::scalar_t l, S::scalar_t r) {
            return std::min(l, r);
        });
        r = min(a, b);
        REQUIRE(r == e);
    }
    SECTION("maximum") {
        std::transform(begin(bufAS), end(bufAS), begin(bufBS), begin(e), [](S::scalar_t l, S::scalar_t r) {
            return std::max(l, r);
        });
        r = max(a, b);
        REQUIRE(r == e);
    }
    SECTION("absolute value") {
        std::transform(begin(bufAS), end(bufAS), begin(e), [](S::scalar_t a) { return std::abs(a); });
        r = abs(a);
        REQUIRE(r == e);
    }
    SECTION("signum") {
        std::transform(begin(bufAS), end(bufAS), begin(e), [](S::scalar_t a) { return (a < 0) ? S::scalar_t(-1) : S::scalar_t(1); });
        r = signum(a);
        REQUIRE(r == e);
    }
}

TEST_CASE(SIMD_TYPE " float comparison", SIMD_TEST_TAG) {
    auto if_ = [](bool in) { return in ? 0xffffffffU : 0x00000000U; };
    simd::storage<U> r, e;
    F a = bufAF;
    F b = bufBF;

    SECTION("equal to") {
        std::transform(begin(bufAF), end(bufAF), begin(bufBF), begin(e), [&if_](F::scalar_t a, F::scalar_t b) {
            return if_(a == b);
        });
        r = a == b;
        REQUIRE(r == e);
    }
    SECTION("not equal to") {
        std::transform(begin(bufAF), end(bufAF), begin(bufBF), begin(e), [&if_](F::scalar_t a, F::scalar_t b) {
            return if_(a != b);
        });
        r = a != b;
        REQUIRE(r == e);
    }
    SECTION("greater") {
        std::transform(begin(bufAF), end(bufAF), begin(bufBF), begin(e), [&if_](F::scalar_t a, F::scalar_t b) {
            return if_(a > b);
        });
        r = a > b;
        REQUIRE(r == e);
    }
    SECTION("less") {
        std::transform(begin(bufAF), end(bufAF), begin(bufBF), begin(e), [&if_](F::scalar_t a, F::scalar_t b) {
            return if_(a < b);
        });
        r = a < b;
        REQUIRE(r == e);
    }
    SECTION("greater equal") {
        std::transform(begin(bufAF), end(bufAF), begin(bufBF), begin(e), [&if_](F::scalar_t a, F::scalar_t b) {
            return if_(a >= b);
        });
        r = a >= b;
        REQUIRE(r == e);
    }
    SECTION("less equal") {
        std::transform(begin(bufAF), end(bufAF), begin(bufBF), begin(e), [&if_](F::scalar_t a, F::scalar_t b) {
            return if_(a <= b);
        });
        r = a <= b;
        REQUIRE(r == e);
    }
}

TEST_CASE(SIMD_TYPE " int comparison", SIMD_TEST_TAG) {
    auto if_ = [](bool in) { return in ? 0xffffffffU : 0x00000000U; };
    simd::storage<U> r, e;
    S a = bufAS;
    S b = bufBS;

    SECTION("equal to") {
        std::transform(begin(bufAS), end(bufAS), begin(bufBS), begin(e), [&if_](S::scalar_t a, S::scalar_t b) {
            return if_(a == b);
        });
        r = a == b;
        REQUIRE(r == e);
    }
    SECTION("not equal to") {
        std::transform(begin(bufAS), end(bufAS), begin(bufBS), begin(e), [&if_](S::scalar_t a, S::scalar_t b) {
            return if_(a != b);
        });
        r = a != b;
        REQUIRE(r == e);
    }
    SECTION("greater") {
        std::transform(begin(bufAS), end(bufAS), begin(bufBS), begin(e), [&if_](S::scalar_t a, S::scalar_t b) {
            return if_(a > b);
        });
        r = a > b;
        REQUIRE(r == e);
    }
    SECTION("less") {
        std::transform(begin(bufAS), end(bufAS), begin(bufBS), begin(e), [&if_](S::scalar_t a, S::scalar_t b) {
            return if_(a < b);
        });
        r = a < b;
        REQUIRE(r == e);
    }
    SECTION("greater equal") {
        std::transform(begin(bufAS), end(bufAS), begin(bufBS), begin(e), [&if_](S::scalar_t a, S::scalar_t b) {
            return if_(a >= b);
        });
        r = a >= b;
        REQUIRE(r == e);
    }
    SECTION("less equal") {
        std::transform(begin(bufAS), end(bufAS), begin(bufBS), begin(e), [&if_](S::scalar_t a, S::scalar_t b) {
            return if_(a <= b);
        });
        r = a <= b;
        REQUIRE(r == e);
    }
}

TEST_CASE(SIMD_TYPE " bitwise", SIMD_TEST_TAG) {
    simd::storage<U> r, e;
    U a = bufAU;
    U b = bufBU;

    SECTION("bit and") {
        std::transform(begin(bufAU), end(bufAU), begin(bufBU), begin(e), [](U::scalar_t a, U::scalar_t b) {
            return simd::tou(a) & simd::tou(b);
        });
        r = a & b;
        REQUIRE(r == e);
        a &= b;
        r = a;
        REQUIRE(r == e);
    }
    SECTION("bit or") {
        std::transform(begin(bufAU), end(bufAU), begin(bufBU), begin(e), [](U::scalar_t a, U::scalar_t b) {
            return simd::tou(a) | simd::tou(b);
        });
        r = a | b;
        REQUIRE(r == e);
        a |= b;
        r = a;
        REQUIRE(r == e);
    }
    SECTION("bit xor") {
        std::transform(begin(bufAU), end(bufAU), begin(bufBU), begin(e), [](U::scalar_t a, U::scalar_t b) {
            return simd::tou(a) ^ simd::tou(b);
        });
        r = a ^ b;
        REQUIRE(r == e);
        a ^= b;
        r = a;
        REQUIRE(r == e);
    }
	SECTION("bit andnot") {
		std::transform(begin(bufAU), end(bufAU), begin(bufBU), begin(e), [](U::scalar_t a, U::scalar_t b) {
			return simd::tou(a) & ~simd::tou(b);
		});
		r = a & ~b;
		REQUIRE(r == e);
		r = andnot(a, b);
		REQUIRE(r == e);
	}
    SECTION("bit not") {
        std::transform(begin(bufAU), end(bufAU), begin(e), [](U::scalar_t a) {
            return ~simd::tou(a);
        });
        r = ~a;
        REQUIRE(r == e);
    }
    SECTION("complex expr") {
        std::transform(begin(bufAU), end(bufAU), begin(bufBU), begin(e), [](U::scalar_t af, U::scalar_t bf) {
            auto a = simd::tou(af);
            auto b = simd::tou(bf);
            return ~((~a & ~b) | (~a ^ ~b));
        });
        r = ~((~a & ~b) | (~a ^ ~b));
        REQUIRE(r == e);
    }
}

TEST_CASE(SIMD_TYPE " horizontal operations", SIMD_TEST_TAG) {
    F a = bufAF;
    auto max = [](F::scalar_t l, F::scalar_t r) { return std::max(l, r); };
    auto min = [](F::scalar_t l, F::scalar_t r) { return std::min(l, r); };
    F::scalar_t inf = std::numeric_limits<F::scalar_t>::infinity();
    F::scalar_t v, v0;

    SECTION("max") {
        v0 = std::accumulate(begin(bufAF), end(bufAF), -inf, max);
        v = a.reduce(simd::max).first_element();
        REQUIRE(v == v0);
        v = max_mask(a, simd::zero()).reduce(simd::max).first_element();
        REQUIRE(v == -inf);
    }
    SECTION("min") {
        v0 = std::accumulate(begin(bufAF), end(bufAF), inf, min);
        v = a.reduce(simd::min).first_element();
        REQUIRE(v == v0);
        v = min_mask(a, simd::zero()).reduce(simd::min).first_element();
        REQUIRE(v == inf);
    }
    SECTION("sum") {
        v0 = std::accumulate(begin(bufAF), end(bufAF), F::scalar_t(0));
        v = a.reduce(simd::operator+).first_element();
        REQUIRE(v == Approx(v0));
        v = sum_mask(a, simd::zero()).reduce(simd::operator+).first_element();
        REQUIRE(v == 0);
    }
    SECTION("product") {
        v0 = std::accumulate(begin(bufAF), end(bufAF), F::scalar_t(1), std::multiplies<F::scalar_t>());
        v = a.reduce(simd::operator*).first_element();
        REQUIRE(v == Approx(v0));
        v = product_mask(a, simd::zero()).reduce(simd::operator*).first_element();
        REQUIRE(v == 1);
    }
}

TEST_CASE(SIMD_TYPE " conditional", SIMD_TEST_TAG) {
    F af = bufAF;
    U au = bufAU;
    S as = bufAS;
    F bf = bufBF;
    U bu = bufBU;
    S bs = bufBS;

    U sel = af >= bf;
    auto mask = sel.mask();

    simd::storage<F> rf(cond(sel, af, bf));
    simd::storage<U> ru(cond(sel, au, bu));
    simd::storage<S> rs(cond(sel, as, bs));

    for (int i = 0; i < F::W; ++i) {
        REQUIRE((rf[i]) == (mask[i] ? bufAF[i] : bufBF[i]));
        REQUIRE((ru[i]) == (mask[i] ? bufAU[i] : bufBU[i]));
        REQUIRE((rs[i]) == (mask[i] ? bufAS[i] : bufBS[i]));
    }
}

TEST_CASE(SIMD_TYPE " expression template compatibility", SIMD_TEST_TAG) {
    U in = bufAU;
    U vec1 = ~in;
    auto vec2 = ~in;

    REQUIRE(vec1.mask() == vec2.mask());
    REQUIRE(vec1.first_element() == vec2.first_element());
}
