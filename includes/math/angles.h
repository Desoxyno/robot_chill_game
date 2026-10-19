#include <cmath>

inline float fastSin(float x){
    x = fmod(x + M_PI, M_PI * 2) - M_PI; // restrict x so that -M_PI < x < M_PI
    const float B = 4.0f/M_PI;
    const float C = -4.0f/(M_PI*M_PI);

    float y = B * x + C * x * std::abs(x);

    const float P = 0.225f;

    return P * (y * std::abs(y) - y) + y; 
}