#ifndef PROMOTE_VECTOR_HPP
#define PROMOTE_VECTOR_HPP

namespace promote {
  namespace vector {
    template <typename T, bool POD>
    struct Traits {

    };

    template <typename T>
    struct Traits<T,false> {

    };
  };

  template <typename T, bool POD=IsClass<T>::No and IsReference<T>::No,
            typename Allocator=std::allocator<T> >
  class Vector {
  public:
    // Type defs
    typedef Allocator::const_pointer const_pointer;
    typedef Allocator::const_reference const_reference;
    typedef Allocator::difference_type difference_type;
    typedef Allocator::pointer pointer;
    typedef Allocator::reference reference;
    typedef Allocator::size_type size_type;
    typedef Allocator::value_type value_type;
  
    // Constructors
    explicit Vector(Allocator const& allocator = Allocator());
    explicit Vector(size_type const n, const_reference value = T(),
                    Allocator const& allocator = Allocator());
    template <typename InputIterator>
    Vector(InputIterator first, InputIterator last,
           Allocator const& allocator = Allocator());
    Vector(Vector<T,POD,Allocator> const& v);
  private:
    static size_type const DEFAULT_CAPACITY = 16;

    Allocator _allocator;
    T* _begin;
    T* _end;
    T* _max;
  };
}

/**
 * Implementation
 **/
template <typename T, bool P, typename A>
promote::Vector<T,P,A>::Vector(A const& allocator)
: _allocator(allocator),
  _begin(allocator.allocate(DEFAULT_CAPACITY)),
  _end(_begin),
  _max(_begin + DEFAULT_CAPACITY)
{
}

template <typename T, bool P, typename A>
promote::Vector<T,P,A>::Vector(size_t const n, const_reference value = T(),
                               Allocator const& allocator = Allocator())
: _allocator(allocator),
  _begin(allocator.allocate(n)),
  _end(_begin),
  _max(_begin + n)
{
  Structors<T,P>::construct(_begin, _max, value);
}

template <typename T, bool P, typename A>
template <typename I>
promote::Vector<T,P,A>::Vector<I>(I first, I last, 
                                  Allocator const& allocator = Allocator())
: _allocator(allocator),
  _begin(allocator.allocate(DEFAULT_CAPACITY)),
  _end(_begin),
  _max(_begin + DEFAULT_CAPACITY)
{
  while (first != last) {
    push_back(*(first++));
  }
}

template <typename T, bool P, typename A>
promote::Vector<T,P,A>::Vector(promote::Vector<T,P,A> const& v)
: _allocator(v._allocator),
  _begin(_allocator.allocate(v.size())),
  _end(_begin),
  _max(_begin + v.size())
{
  for(auto iter = v.begin(); iter != v.end(); ++iter) {
    push_back(*iter);
  }
}

#endif /* PROMOTE_VECTOR_HPP */

