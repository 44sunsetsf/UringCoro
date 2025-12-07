#pragma once

#include <atomic>
#include <thread>

#if defined(_MSC_VER) && (defined(_M_IX86) || defined(_M_X64))
#include <immintrin.h>
#endif

namespace coro::detail {
using std::atomic;
using std::memory_order_acquire;
using std::memory_order_relaxed;
using std::memory_order_release;

struct spinlock {
  atomic<bool> lock_ = {0};

  void lock() noexcept {
    for (;;) {
      // Optimistically assume the lock is free on the first try
      if (!lock_.exchange(true, memory_order_acquire)) {
        return;
      }

      // Wait for lock to be released without generating cache misses
      while (lock_.load(memory_order_relaxed)) {
        // 在 x86/x64 上用 pause，其他架构就用 std::this_thread::yield()

#if (defined(__GNUC__) || defined(__clang__)) &&                               \
    (defined(__i386__) || defined(__x86_64__))
        __builtin_ia32_pause();
#elif defined(_MSC_VER) && (defined(_M_IX86) || defined(_M_X64))
        _mm_pause();
#else
        std::this_thread::yield();
#endif
      }
    }
  }

  bool try_lock() noexcept {
    // First do a relaxed load to check if lock is free in order to prevent
    // unnecessary cache misses if someone does while(!try_lock())
    return !lock_.load(memory_order_relaxed) &&
           !lock_.exchange(true, memory_order_acquire);
  }

  void unlock() noexcept { lock_.store(false, memory_order_release); }
};

}; // namespace coro::detail
