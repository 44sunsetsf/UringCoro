# UringCoro — C++20 Coroutine Runtime with io_uring

UringCoro is a lightweight high-performance coroutine runtime built with **C++20 stackless coroutines** and **Linux io_uring**. It provides structured concurrency, coroutine-safe synchronization primitives, and a simple multi-engine scheduler for scalable asynchronous workloads.

## Features

- C++20 coroutine-based async runtime
- io_uring-backed TCP read/write operations
- Multi-engine scheduler for parallel I/O
- Coroutine-friendly primitives: `timer`, `event`, `latch`, `wait_group`, `mutex`, `cond_var`, `channel`
- Write async logic in a synchronous style

## Example

```cpp
task<> run() {
    co_await timer{}.add_seconds(1);
}

int main() {
    scheduler::init();
    submit_to_scheduler(run());
    scheduler::loop();
}
```

## Build

Requirements: Linux, GCC (C++20), CMake ≥ 3.15, liburing

```bash
git clone https://github.com/44sunsetsf/UringCoro
cd UringCoro
git submodule update --init --recursive

mkdir build && cd build
cmake ..
make -j$(nproc)
```

