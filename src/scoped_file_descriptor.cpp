#include "../include/iot/scoped_file_descriptor.hpp"
#include <unistd.h>
#include <utility>

namespace iot::platform {
    ScopedFileDescriptor::ScopedFileDescriptor(int fd) noexcept : fd_{fd} {}
    
    ScopedFileDescriptor::~ScopedFileDescriptor() {
        reset();
    }

    ScopedFileDescriptor::ScopedFileDescriptor(ScopedFileDescriptor&& other) noexcept 
        : fd_{std::exchange(other.fd_, invalid_fd)} {}
    
    ScopedFileDescriptor& ScopedFileDescriptor::operator=(ScopedFileDescriptor&& other) noexcept {
        if (this != &other) {
            reset();
            fd_ = std::exchange(other.fd_, invalid_fd);
        }

        return *this;
    }

    int ScopedFileDescriptor::get() const noexcept {
        return fd_;
    }

    bool ScopedFileDescriptor::is_valid() const noexcept {
        return fd_ >= 0;
    }

    int ScopedFileDescriptor::release() noexcept {
        return std::exchange(fd_, invalid_fd);
    }

    void ScopedFileDescriptor::reset(int new_fd) noexcept {
        if (fd_ >= 0) {
            ::close(fd_);
        }
        fd_ = new_fd;
    }

   
}