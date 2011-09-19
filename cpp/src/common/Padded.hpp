#ifndef PROMOTE_PADDED_HPP
#define PROMOTE_PADDED_HPP

#include <cstddef>

namespace promote {
  template <typename T>
  class Padded {
  public:
    static std::size_t const PADDING_COUNT =
      sizeof(T) > 4 ? ((sizeof(T) % 8) ? 8 - (sizeof(T) % 8) : 0) : 
      (sizeof(T) == 3 ? 1 : 0);

    Padded();
    Padded(T const& value);
    
    // Const
    inline T const& value() const;

    // Non-Const
    inline T& value();
    Padded<T> const& operator=(Padded<T> const& p);
  private:
    T _value;
    char _padding[PADDING_COUNT];
  };
}

/** 
 * Implementation
 **/
template <typename T>
promote::Padded<T>::Padded()
{
}

template <typename T>
promote::Padded<T>::Padded(T const& value)
: _value(value)
{
}

template <typename T>
inline T const& promote::Padded<T>::value() const
{
  return _value;
}

template <typename T>
inline T& promote::Padded<T>::value()
{
  return _value;
}

template <typename T>
promote::Padded<T> const& promote::Padded<T>::operator=(promote::Padded<T> const& padded)
{
  if (this != &padded) {
    _value = padded.value();
  }
  return *this;
}

#endif /* PROMOTE_PADDED_HPP */

