#pragma once
#include <iostream>
#include <vector>

inline std::vector<double> frame_ms_measures{};


inline constinit double highest = 0.0;
inline constinit double lowest = 0.0;
inline constinit double mean = 0.0;

inline void CalculateData() {
    double sum = 0.0;
    for (auto value : frame_ms_measures) {
        sum += value;
        if (value > highest) {
            highest = value;
        }
        if (value < lowest) {
            lowest = value;
        }
    }

    mean = sum / frame_ms_measures.size();
}

inline void ShowData() {
    CalculateData();
    std::cout << "Average Frame Time : " << mean * 1000 << "ms" << std::endl;
    std::cout << "Highest Frame Time : " << highest * 1000 << "ms" << std::endl;
    std::cout << "Lowest Frame Time : " << lowest * 1000 << "ms" << std::endl;
}