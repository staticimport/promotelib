#ifndef PROMOTE_ATOMIC_HPP_
#define PROMOTE_ATOMIC_HPP_

namespace promote
{
    template <typename T>
    class Atomic
    {
    public:
        Atomic(const T initValue=0)
        :   _value(initValue)
        { }
        ~Atomic() { }

        /* Const */
        T fetch() const { return _value; }

        /* Compare-and-Swap */
        bool compareAndSwap(T compare, T swap)
        {
            return __sync_bool_compare_and_swap(&_value, compare, swap);
        }
        T fetchCompareAndSwap(T compare, T swap)
        {
            return __sync_val_compare_and_swap(&_value, compare, swap);
        }
        
        /* Fetch and Operate */
        T fetchAndAdd(T inc)        { return __sync_fetch_and_add(&_value, inc); }
        T fetchAndAnd(T value)      { return __sync_fetch_and_and(&_value, value); }
        T fetchAndNand(T value)     { return __sync_fetch_and_nand(&_value, value); }
        T fetchAndOr(T value)       { return __sync_fetch_and_or(&_value, value); }
        T fetchAndSubtract(T dec)   { return __sync_fetch_and_sub(&_value, dec); }
        T fetchAndXor(T value)      { return __sync_fetch_and_xor(&_value, value); }

        /* Operate and Fetch */
        T addAndFetch(T inc)        { return __sync_add_and_fetch(&_value, inc); }
        T andAndFetch(T value)      { return __sync_and_and_fetch(&_value, value); }
        T nandAndFetch(T value)     { return __sync_nand_and_fetch(&_value, value); }
        T orAndFetch(T value)       { return __sync_or_and_fetch(&_value, value); }
        T subtractAndFetch(T dec)   { return __sync_sub_and_fetch(&_value, dec); }
        T xorAndFetch(T value)      { return __sync_xor_and_fetch(&_value, value); }

        void set(T const value)     { _value = value; }
    private:
        T volatile _value;
    };
}


#endif /* PROMOTE_ATOMIC_HPP_ */
