
#ifndef FOXERENGINE_SIDPREPROCESSOR_H
#define FOXERENGINE_SIDPREPROCESSOR_H

#define SID(str) processString(str)

typedef unsigned int StringId;

constexpr unsigned int processString(const char *str) {
    unsigned int hash = 5381;
    int c = 0;

    while ((c = *str++)) {
        hash = ((hash << 5) + hash) ^ c;
    }

    return hash;
}

#endif //FOXERENGINE_SIDPREPROCESSOR_H
