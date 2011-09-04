#ifndef PROMOTE_THREADS_HPP
#define PROMOTE_THREADS_HPP

#include <pthread.h>
#include <stdint.h>

#include "Utilities.hpp"

namespace promote {
    namespace threads {
        template <std::size_t PTHREAD_SIZE=sizeof(pthread_t)>
        struct Traits {
            static void copy(pthread_t& dest, typename Passable<pthread_t const>::Type source)
            {
                memcpy(&dest, &source, sizeof(pthread_t));
            }

            static bool equal(typename Passable<pthread_t const>::Type t1, 
                              typename Passable<pthread_t const>::Type t2)
            {
                return pthread_equal(t1, t2) != 0;
            }
        };

        template <>
        struct Traits<1> {
            static void copy(pthread_t& dest, pthread_t const source)
            {
                *reinterpret_cast<uint8_t*>(&dest) = *reinterpret_cast<uint8_t const*>(&source);
            }

            static bool equal(pthread_t const t1, pthread_t const t2) {
                return *reinterpret_cast<uint8_t const*>(&t1) == *reinterpret_cast<uint8_t const*>(&t2);
                //return (uint8_t)t1 == (uint8_t)t2;
                //return static_cast<uint8_t>(t1) == static_cast<uint8_t>(t2);
                //return *((uint8_t*)&t1) == *((uint8_t*)&t2);
            }
        };
        
        template <>
        struct Traits<2> {
            static void copy(pthread_t& dest, pthread_t const source)
            {
                *reinterpret_cast<uint16_t*>(&dest) = *reinterpret_cast<uint16_t const*>(&source);
            }

            static bool equal(pthread_t const t1, pthread_t const t2) {
                //return reinterpret_cast<uint16_t>(t1) == reinterpret_cast<uint16_t>(t2);
                return *reinterpret_cast<uint16_t const*>(&t1) == *reinterpret_cast<uint32_t const*>(&t2);
            }
        };
        template <>
        struct Traits<4> {
            static void copy(pthread_t& dest, pthread_t const source)
            {
                *reinterpret_cast<uint32_t*>(&dest) = *reinterpret_cast<uint32_t const*>(&source);
            }

            static bool equal(pthread_t const t1, pthread_t const t2) {
                //return reinterpret_cast<uint32_t>(t1) == reinterpret_cast<uint32_t>(t2);
                return *reinterpret_cast<uint32_t const*>(&t1) == *reinterpret_cast<uint32_t const*>(&t2);
            }
        };
        template <>
        struct Traits<8> {
            static void copy(pthread_t& dest, pthread_t const source)
            {
                *reinterpret_cast<uint64_t*>(&dest) = *reinterpret_cast<uint64_t const*>(&source);
            }

            static bool equal(pthread_t const t1, pthread_t const t2) {
                return reinterpret_cast<uint64_t>(t1) == reinterpret_cast<uint64_t>(t2);
            }
        };
    }

    struct Threads {
        static void copy(pthread_t& dest, Passable<pthread_t const>::Type source) {
            threads::Traits<>::copy(dest, source);
        }

        static bool equal(Passable<pthread_t const>::Type t1,
                          Passable<pthread_t const>::Type t2)
        {
            return threads::Traits<>::equal(t1, t2);
        }
    };
}

#endif /* PROMOTE_THREADS_HPP */

