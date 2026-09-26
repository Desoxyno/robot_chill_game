#pragma once

#include <random>

inline std::random_device rd;  
inline std::mt19937 gen(rd());

inline int generate_rd_n(int min, int max) {
        std::uniform_int_distribution<int> dist(min, max);   

        return dist(gen); 
}  
        
inline float generate_rd_n(float min, float max) {       
    std::uniform_real_distribution<float> dist(min, max);      

    return dist(gen); }