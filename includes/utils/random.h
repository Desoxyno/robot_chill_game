#pragma once

#include <cstdint>
#include <random>
#include <hash_fun.h>
#include <raymath.h>
#include <sys/types.h>

inline std::random_device rd;  
inline std::mt19937 gene(rd());

inline const int generate_n(const int &min, const int &max) {
        std::uniform_int_distribution<int> dist(min, max);   

        return dist(gene); 
}  
        
inline const float generate_n(const float &min, const float &max) {       
    std::uniform_real_distribution<float> dist(min, max);      

    return dist(gene); }

inline const uint32_t hash32(const uint32_t value) {
        uint32_t x = value;
        x = ((x >> 16) ^ x) * 0x45d9f3bu;
        x = ((x >> 16) ^ x) * 0x45d9f3bu;
        x = (x >> 16) ^ x;

        return x;
}

inline const float generate_dn(const uint &seed, const uint &index, const int &channel, const float &min, const float &max) {      
        uint32_t x = hash32(seed);
        x ^= index;
        x = hash32(x);
        x ^= channel;
        x = hash32(x);
        
        float normalized = float(x) / float(4294967295);
        float result = min + normalized * (max - min);
        
        return result;
}