#pragma once

#include <chrono>
#include <raylib.h>
#include <string_view>

class Timer {
    std::chrono::time_point<std::chrono::system_clock, std::chrono::duration<long, std::ratio<1, 1000000000>>> start_time;
    std::chrono::time_point<std::chrono::system_clock, std::chrono::duration<long, std::ratio<1, 1000000000>>> end_timer;

    public:
        const void startTimer() {
            start_time = std::chrono::system_clock::now();
        }

        const void endTimer() {
            end_timer = std::chrono::system_clock::now();
        }

        const std::chrono::duration<double> getElapsed() {
            return end_timer - start_time;
        }
};

template <typename F, typename T>
inline void measure(F operation, T& value, std::string_view function_name) {
    Timer timer;
    timer.startTimer();

    operation(value);

    timer.endTimer();
    auto elapsed_time = timer.getElapsed().count();

    TraceLog(LOG_WARNING, "Function %s took %f seconds", function_name.data(), elapsed_time);

}

