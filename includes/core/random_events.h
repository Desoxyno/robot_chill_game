#pragma once

#include <raylib.h>
#include "utils/random.h"

enum RandomEvent {
    MAN_TREE,
};

inline int randomEvent() {
    static double lastEventCheck = 0.0;

    const double currentTime = GetTime();

    if (currentTime - lastEventCheck >= 1.0) {
        lastEventCheck = currentTime;

        float random = generate_nf(0.0f, 1.0f);

        if (random > 0.5f) [[unlikely]] {
            TraceLog(LOG_WARNING, "Event Active : %d", MAN_TREE);
            return MAN_TREE;
        }
        return -1;
    }
    return -1;
}