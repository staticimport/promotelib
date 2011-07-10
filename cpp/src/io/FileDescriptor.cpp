
#include <cerrno>
#include <cstring>
#include <fcntl.h>
#include <sys/socket.h>

#include "FileDescriptor.hpp"
#include "FormattedString.hpp"

using namespace promote;

FileDescriptor::FileDescriptor(int const raw)
:   _raw(raw)
{
}

FileDescriptor::~FileDescriptor()
{
    close(_raw);
}

void FileDescriptor::setNonBlocking() const {
    int const oldFlags(fcntl(_raw, F_GETFL, 0));
    if( fcntl(_raw, F_SETFL, oldFlags | O_NONBLOCK) < 0 ) {
        FormattedString const reason("Failed to set non-blocking: errno=%d, reason=%s",
                                     errno, strerror(errno));
        throw Exception(reason);
    }
}

void FileDescriptor::setReusable() const {
    int const flag(1);
    int const result(setsockopt(_raw, SOL_SOCKET, SO_REUSEADDR,
                                &flag, sizeof(int)));
    if( result < 0 ) {
        FormattedString const reason("Failed to set reusable: errno=%d, reason=%s",
                                     errno, strerror(errno));
        throw Exception(reason);
    }
}

