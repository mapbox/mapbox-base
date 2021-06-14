#pragma once

#include <memory>
#include <mutex>
#include <type_traits>
#include <utility>

namespace mapbox {
namespace base {

class TypeWrapper {
public:
    using Guard = std::unique_lock<std::mutex>;

    TypeWrapper() noexcept : storage_(nullptr, noop_deleter) {}

    TypeWrapper(TypeWrapper&& other) : storage_(nullptr, noop_deleter) {
        std::lock_guard<std::mutex> lock(other.mutex_);
        storage_ = std::move(other.storage_);
    }

    TypeWrapper& operator=(TypeWrapper&& other) {
        if (this != &other) {
            std::lock(mutex_, other.mutex_);
            std::lock_guard<std::mutex> lock1(mutex_, std::adopt_lock);
            std::lock_guard<std::mutex> lock2(other.mutex_, std::adopt_lock);

            storage_ = std::move(other.storage_);
        }
        return *this;
    }

    template <typename T> // NOLINTNEXTLINE misc-forwarding-reference-overload
    TypeWrapper(T&& value) noexcept
        : storage_(new std::decay_t<T>(std::forward<T>(value)), cast_deleter<std::decay_t<T>>) {
        static_assert(!std::is_same<TypeWrapper, std::decay_t<T>>::value, "TypeWrapper must not wrap itself.");
    }

    bool has_value() const noexcept {
        return static_cast<bool>(storage_);
    }

    template <typename T>
    void set(T&& value) {
        static_assert(!std::is_same<TypeWrapper, std::decay_t<T>>::value, "TypeWrapper must not wrap itself.");
        storage_ = std::unique_ptr<void, void (*)(void*)>(new std::decay_t<T>(std::forward<T>(value)), cast_deleter<std::decay_t<T>>);
    }
    template <typename T>
    T& get() noexcept { // NOLINTNEXTLINE cppcoreguidelines-pro-type-reinterpret-cast
        return *reinterpret_cast<T*>(storage_.get());
    }

    Guard lock() {
        return Guard(mutex_);
    }

private:
    template <typename T>
    static void cast_deleter(void* ptr) noexcept {
        delete reinterpret_cast<T*>(ptr); // NOLINT cppcoreguidelines-pro-type-reinterpret-cast
    }
    static void noop_deleter(void*) noexcept {}

    using storage_t = std::unique_ptr<void, void (*)(void*)>;
    storage_t storage_;
    mutable std::mutex mutex_;
};

} // namespace base
} // namespace mapbox
