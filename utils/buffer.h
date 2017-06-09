#ifndef RACKCPP_BUFFER_H
#define RACKCPP_BUFFER_H

#include <cstdlib>
#include <vector>

static const size_t BLOCK_SIZE_EXP = 12;

static const size_t BLOCK_SIZE = 1 << BLOCK_SIZE_EXP;

struct BufferBlock
{
    char* block;
    size_t length;

    BufferBlock();
    ~BufferBlock();
};

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
    char operator[](size_t index) const;
    ~Buffer();
};

#endif
