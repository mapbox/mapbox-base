#include "../mapbox/typewrapper/include/mapbox/type_wrapper.hpp"

#include <cassert>

using mapbox::base::TypeWrapper;

namespace {

class TestType {
public:
    TestType() { str[0] = 'a'; }

    // Detect moves
    TestType(TestType&& t) noexcept : i1(t.i1 + 1), i2(t.i2 + 2) { str[0] = t.str[0] + 1; }

    TestType(const TestType&) = delete;
    TestType& operator=(const TestType&) = delete;

    int i1 = 0;
    int i2 = 1;
    char str[256] = {};
};

void testEmpty() {
    assert(!TypeWrapper().has_value());
}

void testBasicTypes() {
    TypeWrapper i = 3;
    assert(i.has_value());
    assert(i.get<decltype(3)>() == 3);

    auto iValue = i.get<decltype(3)>();
    assert(iValue == 3);

    assert(TypeWrapper(4).has_value());
    assert(TypeWrapper(4).get<decltype(4)>() == 4);

    TypeWrapper f = 6.2f;
    assert(f.has_value());
    assert(f.get<decltype(6.2f)>() == 6.2f);

    const float fValue = f.get<decltype(6.2f)>();
    assert(fValue == 6.2f);

    assert(TypeWrapper(1.0f).has_value());
    assert(TypeWrapper(1.0f).get<decltype(1.0f)>() == 1.0f);

    TypeWrapper c = 'z';
    assert(c.has_value());
    assert(c.get<decltype('z')>() == 'z');

    assert(TypeWrapper('z').has_value());
    assert(TypeWrapper('z').get<decltype('z')>() == 'z');
}

void testBasicTypesMove() {
    TypeWrapper i = 3;
    assert(i.has_value());

    TypeWrapper f = 6.2f;
    assert(f.has_value());

    f = std::move(i);
    assert(!i.has_value()); // NOLINT(bugprone-use-after-move)

    assert(f.has_value());
    assert(f.get<decltype(3)>() == 3);
}

void testSmallType() {
    struct T {
        explicit T(int32_t* p_) : p(p_) { (*p)++; }

        T(T&& t) noexcept : p(t.p) { (*p)++; }

        ~T() { (*p)--; }

        T(const T&) = delete;
        T& operator=(const T&) = delete;

        int32_t* p;
    };

    int32_t p = 0;

    {
        TypeWrapper u1 = TypeWrapper(T(&p));
        assert(p == 1);

        auto u2(std::move(u1));
        assert(p == 1);
    }

    assert(p == 0);
}

void testLargeType() {
    TestType t1;
    TypeWrapper u1 = TypeWrapper(std::move(t1));
    assert(u1.has_value());

    // TestType should be moved into owning TypeWrapper
    assert(u1.get<TestType>().i1 == 1);

    auto u2(std::move(u1));
    assert(!u1.has_value()); // NOLINT(bugprone-use-after-move)

    // TestType should not be moved when owning TypeWrapper is moved;
    assert(u2.get<TestType>().i1 == 1);

    // TestType should not be moved out of owning TypeWrapper
    auto& t2 = u2.get<TestType>();
    assert(u2.has_value());
    assert(t2.i1 == 1);
}

void testPointer() {
    auto* t1 = new TestType(); // NOLINT cppcoreguidelines-owning-memory

    auto u1 = TypeWrapper(t1);
    assert(u1.has_value());

    // Only the pointer should be moved
    TestType* t2 = u1.get<TestType*>(); // NOLINT cppcoreguidelines-owning-memory
    assert(t2->i1 == 0);

    TypeWrapper u2(4);
    std::swap(u2, u1);

    assert(u1.has_value());

    assert(u2.has_value());

    t2 = u2.get<TestType*>();
    assert(t2->i1 == 0);
    delete t2; // NOLINT cppcoreguidelines-owning-memory
}

void testUniquePtr() {
    auto t1 = std::make_unique<TestType>();
    auto u1 = TypeWrapper(std::move(t1));

    assert(t1 == nullptr);
    assert(u1.has_value());

    u1 = TypeWrapper();
    assert(!u1.has_value());

    TypeWrapper u2;
    auto* t3 = new TestType(); // NOLINT cppcoreguidelines-owning-memory
    u2 = std::unique_ptr<TestType>(t3);
    assert(u2.has_value());
}

void testSharedPtr() {
    std::shared_ptr<int> shared(new int(3));
    std::weak_ptr<int> weak = shared;
    TypeWrapper u1 = 0;

    assert(weak.use_count() == 1);
    TypeWrapper u2 = shared;
    assert(weak.use_count() == 2);

    u1 = std::move(u2);
    assert(weak.use_count() == 2);
    std::swap(u2, u1);
    assert(weak.use_count() == 2);
    u2 = 0;
    assert(weak.use_count() == 1);
    shared = nullptr;
    assert(weak.use_count() == 0);
}

} // namespace

int main() {
    testEmpty();
    testBasicTypes();
    testBasicTypesMove();
    testSmallType();
    testLargeType();
    testPointer();
    testUniquePtr();
    testSharedPtr();

    return 0;
}