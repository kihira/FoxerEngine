
#ifndef FOXERENGINE_ASSET_H
#define FOXERENGINE_ASSET_H

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

#endif //FOXERENGINE_ASSET_H
