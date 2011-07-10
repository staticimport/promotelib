#ifndef PROMOTE_FILE_DESCRIPTOR_HPP_
#define PROMOTE_FILE_DESCRIPTOR_HPP_

#include <exception>

#include "String.hpp"

namespace promote
{
    class FileDescriptor {
    public:
        class Exception : public std::exception {
        public:
            Exception(String const& reason) : _reason(reason) { }
            ~Exception() throw() { }
            char const* what() const throw() { return _reason.c_str(); }
        private:
            String _reason;
        };


        FileDescriptor(int const raw);
        ~FileDescriptor();

        // Const
        int raw() const { return _raw; }
        void setNonBlocking() const;
        void setReusable() const;
    private:
        // Not supported
        FileDescriptor(FileDescriptor const& fd) : _raw(-1) { }

        int const _raw;
    };
}

#endif /* PROMOTE_FILE_DESCRIPTOR_HPP_ */

