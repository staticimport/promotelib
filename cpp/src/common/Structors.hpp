#ifndef PROMOTE_STRUCTORS_HPP
#define PROMOTE_STRUCTORS_HPP

#include "Utilities.hpp"

namespace promote {
  template <typename T>
  class StandardStructors {
  public:
    static void construct(T& toConstruct, T const& toCopy) {
      new(&toConstruct) T(toCopy);
    }
    static void construct(T* const begin, T* const end, T const& toCopy) {
      for(T* entry = begin; entry != end; ++entry) {
        construct(*entry, toCopy);
      }
    }
    static void copy(T* const dest, T* const src, std::size_t const count) {
      T* const srcEnd = src + count;
      T* destEntry = dest;
      for(T* srcEntry = src; srcEntry != srcEnd; ++srcEntry) {
        construct(*(destEntry++), *srcEntry);
      }
    }
    static void destruct(T& toDestruct) {
      toDestruct.~T();
    }
    static void destruct(T* const begin, T* const end) {
      for(T* entry = begin; entry != end; ++entry) {
        entry->~T();
      }
    }

  private:
    StandardStructors() { }
    StandardStructors(StandardStructors const& s) { }
  };

  template <typename T>
  class PlainOldDataStructors {
  public:
    static void construct(T& toConstruct, typename Passable<T>::Type toCopy) {
      toConstruct = toCopy;
    }
    static void construct(T* const begin, T* const end, 
                          typename Passable<T>::Type toCopy)
    {
      for(T* entry = begin; entry != end; ++entry) {
        *entry = toCopy;
      }
    }
    static void copy(T* const dest, T* const src, std::size_t const count) {
      long long const signedCount(static_cast<long long>(count));
      long long const ptrDiff = 
        static_cast<long long>(dest) - static_cast<long long>(src);
      if( ptrDiff > signedCount or ptrDiff < -signedCount ) {
        memcpy(dest, src, count * sizeof(T));
      } else {
        memmove(dest, src, count * sizeof(T));
      }
    }
    static void destruct(typename Passable<T>::Type toDestruct) { }
    static void destruct(T* const begin, T* const end) { }
  private:
    PlainOldDataStructors() { }
    PlainOldDataStructors(PlainOldDataStructors const& s) { }
  };

  template <typename T, 
            bool STANDARD_STRUCTORS=IsClass<T>::Yes or IsReference<T>::Yes>
  class AutoStructors {
  public:
    static void construct(T& toConstruct, T const& toCopy) {
      StandardStructors<T>::construct(toConstruct, toCopy);
    }
    static void destruct(T& toDestruct) {
      StandardStructors<T>::destruct(toDestruct);
    }
  private:
    AutoStructors() { }
    AutoStructors(AutoStructors const& s) { }
  };

  template <typename T>
  class AutoStructors<T,false> {
  public:
    static void construct(T& toConstruct, typename Passable<T const>::Type toCopy) {
      PlainOldDataStructors<T>::construct(toConstruct, toCopy);
    }
    static void destruct(typename Passable<T>::Type toDestruct) {
      PlainOldDataStructors<T>::destruct(toDestruct);
    }
  private:
    AutoStructors() { }
    AutoStructors(AutoStructors const& s) { }
  };
}

#endif /* PROMOTE_STRUCTORS_HPP */

