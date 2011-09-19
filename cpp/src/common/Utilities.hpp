#ifndef PROMOTE_UTILITIES_HPP
#define PROMOTE_UTILITIES_HPP

namespace promote {
  template <typename T>
  class IsClass
  {
  private:
    typedef char One;
    typedef struct { char a[2]; } Two;
    template <typename C> static One test(int (C::*pmt));
    template <typename C> static Two test(...);
  public:
    enum { Yes = sizeof(test<T>(0)) == 1 };
    enum { No = !Yes };
  };

  template <typename T>
  class IsReference
  {
  private:
    typedef char One;
    typedef struct { char a[2]; } Two;
    template <typename C> static One test(C* x);
    template <typename C> static Two test(...);
  public:
    enum { Yes = sizeof(test<T>(0)) == 2 };
    enum { No = !Yes };
  };

  template <typename T, bool IS_REF=IsReference<T>::Yes >
  class Reference
  {
  public:
    typedef T Type;
  };

  template <typename T>
  class Reference<T,false>
  {
  public:
    typedef T& Type;
  };

  template <bool first, typename T1, typename T2>
  class TypeChooser
  {
  public:
    typedef T1 Type;
  };

  template <typename T1, typename T2>
  class TypeChooser<false, T1, T2>
  {
  public:
    typedef T2 Type;
  };

  template <typename T>
  class Passable
  {
  public:
    typedef typename TypeChooser<IsClass<T>::No, T, typename Reference<T>::Type>::Type Type;
  };
}

#endif /* PROMOTE_UTILITIES_HPP */

