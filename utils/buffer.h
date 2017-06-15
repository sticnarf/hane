#ifndef RACKCPP_BUFFER_H
#define RACKCPP_BUFFER_H

#include <cstdlib>
#include <string>
#include <vector>

static const size_t BLOCK_SIZE_EXP = 12;

static const size_t BLOCK_SIZE = 1 << BLOCK_SIZE_EXP;

struct BufferBlock
{
    char* block;
    size_t length;
    BufferBlock* nextBlock;

    BufferBlock();
    ~BufferBlock();
};

/**
 * Not optimized! Random access is about 20 times slower than using an array.
 * And, do not use iterator!
 * It performs even worse than random access!
 */
class Buffer
{
private:
    std::vector<BufferBlock*> blocks;
    size_t length;
    size_t head;

public:
    Buffer();
    void push(const char* bytes, size_t len);
    Buffer split(size_t pos);
    size_t len();
    char& operator[](size_t index);
    ~Buffer();

    class iterator
    {
    public:
        friend class Buffer;

        iterator operator++();
        iterator operator++(int);
        char& operator*();
        char* operator->();
        bool operator==(const iterator rhs);
        bool operator!=(const iterator rhs);
    private:
        Buffer* buffer;
        size_t pos;

        iterator(Buffer* buffer, size_t pos);
    };

    iterator begin();
    iterator end();

    size_t find(char c);
    size_t find(const char* pattern, size_t len);

    std::string toString();
    std::string toString(size_t from, size_t to);
};

#endif
