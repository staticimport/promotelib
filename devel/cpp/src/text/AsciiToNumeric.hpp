#ifndef PROMOTE_ASCII_TO_NUMERIC_HPP_
#define PROMOTE_ASCII_TO_NUMERIC_HPP_

#include <cstring>
#include <stdint.h>
#include <string>

#include "NumericUtils.hpp"
#include "Power.hpp"

namespace promote
{
    template <typename T>
    T parseIntegral(const char* string,
                    const std::size_t length,
                    std::size_t* nextIndex=0)
    {
        bool const negative(string[0] == '-');
        std::size_t ii(negative ? 1 : 0);
        char c;
        T value(0);

        for( ; ii != length and isdigit(c = string[ii]); ++ii) {
            value = (value << 3) + (value << 1) + static_cast<T>(c - '0');
        }
        if( nextIndex ) {
            *nextIndex = ii;
        }
        return negative ? 0 - value : value;
    }
    
    template <typename T> T parseIntegral(const std::string& string,
                                          std::size_t* nextIndex=0)
    {
        return parseIntegral<T>(string.c_str(), string.length(), nextIndex);
    }

    template <typename T> T parseFloatingPoint(const char* string,
                                               const std::size_t length,
                                               std::size_t* nextIndex=0)
    {
        bool const negative(string[0] == '-');
        std::size_t ii(negative ? 1 : 0);
        char c;
        T result;
        uint64_t asIntegral(0);

        // Before the decimal
        for( ; ii != length and isdigit(c = string[ii]); ++ii ) {
            asIntegral = (asIntegral << 3) + (asIntegral << 1) + (c - '0');
        }

        //printf("Integral part = %llu\n", (unsigned long long)asIntegral);
        if( ii != length and c == '.' ) {
            // After the decimal
            int exponent(0);
            for(++ii ; ii != length and isdigit(c = string[ii]); ++ii ){
                asIntegral = (asIntegral << 3) + (asIntegral << 1) + (c - '0');
                ++exponent;
            }
            //printf("integral now = %llu, exponent = %d\n", (unsigned long long)asIntegral, exponent);
            if( ii != length and toupper(c) == 'E' ) { 
                exponent += promote::parseIntegral<int>(string + ii, length - ii, nextIndex);
            } else if( nextIndex ) {
                *nextIndex = ii;
            }

            //printf("Integral as float: %f\n", static_cast<float>(asIntegral));
            //printf("result = %f\n", static_cast<float>(asIntegral) / promote::POWER_10s[exponent]);
            if( exponent >= 0 ) {
                result = static_cast<T>(static_cast<double>(asIntegral) / promote::POWER_10s[exponent]);
                //result = static_cast<T>(asIntegral) / promote::POWER_10s[exponent];
            } else {
                result = static_cast<T>(asIntegral) * promote::POWER_10s[0-exponent];
            }
        } else {
            if( nextIndex ) *nextIndex = ii;
            result = static_cast<T>(asIntegral);
        }

        return negative ? 0 - result : result;
    }


    template <typename T> T parseFloatingPoint(const std::string& string,
                                               std::size_t* nextIndex=0)
    {
        return parseFloatingPoint<T>(string.c_str(), string.length(), nextIndex);
    }

    
    template <typename T, bool integral=Primitive<T>::IsIntegral>//isIntegral<T>()>//Primitive<T>::IsIntegral>
    class AsciiToNumeric
    {
    public:
        static bool parse(char const* string, 
                          std::size_t const length,
                          T& output,
                          std::size_t* nextIndex=0)
        {
            if( length and (isdigit(string[0]) or (length > 1 and string[0] == '-' and isdigit(string[1]))) ) { 
                output = parseIntegral<T>(string, length, nextIndex);
                return true;
            } else {
                return false;
            }
        }
    };

    template <typename T>
    class AsciiToNumeric<T,false>
    {
    public:
        static bool parse(char const* string,
                          std::size_t const length,
                          T& output,
                          std::size_t* nextIndex=0)
        {
            if( length and (isdigit(string[0]) or (length > 1 and (string[0] == '-') and isdigit(string[1]))) ) {
                output = parseFloatingPoint<T>(string, length, nextIndex);
                return true;
            } else {
                return false;
            }
        }
    };
}

/**
 * Implemenation
 **/
/*template <typename T>
T parseFloatingPoint(const char* string,
                     const std::size_t length,
                     std::size_t* nextIndex)
{
}*/

#endif /* PROMOTE_ASCII_TO_NUMERIC_HPP_ */

