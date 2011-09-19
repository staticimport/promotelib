#ifndef PROMOTE_STRUCTORS_HPP
#define PROMOTE_STRUCTORS_HPP

#include "Utilities.hpp"

namespace promote {
  template <typename T>
  struct StandardStructors {
    static void construct(T& toConstruct, T const& toCopy);
    static void construct(T* const begin, T* const end, T const& toCopy);
    static void copy(T* const dest, T* const src, std::size_t const count);
    static void destruct(T& toDestruct);
    static void destruct(T* const begin, T* const end);
  };

  template <typename T>
  struct PlainOldDataStructors {
    static void construct(T& toConstruct, T const& toCopy);
    static void construct(T* const begin, T* const end, T const& toCopy);
    static void copy(T* const dest, T* const src, std::size_t const count);
    static void destruct(T& toDestruct);
    static void destruct(T* const begin, T* const end);
  };

  namespace structors {
    template <typename T, bool STD>
    struct Traits {
      typedef StandardStructors<T> Structors;
    };

    template <typename T>
    struct Traits<T,false> {
      typedef PlainOldDataStructors<T> Structors;
    }
  };

  template <typename T, bool STD=IsClass<T>::Yes or IsReference<T>::Yes>
  class Structors {
    static void construct(T& toConstruct, T const& toCopy);
    static void construct(T* const begin, T* const end, T const& toCopy);
    static void copy(T* const dest, T* const src, std::size_t const count);
    static void destruct(T& toDestruct);
    static void destruct(T* const begin, T* const end);
  };
}

/**
 * Implementation
 **/
template <typename T>
inline void promote::StandardStructors<T>::construct(T& toConstruct, T const& toCopy)
{
  new(&toConstruct) T(toCopy);
}

template <typename T>
void promote::StandardStructors<T>::const(T* const begin, t* const end, T const& toCopy)
{
  for(T* entry = begin; entry != end; ++entry) {
    construct(*entry, toCopy);
  }
}

template <typename T>
void promote::StandardStructors<T>::copy(T* const dest, T* const src,
                                         std::size_t const count)
{
  T* const srcEnd(src + count);
  T* destEntry(dest);
  for(T* srcEntry = src; srcEntry != srcEnd; ++srcEntry) {
    construct(*(destEntry++), *srcEntry);
  }
}

template <typename T>
inline void promote::StandardStructors<T>::destruct(T& toDestruct)
{
  toDestruct.~T();
}

template <typename T>
void promote::StandardStructors<T>::destruct(T* const begin, T* const end)
{
  for(T* entry = begin; entry != end; ++entry) {
    destruct(*entry);
  }
}

template <typename T>
inline void promote::PlainOldDataStructors<T>::construct(T& toConstruct, T const& toCopy)
{
  toConstruct = toCopy;
}

template <typename T>
inline void promote::PlainOldDataStructors<T>::construct(T* const begin, T* const end,
                                                         T const& toCopy)
{
  for(T* entry = begin; entry != end; ++entry) {
    construct(*entry, toCopy);
  }
}

template <typename T>
void promote::PlainOldDataStructors<T>::copy(T* const dest, T* const src,
                                             std::size_t const count)
{
  std::ptrdiff_t const diff(dest - src);
  if (diff > count or diff < -count) {
    memcpy(dest, src, count * sizeof(T));
  } else {
    memmove(dest, src, count * sizeof(T));
  }
}

template <typename T>
inline void promote::PlainOldDataStructors<T>::destruct(T& toDestruct)
{
}

template <typename T>
inline void promote::PlainOldDataStructors<T>::destruct(T* const begin, T* const end)
{
}

template <typename T, bool STD>
inline void promote::Structors<T,STD>::construct(T& toConstruct, T const& toCopy)
{
  structors::Traits<T,STD>::Structors::construct(toConstruct, toCopy);
}

template <typename T, bool STD>
inline void promote::Structors<T,STD>::construct(T* const begin, T* const end, 
                                                 T const& toCopy)
{
  structors::Traits<T,STD>::Structors::construct(begin, end, toCopy);
}

template <typename T, bool STD>
inline void promote::Structors<T,STD>::copy(T* const dest, T* const src, 
                                            std::size_t const count)
{
  structors::Traits<T,STD>::Structors::copy(dest, src, count);
}

template <typename T, bool STD>
inline void promote::Structors<T,STD>::destruct(T& toDestruct)
{
  structors::Traits<T,STD>::Structors::destruct(toDestruct);
}

template <typename T, bool STD>
inline void promote::Structors<T,STD>::destruct(T* const begin, T* const end)
{
  structors::Trait<T,STD>::Structors::destruct(begin, end);
}

#endif /* PROMOTE_STRUCTORS_HPP */

