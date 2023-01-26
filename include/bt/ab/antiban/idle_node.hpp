#pragma once

#include <Game/Tools/Antiban.hpp>
#include <bt/status.hpp>

#include <chrono>
#include <random>

namespace bt::ab::antiban {
template <typename Context> class idle_node {
public:
  std::normal_distribution<float> interval_distribution;
  std::normal_distribution<float> duration_distribution;

  template <typename Rep, typename Period>
  idle_node(std::chrono::duration<Rep, Period> interval_mean,
            std::chrono::duration<Rep, Period> interval_stddev,
            std::chrono::duration<Rep, Period> duration_mean,
            std::chrono::duration<Rep, Period> duration_stddev)
      : _random_engine(std::random_device()()) {
    interval_distribution = std::normal_distribution<float>(
        std::chrono::duration_cast<std::chrono::milliseconds>(interval_mean)
            .count(),
        std::chrono::duration_cast<std::chrono::milliseconds>(interval_stddev)
            .count());

    duration_distribution = std::normal_distribution<float>(
        std::chrono::duration_cast<std::chrono::milliseconds>(duration_mean)
            .count(),
        std::chrono::duration_cast<std::chrono::milliseconds>(duration_stddev)
            .count());

    _next_idle = std::chrono::steady_clock::now() +
                 std::chrono::milliseconds(
                     static_cast<int>(interval_distribution(_random_engine)));

    _idle_end = std::chrono::steady_clock::now() +
                std::chrono::milliseconds(
                    static_cast<int>(duration_distribution(_random_engine)));
  }

  status tick(Context &ctx) {
    const auto now = std::chrono::steady_clock::now();
    if (now > _next_idle && now < _idle_end) {
      if (!Internal::GetFocused())
        return status::running;
      
      std::cout << "Idling...\n";
      Antiban::MouseOffClient(true);
      return status::running;
    }

    if (now > _idle_end) {
      const auto interval = interval_distribution(_random_engine);
      std::cout << "Next idle in: " << interval << "ms, ";
      _next_idle = now + std::chrono::milliseconds(static_cast<int>(interval));

      const auto duration = duration_distribution(_random_engine);
      std::cout << "Duration: " << duration << "ms\n";
      _idle_end =
          _next_idle + std::chrono::milliseconds(static_cast<int>(duration));
    }

    return status::success;
  }

private:
  std::default_random_engine _random_engine;
  std::chrono::steady_clock::time_point _next_idle;
  std::chrono::steady_clock::time_point _idle_end;
};
} // namespace bt::ab::antiban