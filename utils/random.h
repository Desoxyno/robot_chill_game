#pragma once

#include <random>

inline int generate_rd_n(int min, int max) {
        std::random_device rd;  
        std::mt19937 gen(rd());
         
        std::uniform_int_distribution<int> dist(min, max);   

        return dist(gen); 
}  
        
inline float generate_rd_n(float min, float max) {     
    std::random_device rd;     
    std::mt19937 gen(rd());

    std::uniform_real_distribution<float> dist(min, max);      

    return dist(gen); }