#include "../mapbox/weak/include/mapbox/weak.hpp"

#include <atomic>
#include <chrono>
#include <ctime>
#include <memory>
#include <thread>
#include <vector>

namespace {

void testLock() {
    using namespace std::chrono_literals;
    static std::atomic_int g_i;
    struct TestLock {
        void inc() { ++g_i; }
        mapbox::base::WeakPtrFactory<TestLock> factory_{this};
    };

    auto t = std::make_unique<TestLock>();
    auto weak1 = t->factory_.makeWeakPtr();
    auto weak2 = t->factory_.makeWeakPtr();
    auto weak3 = weak2;

    std::thread thread1([&] {
        auto guard = weak1.lock();
        std::this_thread::sleep_for(150ms);
        weak1->inc();
    });
    std::thread thread2([&] {
        auto guard = weak2.lock();
        std::this_thread::sleep_for(200ms);
        weak2->inc();
    });
    {
        auto guard = weak3.lock();
        std::this_thread::sleep_for(50ms);
        weak3->inc();
    }

    assert(!weak1.expired());
    assert(!weak2.expired());
    assert(weak1);
    assert(weak2);
    t.reset(); // Should not crash.
    thread1.join();
    thread2.join();

    assert(weak1.expired());
    assert(weak2.expired());
    assert(!weak1);
    assert(!weak2);
    assert(g_i == 3);
}

void testMultipleLock() {
    using namespace std::chrono_literals;
    using std::chrono::system_clock;

    static std::atomic_int g_i;
    struct TestLock {
        void inc() { ++g_i; }
        mapbox::base::WeakPtrFactory<TestLock> factory_{this};
    };

    std::time_t now = system_clock::to_time_t(system_clock::now());

    struct std::tm* tm = std::localtime(&now);
    ++tm->tm_sec; // Wait for the next second.
    auto nextSecond = system_clock::from_time_t(mktime(tm));

    auto t = std::make_unique<TestLock>();

    const size_t threadsCount = 50u;

    std::vector<std::thread> threads;
    threads.reserve(threadsCount);

    for (size_t i = 0; i < threadsCount; ++i) {
        std::thread thread([nextSecond, weak = t->factory_.makeWeakPtr()] {
            auto guard = weak.lock();
            std::this_thread::sleep_until(nextSecond);
            weak->inc();
        });
        threads.emplace_back(std::move(thread));
    }

    t.reset();
    for (auto& thread : threads) {
        thread.join();
    }
    assert(g_i == threadsCount);
}

void testWeakMethod() {
    using namespace std::chrono_literals;
    static std::atomic_int g_i;
    class Test {
    public:
        void increaseGlobal(int delta) { g_i += delta; }
        std::function<void(int)> makeWeakIncreaseGlobal() { return factory_.makeWeakMethod(&Test::increaseGlobal); }

    private:
        mapbox::base::WeakPtrFactory<Test> factory_{this};
    };

    auto t = std::make_unique<Test>();
    std::function<void(int)> weak1 = t->makeWeakIncreaseGlobal();
    std::function<void(int)> weak2 = t->makeWeakIncreaseGlobal();
    std::function<void(int)> weak3 = weak2;

    std::thread thread1([&] { weak1(1); });
    std::thread thread2([&] { weak2(10); });
    std::this_thread::sleep_for(50ms);
    weak3(100);

    t.reset(); // Should not crash.
    // The following calls are ignored.
    weak1(1);
    weak2(2);
    weak3(3);
    thread1.join();
    thread2.join();

    assert(g_i == 111);
}

void testWeakMethodBlock() {
    // NOLINTNEXTLINE(google-build-using-namespace)
    using namespace std::chrono;
    using namespace std::chrono_literals;
    static std::atomic_bool g_call_finished{false};
    struct Test {
        void block(decltype(1ms) duration) {
            std::this_thread::sleep_for(duration);
            g_call_finished = true;
        }
        mapbox::base::WeakPtrFactory<Test> factory_{this};
    };

    auto t = std::make_unique<Test>();
    auto weak = t->factory_.makeWeakMethod(&Test::block);
    auto first = high_resolution_clock::now();

    std::thread thread([&] { weak(100ms); });
    std::this_thread::sleep_for(10ms);
    t.reset(); // Deletion is blocked until weak(100ms) call returns.
    thread.join();
    auto totalTime = duration_cast<milliseconds>(high_resolution_clock::now() - first);

    assert(g_call_finished);
    assert(totalTime >= 100ms);
}

} // namespace

int main() {
    testLock();
    testMultipleLock();
    testWeakMethod();
    testWeakMethodBlock();
    return 0;
}
