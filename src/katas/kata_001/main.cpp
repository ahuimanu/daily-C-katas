#include <iostream>
#include <cstdio>
#include <cassert>

class FileGuard
{
public:
    explicit FileGuard(const char *path, const char *mode);
    ~FileGuard();

    FileGuard(const FileGuard &) = delete;
    FileGuard &operator=(const FileGuard &) = delete;

    FileGuard(FileGuard &&other) noexcept;
    FileGuard &operator=(FileGuard &&other) noexcept;

    std::FILE *get() const;
    explicit operator bool() const;

private:
    std::FILE *file_ = nullptr;
};

FileGuard::FileGuard(const char *path, const char *mode)
{
    file_ = std::fopen(path, mode);
}

FileGuard::~FileGuard()
{
    if (file_)
    {
        std::fclose(file_);
        file_ = nullptr;
    }
}

FileGuard::FileGuard(FileGuard &&other) noexcept : file_(other.file_)
{
    other.file_ = nullptr;
}

FileGuard &FileGuard::operator=(FileGuard &&other) noexcept
{
    if (this != &other)
    {
        if (file_)
        {
            std::fclose(file_);
            file_ = nullptr;
        }
        file_ = other.file_;
        other.file_ = nullptr;
    }
    return *this;
}

std::FILE *FileGuard::get() const
{
    return file_;
}

FileGuard::operator bool() const
{
    return file_ != nullptr;
}

int main()
{
    {
        std::cout << "Writing to example.txt\n"
                  << std::endl;
        FileGuard file("example.txt", "w");
        assert(file && "Failed to open file");
        if (file)
        {
            std::fputs("Hello, World!\n", file.get());
            std::cout << "File written successfully." << std::endl;
        }
        else
        {
            std::cerr << "Failed to open file." << std::endl;
        }

        std::cout << "Changing File Handle\n"
                  << std::endl;

        FileGuard other = std::move(file);
        assert(!file && "Original file handle should be null after move");
        assert(other && "New file handle should be valid after move");
    }

    {

        std::cout << "Now in additional scope\n"
                  << std::endl;

        FileGuard again("example.txt", "a");
        assert(again && "Failed to open file again");        
        std::fputs("Second write after close\n", again.get());

    }

    return 0;
}