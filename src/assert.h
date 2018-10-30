
#ifndef GAMEENGINE301CR_ASSET_H
#define GAMEENGINE301CR_ASSET_H

#if ASSERTIONS_ENABLED

#include <cassert>

#ifdef __APPLE__
#define ASSERT(expr) assert(expr)
#else
#define ASSERT(expr) // noop
#endif

#else

#define ASSERT(expr) // noop

#endif

#endif //GAMEENGINE301CR_ASSET_H
