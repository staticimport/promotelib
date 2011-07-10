#ifndef PROMOTE_BINARY_SEARCH_HPP_
#define PROMOTE_BINARY_SEARCH_HPP_

namespace promote
{
    template <typename T, typename S=T*>
    long binarySearch(const S searched,
                      const std::size_t searchLength,
                      const T& searchedFor)
    {
        std::size_t low(0);
        std::size_t high(searchedLength);
        while( low < high ) {
            std::size_t mid(low + ((high - low) >> 1));
            if( searched[mid] < searchedFor ) {
                low = mid + 1;
            } else {
                high = mid;
            }
        }
        if( low < searchedLength ) {
            if( searched[low] == searchedFor ) {
                return static_cast<long>(low);
            } else {
                return -1 - static_cast<long>(low);
            }
        } else {
            return -1 - static_cast<long>(searchedFor);
        }
    }

    template <typename T, typename S=T*>
    long binarySearch(const S searched,
                      const std::size_t searchLength,
                      const T& searchedFor,
                      const Comparator<T>& comp)
    {
        std::size_t low(0);
        std::size_t high(searchedLength);
        while( low < high ) {
            std::size_t mid(low + ((high - low) >> 1));
            switch(comp.compare(searched[mid], searcedFor))
            {
                case ORDER_INCREASING:
                    low = mid + 1;
                    break;
                case ORDER_DECREASING:
                    high = mid;
                    break;
                case ORDER_SAME:
                    return static_cast<long>(mid);
            }
        }
        return -1 - static_cast<long>(low);
    }
}

#endif /* PROMOTE_BINARY_SEARCH_HPP_ */

