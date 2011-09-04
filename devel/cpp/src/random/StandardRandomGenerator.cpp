#include <cstdlib>

#include "StandardRandomGenerator.hpp"

promote::rand_t
promote::StandardRandomGenerator::generate() const
{
#ifdef __LP64__
    return (static_cast<uint64_t>(rand()) << 32) | (static_cast<uint64_t>(rand()));
#else
    return static_cast<uint32_t>(rand());
#endif
}

/*const promote::StandardRandomGenerator&
promote::StandardRandomGenerator::instance()
{
    static StandardRandomGenerator instance;
    return instance;
}*/

