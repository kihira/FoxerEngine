
#ifndef GAMEENGINE301CR_SIDPREPROCESSOR_H
#define GAMEENGINE301CR_SIDPREPROCESSOR_H

#define SID(str) processString(str)

typedef unsigned int StringId;

constexpr unsigned int processString(const char *str) {
    unsigned int hash = 5381;
    int c = 0;

    while ((c = *str++)) {
        hash = ((hash << 5) + hash) + c;
    }

    return hash;
}

#endif //GAMEENGINE301CR_SIDPREPROCESSOR_H
