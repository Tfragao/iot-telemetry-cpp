#pragma once 

namespace iot::platform {
    class ScopedFileDescriptor {
        public:
            ScopedFileDescriptor() noexcept = default;
            explicit ScopedFileDescriptor(int fd) noexcept;

            ~ScopedFileDescriptor();

            ScopedFileDescriptor(const ScopedFileDescriptor&) = delete;
            ScopedFileDescriptor& operator=(const ScopedFileDescriptor&) = delete;

            ScopedFileDescriptor(ScopedFileDescriptor&& other) noexcept;
            ScopedFileDescriptor& operator=(ScopedFileDescriptor&& other) noexcept;

            [[nodiscard]] int get() const noexcept;
            [[nodiscard]] bool is_valid() const noexcept;
            [[nodiscard]] int release() noexcept;
            void reset(int new_fd_ = invalid_fd) noexcept;

        private:
            static constexpr int invalid_fd{-1};
            int fd_{invalid_fd};
    };
}