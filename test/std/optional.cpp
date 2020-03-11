// Copyright (C) 2011 - 2017 Andrzej Krzemienski.
//
// Use, modification, and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// The idea and interface is based on Boost.Optional library
// authored by Fernando Luis Cacciola Carballal

#include "mapbox/std/optional.hpp"

#include <gtest/gtest.h>

#include <complex>
#include <functional>
#include <iostream>
#include <vector>

enum State {
    sDefaultConstructed,
    sValueCopyConstructed,
    sValueMoveConstructed,
    sCopyConstructed,
    sMoveConstructed,
    sMoveAssigned,
    sCopyAssigned,
    sValueCopyAssigned,
    sValueMoveAssigned,
    sMovedFrom,
    sValueConstructed
};

struct OracleVal {
    State s;
    int i;
    OracleVal(int i = 0) : s(sValueConstructed), i(i) {}
};

struct Oracle {
    State s;
    OracleVal val;

    Oracle() : s(sDefaultConstructed) {}
    Oracle(const OracleVal& v) : s(sValueCopyConstructed), val(v) {}
    Oracle(OracleVal&& v) : s(sValueMoveConstructed), val(std::move(v)) { v.s = sMovedFrom; }
    Oracle(const Oracle& o) : s(sCopyConstructed), val(o.val) {}
    Oracle(Oracle&& o) : s(sMoveConstructed), val(std::move(o.val)) { o.s = sMovedFrom; }

    Oracle& operator=(const OracleVal& v) {
        s = sValueCopyConstructed;
        val = v;
        return *this;
    }
    Oracle& operator=(OracleVal&& v) {
        s = sValueMoveConstructed;
        val = std::move(v);
        v.s = sMovedFrom;
        return *this;
    }
    Oracle& operator=(const Oracle& o) {
        s = sCopyConstructed;
        val = o.val;
        return *this;
    }
    Oracle& operator=(Oracle&& o) {
        s = sMoveConstructed;
        val = std::move(o.val);
        o.s = sMovedFrom;
        return *this;
    }
};

struct Guard {
    std::string val;
    Guard() : val{} {}
    explicit Guard(std::string s, int = 0) : val(s) {}
    Guard(const Guard&) = delete;
    Guard(Guard&&) = delete;
    void operator=(const Guard&) = delete;
    void operator=(Guard&&) = delete;
};

struct ExplicitStr {
    std::string s;
    explicit ExplicitStr(const char* chp) : s(chp) {}
};

struct Date {
    int i;
    Date() = delete;
    Date(int i) : i{i} {}
    Date(Date&& d) : i(d.i) { d.i = 0; }
    Date(const Date&) = delete;
    Date& operator=(const Date&) = delete;
    Date& operator=(Date&& d) {
        i = d.i;
        d.i = 0;
        return *this;
    }
};

bool operator==(Oracle const& a, Oracle const& b) {
    return a.val.i == b.val.i;
}
bool operator!=(Oracle const& a, Oracle const& b) {
    return a.val.i != b.val.i;
}

namespace tr2 = mapbox::base;

TEST(Optional, disengaged_ctor) {
    tr2::optional<int> o1;
    EXPECT_FALSE(o1);

    tr2::optional<int> o2 = tr2::nullopt;
    EXPECT_FALSE(o2);

    tr2::optional<int> o3 = o2;
    EXPECT_FALSE(o3);

    EXPECT_EQ(o1, tr2::nullopt);
    EXPECT_EQ(o1, tr2::optional<int>{});
    EXPECT_FALSE(o1);
    EXPECT_EQ(bool(o1), false);

    EXPECT_EQ(o2, tr2::nullopt);
    EXPECT_EQ(o2, tr2::optional<int>{});
    EXPECT_FALSE(o2);
    EXPECT_EQ(bool(o2), false);

    EXPECT_EQ(o3, tr2::nullopt);
    EXPECT_EQ(o3, tr2::optional<int>{});
    EXPECT_FALSE(o3);
    EXPECT_EQ(bool(o3), false);

    EXPECT_EQ(o1, o2);
    EXPECT_EQ(o2, o1);
    EXPECT_EQ(o1, o3);
    EXPECT_EQ(o3, o1);
    EXPECT_EQ(o2, o3);
    EXPECT_EQ(o3, o2);
};

TEST(Optional, value_ctor) {
    OracleVal v;
    tr2::optional<Oracle> oo1(v);

    EXPECT_NE(oo1, tr2::nullopt);
    EXPECT_NE(oo1, tr2::optional<Oracle>{});
    EXPECT_EQ(oo1, tr2::optional<Oracle>{v});

    EXPECT_TRUE(!!oo1);
    EXPECT_TRUE(bool(oo1));
    // NA: assert (oo1->s == sValueCopyConstructed);
    EXPECT_EQ(oo1->s, sMoveConstructed);
    EXPECT_EQ(v.s, sValueConstructed);

    tr2::optional<Oracle> oo2(std::move(v));
    EXPECT_NE(oo2, tr2::nullopt);
    EXPECT_NE(oo2, tr2::optional<Oracle>{});
    EXPECT_EQ(oo2, oo1);
    EXPECT_TRUE(!!oo2);
    EXPECT_TRUE(bool(oo2));
    // NA: assert (oo2->s == sValueMoveConstructed);
    EXPECT_EQ(oo2->s, sMoveConstructed);
    EXPECT_EQ(v.s, sMovedFrom);

    {
        OracleVal v;
        tr2::optional<Oracle> oo1{tr2::in_place, v};
        EXPECT_NE(oo1, tr2::nullopt);
        EXPECT_NE(oo1, tr2::optional<Oracle>{});
        EXPECT_EQ(oo1, tr2::optional<Oracle>{v});
        EXPECT_TRUE(!!oo1);
        EXPECT_TRUE(bool(oo1));
        EXPECT_EQ(oo1->s, sValueCopyConstructed);
        EXPECT_EQ(v.s, sValueConstructed);

        tr2::optional<Oracle> oo2{tr2::in_place, std::move(v)};
        EXPECT_NE(oo2, tr2::nullopt);
        EXPECT_NE(oo2, tr2::optional<Oracle>{});
        EXPECT_EQ(oo2, oo1);
        EXPECT_TRUE(!!oo2);
        EXPECT_TRUE(bool(oo2));
        EXPECT_EQ(oo2->s, sValueMoveConstructed);
        EXPECT_EQ(v.s, sMovedFrom);
    }
};

TEST(Optional, assignment) {
    tr2::optional<int> oi;
    oi = tr2::optional<int>{1};
    EXPECT_EQ(*oi, 1);

    oi = tr2::nullopt;
    EXPECT_FALSE(oi);

    oi = 2;
    EXPECT_EQ(*oi, 2);

    oi = {};
    EXPECT_FALSE(oi);
};

template <class T>
struct MoveAware {
    T val;
    bool moved;
    MoveAware(T val) : val(val), moved(false) {}
    MoveAware(MoveAware const&) = delete;
    MoveAware(MoveAware&& rhs) : val(rhs.val), moved(rhs.moved) { rhs.moved = true; }
    MoveAware& operator=(MoveAware const&) = delete;
    MoveAware& operator=(MoveAware&& rhs) {
        val = (rhs.val);
        moved = (rhs.moved);
        rhs.moved = true;
        return *this;
    }
};

TEST(Optional, moved_from_state) {
    // first, test mock:
    MoveAware<int> i{1}, j{2};
    EXPECT_EQ(i.val, 1);
    EXPECT_FALSE(i.moved);
    EXPECT_EQ(j.val, 2);
    EXPECT_FALSE(j.moved);

    MoveAware<int> k = std::move(i);
    EXPECT_EQ(k.val, 1);
    EXPECT_FALSE(k.moved);
    EXPECT_EQ(i.val, 1);
    EXPECT_TRUE(i.moved);

    k = std::move(j);
    EXPECT_EQ(k.val, 2);
    EXPECT_FALSE(k.moved);
    EXPECT_EQ(j.val, 2);
    EXPECT_TRUE(j.moved);

    // now, test optional
    tr2::optional<MoveAware<int>> oi{1}, oj{2};
    EXPECT_TRUE(oi);
    EXPECT_FALSE(oi->moved);
    EXPECT_TRUE(oj);
    EXPECT_FALSE(oj->moved);

    tr2::optional<MoveAware<int>> ok = std::move(oi);
    EXPECT_TRUE(ok);
    EXPECT_FALSE(ok->moved);
    EXPECT_TRUE(oi);
    EXPECT_TRUE(oi->moved);

    ok = std::move(oj);
    EXPECT_TRUE(ok);
    EXPECT_FALSE(ok->moved);
    EXPECT_TRUE(oj);
    EXPECT_TRUE(oj->moved);
};

TEST(Optional, copy_move_ctor_optional_int) {
    tr2::optional<int> oi;
    tr2::optional<int> oj = oi;

    EXPECT_FALSE(oj);
    EXPECT_EQ(oj, oi);
    EXPECT_EQ(oj, tr2::nullopt);
    EXPECT_FALSE(bool(oj));

    oi = 1;
    tr2::optional<int> ok = oi;
    EXPECT_TRUE(!!ok);
    EXPECT_TRUE(bool(ok));
    EXPECT_EQ(ok, oi);
    EXPECT_NE(ok, oj);
    EXPECT_EQ(*ok, 1);

    tr2::optional<int> ol = std::move(oi);
    EXPECT_TRUE(!!ol);
    EXPECT_TRUE(bool(ol));
    EXPECT_EQ(ol, oi);
    EXPECT_NE(ol, oj);
    EXPECT_EQ(*ol, 1);
};

TEST(Optional, optional_optional) {
    tr2::optional<tr2::optional<int>> oi1 = tr2::nullopt;

    EXPECT_EQ(oi1, tr2::nullopt);
    EXPECT_FALSE(oi1);

    {
        tr2::optional<tr2::optional<int>> oi2{tr2::in_place};
        EXPECT_NE(oi2, tr2::nullopt);
        EXPECT_TRUE(bool(oi2));
        EXPECT_EQ(*oi2, tr2::nullopt);
        // EXPECT_FALSE ((*oi2));
        // std::cout << typeid(**oi2).name() << std::endl;
    }

    {
        tr2::optional<tr2::optional<int>> oi2{tr2::in_place, tr2::nullopt};
        EXPECT_NE(oi2, tr2::nullopt);
        EXPECT_TRUE(bool(oi2));
        EXPECT_EQ(*oi2, tr2::nullopt);
        EXPECT_FALSE(*oi2);
    }

    {
        tr2::optional<tr2::optional<int>> oi2{tr2::optional<int>{}};
        EXPECT_NE(oi2, tr2::nullopt);
        EXPECT_TRUE(bool(oi2));
        EXPECT_EQ(*oi2, tr2::nullopt);
        EXPECT_FALSE(*oi2);
    }

    tr2::optional<int> oi;
    auto ooi = tr2::make_optional(oi);
    static_assert(std::is_same<tr2::optional<tr2::optional<int>>, decltype(ooi)>::value, "");
};

TEST(Optional, example_guard_1) {
    using namespace tr2;
    // FAILS: optional<Guard> ogx(Guard("res1"));
    // FAILS: optional<Guard> ogx = "res1";
    // FAILS: optional<Guard> ogx("res1");
    optional<Guard> oga;                   // Guard is non-copyable (and non-moveable)
    optional<Guard> ogb(in_place, "res1"); // initialzes the contained value with "res1"
    EXPECT_TRUE(bool(ogb));
    EXPECT_EQ(ogb->val, "res1");

    optional<Guard> ogc(in_place); // default-constructs the contained value
    EXPECT_TRUE(bool(ogc));
    EXPECT_EQ(ogc->val, "");

    oga.emplace("res1"); // initialzes the contained value with "res1"
    EXPECT_TRUE(bool(oga));
    EXPECT_EQ(oga->val, "res1");

    oga.emplace(); // destroys the contained value and
                   // default-constructs the new one
    EXPECT_TRUE(bool(oga));
    EXPECT_EQ(oga->val, "");

    oga = nullopt; // OK: make disengaged the optional Guard
    EXPECT_FALSE(oga);
    // FAILS: ogb = {};                          // ERROR: Guard is not Moveable
};

void process() {}
void process(int) {}
void processNil() {}

TEST(Optional, example1) {
    using namespace tr2;

    optional<int> oi;           // create disengaged object
    optional<int> oj = nullopt; // alternative syntax
    oi = oj;                    // assign disengaged object
    optional<int> ok = oj;      // ok is disengaged

    if (oi) EXPECT_TRUE(false); // 'if oi is engaged...'
    if (!oi) EXPECT_TRUE(true); // 'if oi is disengaged...'

    if (oi != nullopt) EXPECT_TRUE(false); // 'if oi is engaged...'
    if (oi == nullopt) EXPECT_TRUE(true);  // 'if oi is disengaged...'

    EXPECT_EQ(oi, ok); // two disengaged optionals compare equal

    ///////////////////////////////////////////////////////////////////////////
    optional<int> ol{1}; // ol is engaged; its contained value is 1
    ok = 2;              // ok becomes engaged; its contained value is 2
    oj = ol;             // oj becomes engaged; its contained value is 1

    EXPECT_NE(oi, ol); // disengaged != engaged
    EXPECT_NE(ok, ol); // different contained values
    EXPECT_EQ(oj, ol); // same contained value
    EXPECT_LT(oi, ol); // disengaged < engaged
    EXPECT_LT(ol, ok); // less by contained value

    /////////////////////////////////////////////////////////////////////////////
    optional<int> om{1};   // om is engaged; its contained value is 1
    optional<int> on = om; // on is engaged; its contained value is 1
    om = 2;                // om is engaged; its contained value is 2
    EXPECT_NE(on, om);     // on still contains 3. They are not pointers

    /////////////////////////////////////////////////////////////////////////////
    int i = *ol; // i obtains the value contained in ol
    EXPECT_EQ(i, 1);
    *ol = 9; // the object contained in ol becomes 9
    EXPECT_EQ(*ol, 9);
    EXPECT_EQ(ol, make_optional(9));

    ///////////////////////////////////
    int p = 1;
    optional<int> op = p;
    EXPECT_EQ(*op, 1);
    p = 2;
    EXPECT_EQ(*op, 1); // value contained in op is separated from p

    ////////////////////////////////
    if (ol)
        process(*ol); // use contained value if present
    else
        process(); // proceed without contained value

    if (!om)
        processNil();
    else
        process(*om);

    /////////////////////////////////////////
    process(ol.value_or(0)); // use 0 if ol is disengaged

    ////////////////////////////////////////////
    ok = nullopt; // if ok was engaged calls T's dtor
    oj = {};      // assigns a temporary disengaged optional
};

TEST(Optional, example_guard_2) {
    using std::experimental::optional;
    const optional<int> c = 4;
    int i = *c; // i becomes 4
    EXPECT_EQ(i, 4);
    // FAILS: *c = i;                            // ERROR: cannot assign to const int&
};

TEST(Optional, example_ref) {
    int i = 1;
    int j = 2;
    tr2::optional<int&> ora;     // disengaged optional reference to int
    tr2::optional<int&> orb = i; // contained reference refers to object i

    *orb = 3; // i becomes 3
    // FAILS: ora = j;                           // ERROR: optional refs do not have assignment from T
    // FAILS: ora = {j};                         // ERROR: optional refs do not have copy/move assignment
    // FAILS: ora = orb;                         // ERROR: no copy/move assignment
    ora.emplace(j); // OK: contained reference refers to object j
    ora.emplace(i); // OK: contained reference now refers to object i

    ora = tr2::nullopt; // OK: ora becomes disengaged
};

template <typename T>
T getValue(tr2::optional<T> newVal = tr2::nullopt, tr2::optional<T&> storeHere = tr2::nullopt) {
    T cached{};

    if (newVal) {
        cached = *newVal;

        if (storeHere) {
            *storeHere = *newVal; // LEGAL: assigning T to T
        }
    }
    return cached;
}

TEST(Optional, example_optional_arg) {
    int iii = 0;
    iii = getValue<int>(iii, iii);
    iii = getValue<int>(iii);
    iii = getValue<int>();

    {
        tr2::optional<Guard> grd1{tr2::in_place, "res1", 1}; // guard 1 initialized
        tr2::optional<Guard> grd2;

        grd2.emplace("res2", 2); // guard 2 initialized
        grd1 = tr2::nullopt;     // guard 1 released

    } // guard 2 released (in dtor)
};

std::tuple<Date, Date, Date> getStartMidEnd() {
    return std::tuple<Date, Date, Date>{Date{1}, Date{2}, Date{3}};
}
void run(Date const&, Date const&, Date const&) {}

TEST(Optional, example_date) {
    tr2::optional<Date> start, mid, end; // Date doesn't have default ctor (no good default date)

    std::tie(start, mid, end) = getStartMidEnd();
    run(*start, *mid, *end);
};

tr2::optional<char> readNextChar() {
    return {};
}

void run(tr2::optional<std::string>) {}
void run(std::complex<double>) {}

template <class T>
void assign_norebind(tr2::optional<T&>& optref, T& obj) {
    if (optref)
        *optref = obj;
    else
        optref.emplace(obj);
}

template <typename T>
void unused(T&&) {}

TEST(Optional, example_conceptual_model) {
    tr2::optional<int> oi = 0;
    tr2::optional<int> oj = 1;
    tr2::optional<int> ok = tr2::nullopt;

    oi = 1;
    oj = tr2::nullopt;
    ok = 0;

    unused(oi == tr2::nullopt);
    unused(oj == 0);
    unused(ok == 1);
};

TEST(Optional, example_rationale) {
    if (tr2::optional<char> ch = readNextChar()) {
        unused(ch);
        // ...
    }

    //////////////////////////////////
    tr2::optional<int> opt1 = tr2::nullopt;
    tr2::optional<int> opt2 = {};

    opt1 = tr2::nullopt;
    opt2 = {};

    if (opt1 == tr2::nullopt) {
    }
    if (!opt2) {
    }
    if (opt2 == tr2::optional<int>{}) {
    }

    ////////////////////////////////

    run(tr2::nullopt); // pick the second overload
    // FAILS: run({});              // ambiguous

    if (opt1 == tr2::nullopt) {
    } // fine
    // FAILS: if (opt2 == {}) {}   // ilegal

    ////////////////////////////////
    EXPECT_LT(tr2::optional<unsigned>{}, tr2::optional<unsigned>{0});
    EXPECT_LT(tr2::optional<unsigned>{0}, tr2::optional<unsigned>{1});
    EXPECT_FALSE((tr2::optional<unsigned>{} < tr2::optional<unsigned>{}));
    EXPECT_FALSE((tr2::optional<unsigned>{1} < tr2::optional<unsigned>{1}));

    EXPECT_NE(tr2::optional<unsigned>{}, tr2::optional<unsigned>{0});
    EXPECT_NE(tr2::optional<unsigned>{0}, tr2::optional<unsigned>{1});
    EXPECT_EQ(tr2::optional<unsigned>{}, tr2::optional<unsigned>{});
    EXPECT_EQ(tr2::optional<unsigned>{0}, tr2::optional<unsigned>{0});

    /////////////////////////////////
    tr2::optional<int> o;
    o = tr2::make_optional(1); // copy/move assignment
    o = 1;                     // assignment from T
    o.emplace(1);              // emplacement

    ////////////////////////////////////
    int isas = 0, i = 9;
    tr2::optional<int&> asas = i;
    assign_norebind(asas, isas);

    /////////////////////////////////////
    ////tr2::optional<std::vector<int>> ov2 = {2, 3};
    ////EXPECT_TRUE (bool(ov2));
    ////EXPECT_EQ ((*ov2)[1], 3);
    ////
    ////////////////////////////////
    ////std::vector<int> v = {1, 2, 4, 8};
    ////optional<std::vector<int>> ov = {1, 2, 4, 8};

    ////EXPECT_EQ (v, *ov);
    ////
    ////ov = {1, 2, 4, 8};

    ////std::allocator<int> a;
    ////optional<std::vector<int>> ou { in_place, {1, 2, 4, 8}, a };

    ////EXPECT_EQ (ou, ov);

    //////////////////////////////
    // inconvenient syntax:
    {
        tr2::optional<std::vector<int>> ov2{tr2::in_place, {2, 3}};

        EXPECT_TRUE(bool(ov2));
        EXPECT_EQ((*ov2)[1], 3);

        ////////////////////////////

        std::vector<int> v = {1, 2, 4, 8};
        tr2::optional<std::vector<int>> ov{tr2::in_place, {1, 2, 4, 8}};

        EXPECT_EQ(v, *ov);

        ov.emplace({1, 2, 4, 8});
        /*
              std::allocator<int> a;
              optional<std::vector<int>> ou { in_place, {1, 2, 4, 8}, a };

              EXPECT_EQ (ou, ov);
        */
    }

    /////////////////////////////////
    {
        typedef int T;
        tr2::optional<tr2::optional<T>> ot{tr2::in_place};
        tr2::optional<tr2::optional<T>> ou{tr2::in_place, tr2::nullopt};
        tr2::optional<tr2::optional<T>> ov{tr2::optional<T>{}};

        tr2::optional<int> oi;
        auto ooi = make_optional(oi);
        static_assert(std::is_same<tr2::optional<tr2::optional<int>>, decltype(ooi)>::value, "");

        unused(ot);
        unused(ou);
    }
};

bool fun(std::string, tr2::optional<int> oi = tr2::nullopt) {
    return bool(oi);
}

TEST(Optional, example_converting_ctor) {
    EXPECT_EQ(true, fun("dog", 2));
    EXPECT_EQ(false, fun("dog"));
    EXPECT_EQ(false, fun("dog", tr2::nullopt)); // just to be explicit
};

TEST(Optional, bad_comparison) {
    tr2::optional<int> oi, oj;
    int i;
    bool b = (oi == oj);
    b = (oi >= i);
    b = (oi == i);
    unused(b);
};

TEST(Optional, value_or) {
    tr2::optional<int> oi = 1;
    int i = oi.value_or(0);
    EXPECT_EQ(i, 1);

    oi = tr2::nullopt;
    EXPECT_EQ(oi.value_or(3), 3);

    tr2::optional<std::string> os{"AAA"};
    EXPECT_EQ(os.value_or("BBB"), "AAA");
    os = {};
    EXPECT_EQ(os.value_or("BBB"), "BBB");
};

TEST(Optional, reset) {
    tr2::optional<int> oi{1};
    oi.reset();
    EXPECT_FALSE(oi);

    int i = 1;
    tr2::optional<const int&> oir{i};
    oir.reset();
    EXPECT_FALSE(oir);
};

TEST(Optional, mixed_order) {
    tr2::optional<int> oN{tr2::nullopt};
    tr2::optional<int> o0{0};
    tr2::optional<int> o1{1};

    EXPECT_TRUE((oN < 0));
    EXPECT_TRUE((oN < 1));
    EXPECT_TRUE(!(o0 < 0));
    EXPECT_TRUE((o0 < 1));
    EXPECT_TRUE(!(o1 < 0));
    EXPECT_TRUE(!(o1 < 1));

    EXPECT_TRUE(!(oN >= 0));
    EXPECT_TRUE(!(oN >= 1));
    EXPECT_TRUE((o0 >= 0));
    EXPECT_TRUE(!(o0 >= 1));
    EXPECT_TRUE((o1 >= 0));
    EXPECT_TRUE((o1 >= 1));

    EXPECT_TRUE(!(oN > 0));
    EXPECT_TRUE(!(oN > 1));
    EXPECT_TRUE(!(o0 > 0));
    EXPECT_TRUE(!(o0 > 1));
    EXPECT_TRUE((o1 > 0));
    EXPECT_TRUE(!(o1 > 1));

    EXPECT_TRUE((oN <= 0));
    EXPECT_TRUE((oN <= 1));
    EXPECT_TRUE((o0 <= 0));
    EXPECT_TRUE((o0 <= 1));
    EXPECT_TRUE(!(o1 <= 0));
    EXPECT_TRUE((o1 <= 1));

    EXPECT_TRUE((0 > oN));
    EXPECT_TRUE((1 > oN));
    EXPECT_TRUE(!(0 > o0));
    EXPECT_TRUE((1 > o0));
    EXPECT_TRUE(!(0 > o1));
    EXPECT_TRUE(!(1 > o1));

    EXPECT_TRUE(!(0 <= oN));
    EXPECT_TRUE(!(1 <= oN));
    EXPECT_TRUE((0 <= o0));
    EXPECT_TRUE(!(1 <= o0));
    EXPECT_TRUE((0 <= o1));
    EXPECT_TRUE((1 <= o1));

    EXPECT_TRUE(!(0 < oN));
    EXPECT_TRUE(!(1 < oN));
    EXPECT_TRUE(!(0 < o0));
    EXPECT_TRUE(!(1 < o0));
    EXPECT_TRUE((0 < o1));
    EXPECT_TRUE(!(1 < o1));

    EXPECT_TRUE((0 >= oN));
    EXPECT_TRUE((1 >= oN));
    EXPECT_TRUE((0 >= o0));
    EXPECT_TRUE((1 >= o0));
    EXPECT_TRUE(!(0 >= o1));
    EXPECT_TRUE((1 >= o1));
};

struct BadRelops {
    int i;
};

constexpr bool operator<(BadRelops a, BadRelops b) {
    return a.i < b.i;
}
constexpr bool operator>(BadRelops a, BadRelops b) {
    return a.i < b.i;
} // intentional error!

TEST(Optional, bad_relops) {
    BadRelops a{1}, b{2};
    EXPECT_LT(a, b);
    EXPECT_GT(a, b);

    tr2::optional<BadRelops> oa = a, ob = b;
    EXPECT_LT(oa, ob);
    EXPECT_LE(oa, ob);

    EXPECT_LT(oa, b);
    EXPECT_GT(oa, b);

    tr2::optional<BadRelops&> ra = a, rb = b;
    EXPECT_LT(ra, rb);
    EXPECT_LE(ra, rb);

    EXPECT_LT(ra, b);
    EXPECT_GT(ra, b);
};

TEST(Optional, mixed_equality) {
    EXPECT_EQ(tr2::make_optional(0), 0);
    EXPECT_EQ(tr2::make_optional(1), 1);
    EXPECT_NE(tr2::make_optional(0), 1);
    EXPECT_NE(tr2::make_optional(1), 0);

    tr2::optional<int> oN{tr2::nullopt};
    tr2::optional<int> o0{0};
    tr2::optional<int> o1{1};

    EXPECT_EQ(o0, 0);
    EXPECT_EQ(0, o0);
    EXPECT_EQ(o1, 1);
    EXPECT_EQ(1, o1);

    EXPECT_NE(o1, 0);
    EXPECT_NE(0, o1);
    EXPECT_NE(o0, 1);
    EXPECT_NE(1, o0);

    EXPECT_NE(1, oN);
    EXPECT_NE(0, oN);
    EXPECT_NE(oN, 1);
    EXPECT_NE(oN, 0);

    EXPECT_FALSE((1 == oN));
    EXPECT_FALSE((0 == oN));
    EXPECT_FALSE((oN == 1));
    EXPECT_FALSE((oN == 0));

    std::string cat{"cat"}, dog{"dog"};
    tr2::optional<std::string> oNil{}, oDog{"dog"}, oCat{"cat"};

    EXPECT_EQ(oCat, cat);
    EXPECT_EQ(cat, oCat);
    EXPECT_EQ(oDog, dog);
    EXPECT_EQ(dog, oDog);

    EXPECT_NE(oDog, cat);
    EXPECT_NE(cat, oDog);
    EXPECT_NE(oCat, dog);
    EXPECT_NE(dog, oCat);

    EXPECT_NE(dog, oNil);
    EXPECT_NE(cat, oNil);
    EXPECT_NE(oNil, dog);
    EXPECT_NE(oNil, cat);

    EXPECT_FALSE((dog == oNil));
    EXPECT_FALSE((cat == oNil));
    EXPECT_FALSE((oNil == dog));
    EXPECT_FALSE((oNil == cat));
};

TEST(Optional, const_propagation) {
    tr2::optional<int> mmi{0};
    static_assert(std::is_same<decltype(*mmi), int&>::value, "error");

    const tr2::optional<int> cmi{0};
    static_assert(std::is_same<decltype(*cmi), const int&>::value, "error");

    tr2::optional<const int> mci{0};
    static_assert(std::is_same<decltype(*mci), const int&>::value, "error");

    tr2::optional<const int> cci{0};
    static_assert(std::is_same<decltype(*cci), const int&>::value, "error");
};

static_assert(std::is_base_of<std::logic_error, tr2::bad_optional_access>::value, "");

TEST(Optional, safe_value) {
    try {
        tr2::optional<int> ovN{}, ov1{1};

        int& r1 = ov1.value();
        EXPECT_EQ(r1, 1);

        EXPECT_THROW(ovN.value(), tr2::bad_optional_access);

        { // ref variant
            int i1 = 1;
            tr2::optional<int&> orN{}, or1{i1};

            int& r2 = or1.value();
            EXPECT_EQ(r2, 1);

            EXPECT_THROW(orN.value(), tr2::bad_optional_access);
        }
    } catch (...) {
        EXPECT_TRUE(false);
    }
};

TEST(Optional, optional_ref) {
    // FAILS: optional<int&&> orr;
    // FAILS: optional<nullopt_t&> on;
    int i = 8;
    tr2::optional<int&> ori;

    EXPECT_FALSE(ori);
    ori.emplace(i);

    EXPECT_TRUE(bool(ori));
    EXPECT_EQ(*ori, 8);
    EXPECT_EQ(&*ori, &i);

    *ori = 9;
    EXPECT_EQ(i, 9);

    // FAILS: int& ir = ori.value_or(i);
    int ii = ori.value_or(i);
    EXPECT_EQ(ii, 9);
    ii = 7;
    EXPECT_EQ(*ori, 9);

    int j = 22;
    auto&& oj = tr2::make_optional(std::ref(j));
    *oj = 23;

    EXPECT_EQ(&*oj, &j);
    EXPECT_EQ(j, 23);
};

TEST(Optional, optional_ref_const_propagation) {
    int i = 9;
    const tr2::optional<int&> mi = i;
    int& r = *mi;

    tr2::optional<const int&> ci = i;
    static_assert(std::is_same<decltype(*mi), int&>::value, "error");
    static_assert(std::is_same<decltype(*ci), const int&>::value, "error");

    unused(r);
};

TEST(Optional, optional_ref_assign) {
    int i = 9;
    tr2::optional<int&> ori = i;

    int j = 1;
    ori = tr2::optional<int&>{j};
    ori = {j};
    // FAILS: ori = j;

    tr2::optional<int&> orx = ori;
    ori = orx;

    tr2::optional<int&> orj = j;

    EXPECT_TRUE(ori);
    EXPECT_EQ(*ori, 1);
    EXPECT_EQ(ori, orj);
    EXPECT_EQ(i, 9);

    *ori = 2;
    EXPECT_EQ(*ori, 2);
    EXPECT_EQ(ori, 2);
    EXPECT_EQ(2, ori);
    EXPECT_NE(ori, 3);

    EXPECT_EQ(ori, orj);
    EXPECT_EQ(j, 2);
    EXPECT_EQ(i, 9);

    ori = {};
    EXPECT_FALSE(ori);
    EXPECT_NE(ori, orj);
    EXPECT_EQ(j, 2);
    EXPECT_EQ(i, 9);
};

TEST(Optional, optional_swap) {
    tr2::optional<int> oi{1}, oj{};
    swap(oi, oj);

    EXPECT_TRUE(oj);
    EXPECT_EQ(*oj, 1);
    EXPECT_FALSE(oi);
    static_assert(noexcept(swap(oi, oj)), "swap() is not noexcept");
};

TEST(Optional, optional_ref_swap) {
    int i = 0;
    int j = 1;
    tr2::optional<int&> oi = i;
    tr2::optional<int&> oj = j;

    EXPECT_EQ(&*oi, &i);
    EXPECT_EQ(&*oj, &j);

    swap(oi, oj);
    EXPECT_EQ(&*oi, &j);
    EXPECT_EQ(&*oj, &i);
};

TEST(Optional, optional_initialization) {
    std::string s = "STR";

    tr2::optional<std::string> os{s};
    tr2::optional<std::string> ot = s;
    tr2::optional<std::string> ou{"STR"};
    tr2::optional<std::string> ov = std::string{"STR"};

    EXPECT_EQ(*os, "STR");
    EXPECT_EQ(*ot, "STR");
    EXPECT_EQ(*ou, "STR");
    EXPECT_EQ(*ov, "STR");
};

#include <unordered_set>

TEST(Optional, optional_hashing) {
    std::hash<int> hi;
    std::hash<tr2::optional<int>> hoi;
    std::hash<std::string> hs;
    std::hash<tr2::optional<std::string>> hos;

    EXPECT_EQ(hi(0), hoi(tr2::optional<int>{0}));
    EXPECT_EQ(hi(1), hoi(tr2::optional<int>{1}));
    EXPECT_EQ(hi(3198), hoi(tr2::optional<int>{3198}));

    EXPECT_EQ(hs(""), hos(tr2::optional<std::string>{""}));
    EXPECT_EQ(hs("0"), hos(tr2::optional<std::string>{"0"}));
    EXPECT_EQ(hs("Qa1#"), hos(tr2::optional<std::string>{"Qa1#"}));

    std::unordered_set<tr2::optional<std::string>> set;
    EXPECT_EQ(set.find({"Qa1#"}), set.end());

    set.insert({"0"});
    EXPECT_EQ(set.find({"Qa1#"}), set.end());

    set.insert({"Qa1#"});
    EXPECT_NE(set.find({"Qa1#"}), set.end());
};

// optional_ref_emulation
template <class T>
struct generic {
    typedef T type;
};

template <class U>
struct generic<U&> {
    typedef std::reference_wrapper<U> type;
};

template <class T>
using Generic = typename generic<T>::type;

template <class X>
bool generic_fun() {
    tr2::optional<Generic<X>> op;
    return bool(op);
}

TEST(Optional, optional_ref_emulation) {
    tr2::optional<Generic<int>> oi = 1;
    EXPECT_EQ(*oi, 1);

    int i = 8;
    int j = 4;
    tr2::optional<Generic<int&>> ori{i};
    EXPECT_EQ(*ori, 8);
    EXPECT_NE(static_cast<void*>(&*ori), static_cast<void*>(&i)); // !DIFFERENT THAN optional<T&>

    *ori = j;
    EXPECT_EQ(*ori, 4);
};

#if OPTIONAL_HAS_THIS_RVALUE_REFS == 1
TEST(Optional, moved_on_value_or) {
    tr2::optional<Oracle> oo{tr2::in_place};

    EXPECT_TRUE(oo);
    EXPECT_EQ(oo->s, sDefaultConstructed);

    Oracle o = std::move(oo).value_or(Oracle{OracleVal{}});
    EXPECT_TRUE(oo);
    EXPECT_EQ(oo->s, sMovedFrom);
    EXPECT_EQ(o.s, sMoveConstructed);

    tr2::optional<MoveAware<int>> om{tr2::in_place, 1};
    EXPECT_TRUE(om);
    EXPECT_EQ(om->moved, false);

    /*MoveAware<int> m =*/std::move(om).value_or(MoveAware<int>{1});
    EXPECT_TRUE(om);
    EXPECT_EQ(om->moved, true);

#if OPTIONAL_HAS_MOVE_ACCESSORS == 1
    {
        Date d = tr2::optional<Date>{tr2::in_place, 1}.value();
        EXPECT_TRUE(d.i); // to silence compiler warning

        Date d2 = *tr2::optional<Date>{tr2::in_place, 1};
        EXPECT_TRUE(d2.i); // to silence compiler warning
    }
#endif
};
#endif

TEST(Optional, optional_ref_hashing) {
    std::hash<int> hi;
    std::hash<tr2::optional<int&>> hoi;
    std::hash<std::string> hs;
    std::hash<tr2::optional<std::string&>> hos;

    int i0 = 0;
    int i1 = 1;
    EXPECT_EQ(hi(0), hoi(tr2::optional<int&>{i0}));
    EXPECT_EQ(hi(1), hoi(tr2::optional<int&>{i1}));

    std::string s{""};
    std::string s0{"0"};
    std::string sCAT{"CAT"};

    EXPECT_EQ(hs(""), hos(tr2::optional<std::string&>{s}));
    EXPECT_EQ(hs("0"), hos(tr2::optional<std::string&>{s0}));
    EXPECT_EQ(hs("CAT"), hos(tr2::optional<std::string&>{sCAT}));

    std::unordered_set<tr2::optional<std::string&>> set;
    EXPECT_EQ(set.find({sCAT}), set.end());

    set.insert({s0});
    EXPECT_EQ(set.find({sCAT}), set.end());

    set.insert({sCAT});
    EXPECT_NE(set.find({sCAT}), set.end());
};

struct Combined {
    int m = 0;
    int n = 1;

    constexpr Combined() : m{5}, n{6} {}
    constexpr Combined(int m, int n) : m{m}, n{n} {}
};

struct Nasty {
    int m = 0;
    int n = 1;

    constexpr Nasty() : m{5}, n{6} {}
    constexpr Nasty(int m, int n) : m{m}, n{n} {}

    int operator&() { return n; }
    int operator&() const { return n; }
};

TEST(Optional, arrow_operator) {
    tr2::optional<Combined> oc1{tr2::in_place, 1, 2};
    EXPECT_TRUE(oc1);
    EXPECT_EQ(oc1->m, 1);
    EXPECT_EQ(oc1->n, 2);

    tr2::optional<Nasty> on{tr2::in_place, 1, 2};
    EXPECT_TRUE(on);
    EXPECT_EQ(on->m, 1);
    EXPECT_EQ(on->n, 2);
};

TEST(Optional, arrow_wit_optional_ref) {
    Combined c{1, 2};

    tr2::optional<Combined&> oc = c;
    EXPECT_TRUE(oc);
    EXPECT_EQ(oc->m, 1);
    EXPECT_EQ(oc->n, 2);

    Nasty n{1, 2};
    Nasty m{3, 4};
    Nasty p{5, 6};

    tr2::optional<Nasty&> on{n};
    EXPECT_TRUE(on);
    EXPECT_EQ(on->m, 1);
    EXPECT_EQ(on->n, 2);

    on = {m};
    EXPECT_TRUE(on);
    EXPECT_EQ(on->m, 3);
    EXPECT_EQ(on->n, 4);

    on.emplace(p);
    EXPECT_TRUE(on);
    EXPECT_EQ(on->m, 5);
    EXPECT_EQ(on->n, 6);

    tr2::optional<Nasty&> om{tr2::in_place, n};
    EXPECT_TRUE(om);
    EXPECT_EQ(om->m, 1);
    EXPECT_EQ(om->n, 2);
};

TEST(Optional, no_dangling_reference_in_value) {
    // this mostly tests compiler warnings
    tr2::optional<int> oi{2};
    unused(oi.value());
    const tr2::optional<int> coi{3};
    unused(coi.value());
};

struct CountedObject {
    static int _counter;
    bool _throw;
    CountedObject(bool b) : _throw(b) { ++_counter; }
    CountedObject(CountedObject const& rhs) : _throw(rhs._throw) {
        if (_throw) throw int();
    }
    ~CountedObject() { --_counter; }
};

int CountedObject::_counter = 0;

TEST(Optional, exception_safety) {
    using namespace std::experimental;
    try {
        optional<CountedObject> oo(in_place, true); // throw
        optional<CountedObject> o1(oo);
    } catch (...) {
        //
    }
    EXPECT_EQ(CountedObject::_counter, 0);

    try {
        optional<CountedObject> oo(in_place, true); // throw
        optional<CountedObject> o1(std::move(oo));  // now move
    } catch (...) {
        //
    }
    EXPECT_EQ(CountedObject::_counter, 0);
};

TEST(Optional, nested_optional) {
    tr2::optional<tr2::optional<tr2::optional<int>>> o1{tr2::nullopt};
    EXPECT_FALSE(o1);

    tr2::optional<tr2::optional<tr2::optional<int>>> o2{tr2::in_place, tr2::nullopt};
    EXPECT_TRUE(o2);
    EXPECT_FALSE(*o2);

    tr2::optional<tr2::optional<tr2::optional<int>>> o3(tr2::in_place, tr2::in_place, tr2::nullopt);
    EXPECT_TRUE(o3);
    EXPECT_TRUE(*o3);
    EXPECT_FALSE(**o3);
};

TEST(Optional, three_ways_of_having_value) {
    tr2::optional<int> oN, o1(1);

    EXPECT_FALSE(oN);
    EXPECT_FALSE(oN.has_value());
    EXPECT_EQ(oN, tr2::nullopt);

    EXPECT_TRUE(o1);
    EXPECT_TRUE(o1.has_value());
    EXPECT_NE(o1, tr2::nullopt);

    EXPECT_EQ(bool(oN), oN.has_value());
    EXPECT_EQ(bool(o1), o1.has_value());

    int i = 1;
    tr2::optional<int&> rN, r1(i);

    EXPECT_FALSE(rN);
    EXPECT_FALSE(rN.has_value());
    EXPECT_EQ(rN, tr2::nullopt);

    EXPECT_TRUE(r1);
    EXPECT_TRUE(r1.has_value());
    EXPECT_NE(r1, tr2::nullopt);

    EXPECT_EQ(bool(rN), rN.has_value());
    EXPECT_EQ(bool(r1), r1.has_value());
};

//// constexpr tests

// these 4 classes have different noexcept signatures in move operations
struct NothrowBoth {
    NothrowBoth(NothrowBoth&&) noexcept(true) {}
    void operator=(NothrowBoth&&) noexcept(true) {}
};
struct NothrowCtor {
    NothrowCtor(NothrowCtor&&) noexcept(true) {}
    void operator=(NothrowCtor&&) noexcept(false) {}
};
struct NothrowAssign {
    NothrowAssign(NothrowAssign&&) noexcept(false) {}
    void operator=(NothrowAssign&&) noexcept(true) {}
};
struct NothrowNone {
    NothrowNone(NothrowNone&&) noexcept(false) {}
    void operator=(NothrowNone&&) noexcept(false) {}
};

void test_noexcept() {
    {
        tr2::optional<NothrowBoth> b1, b2;
        static_assert(noexcept(tr2::optional<NothrowBoth>{tr2::constexpr_move(b1)}), "bad noexcept!");
        static_assert(noexcept(b1 = tr2::constexpr_move(b2)), "bad noexcept!");
    }
    {
        tr2::optional<NothrowCtor> c1, c2;
        static_assert(noexcept(tr2::optional<NothrowCtor>{tr2::constexpr_move(c1)}), "bad noexcept!");
        static_assert(!noexcept(c1 = tr2::constexpr_move(c2)), "bad noexcept!");
    }
    {
        tr2::optional<NothrowAssign> a1, a2;
        static_assert(!noexcept(tr2::optional<NothrowAssign>{tr2::constexpr_move(a1)}), "bad noexcept!");
        static_assert(!noexcept(a1 = tr2::constexpr_move(a2)), "bad noexcept!");
    }
    {
        tr2::optional<NothrowNone> n1, n2;
        static_assert(!noexcept(tr2::optional<NothrowNone>{tr2::constexpr_move(n1)}), "bad noexcept!");
        static_assert(!noexcept(n1 = tr2::constexpr_move(n2)), "bad noexcept!");
    }
}

void constexpr_test_disengaged() {
    constexpr tr2::optional<int> g0{};
    constexpr tr2::optional<int> g1{tr2::nullopt};
    static_assert(!g0, "initialized!");
    static_assert(!g1, "initialized!");

    static_assert(bool(g1) == bool(g0), "ne!");

    static_assert(g1 == g0, "ne!");
    static_assert(!(g1 != g0), "ne!");
    static_assert(g1 >= g0, "ne!");
    static_assert(!(g1 > g0), "ne!");
    static_assert(g1 <= g0, "ne!");
    static_assert(!(g1 < g0), "ne!");

    static_assert(g1 == tr2::nullopt, "!");
    static_assert(!(g1 != tr2::nullopt), "!");
    static_assert(g1 <= tr2::nullopt, "!");
    static_assert(!(g1 < tr2::nullopt), "!");
    static_assert(g1 >= tr2::nullopt, "!");
    static_assert(!(g1 > tr2::nullopt), "!");

    static_assert((tr2::nullopt == g0), "!");
    static_assert(!(tr2::nullopt != g0), "!");
    static_assert((tr2::nullopt >= g0), "!");
    static_assert(!(tr2::nullopt > g0), "!");
    static_assert((tr2::nullopt <= g0), "!");
    static_assert(!(tr2::nullopt < g0), "!");

    static_assert((g1 != tr2::optional<int>(1)), "!");
    static_assert(!(g1 == tr2::optional<int>(1)), "!");
    static_assert((g1 < tr2::optional<int>(1)), "!");
    static_assert((g1 <= tr2::optional<int>(1)), "!");
    static_assert(!(g1 > tr2::optional<int>(1)), "!");
    static_assert(!(g1 > tr2::optional<int>(1)), "!");
}

constexpr tr2::optional<int> g0{};
constexpr tr2::optional<int> g2{2};
static_assert(g2, "not initialized!");
static_assert(*g2 == 2, "not 2!");
static_assert(g2 == tr2::optional<int>(2), "not 2!");
static_assert(g2 != g0, "eq!");

#if OPTIONAL_HAS_MOVE_ACCESSORS == 1
static_assert(*tr2::optional<int>{3} == 3, "error");
static_assert(tr2::optional<int>{3}.value() == 3, "error");
static_assert(tr2::optional<int>{3}.value_or(1) == 3, "error");
static_assert(tr2::optional<int>{}.value_or(4) == 4, "error");
#endif

constexpr tr2::optional<Combined> gc0{tr2::in_place};
static_assert(gc0->n == 6, "error");

// optional refs
int gi = 0;
constexpr tr2::optional<int&> gori = gi;
constexpr tr2::optional<int&> gorn{};
constexpr int& gri = *gori;
static_assert(gori, "error");
static_assert(!gorn, "error");
static_assert(gori != tr2::nullopt, "error");
static_assert(gorn == tr2::nullopt, "error");
static_assert(&gri == &*gori, "error");

constexpr int gci = 1;
constexpr tr2::optional<int const&> gorci = gci;
constexpr tr2::optional<int const&> gorcn{};

static_assert(gorcn < gorci, "error");
static_assert(gorcn <= gorci, "error");
static_assert(gorci == gorci, "error");
static_assert(*gorci == 1, "error");
static_assert(gorci == gci, "error");

namespace constexpr_optional_ref_and_arrow {
using namespace std::experimental;
constexpr Combined c{1, 2};
constexpr optional<Combined const&> oc = c;
static_assert(oc, "error");
static_assert(oc->m == 1, "error");
static_assert(oc->n == 2, "error");
} // namespace constexpr_optional_ref_and_arrow

#if OPTIONAL_HAS_CONSTEXPR_INIT_LIST

namespace InitList {

using namespace tr2;

struct ConstInitLister {
    template <typename T>
    constexpr ConstInitLister(std::initializer_list<T> il) : len(il.size()) {}
    size_t len;
};

constexpr ConstInitLister CIL{2, 3, 4};
static_assert(CIL.len == 3, "error");

constexpr optional<ConstInitLister> oil{in_place, {4, 5, 6, 7}};
static_assert(oil, "error");
static_assert(oil->len == 4, "error");
} // namespace InitList

#endif // OPTIONAL_HAS_CONSTEXPR_INIT_LIST

// end constexpr tests
