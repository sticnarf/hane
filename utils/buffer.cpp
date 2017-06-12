#include "buffer.h"
#include <cstring>
#include <algorithm>
#include <stdexcept>

Buffer::Buffer()
        :length(0), head(0)
{
    blocks.push_back(new BufferBlock);
}

void Buffer::push(const char* bytes, size_t len)
{
    size_t bytesPtr = 0;
    while (bytesPtr < len)
    {
        BufferBlock* block = blocks.back();
        if (block->length == BLOCK_SIZE)
        {
            block = new BufferBlock;
            blocks.push_back(block);
        }
        size_t& blockPtr = block->length;
        size_t copySize = std::min(len - bytesPtr, BLOCK_SIZE - blockPtr);
        memcpy(block->block + blockPtr, bytes + bytesPtr, copySize);
        this->length += copySize;
        bytesPtr += copySize;
        blockPtr += copySize;
    }
}

char& Buffer::operator[](size_t index)
{
    if (index > length)
    {
        throw std::out_of_range("Buffer access out of range");
    }
    size_t actualIndex = index + head;
    size_t blockIndex = actualIndex >> BLOCK_SIZE_EXP;
    size_t blockPtr = actualIndex - (blockIndex << BLOCK_SIZE_EXP);
    return blocks[blockIndex]->block[blockPtr];
}

Buffer Buffer::split(size_t pos)
{
    if (pos > length)
    {
        throw std::out_of_range("Buffer access out of range");
    }
    size_t actualPos = pos + head;
    size_t blockIndex = actualPos >> BLOCK_SIZE_EXP;
    size_t blockPtr = actualPos - (blockIndex << BLOCK_SIZE_EXP);
    Buffer newBuffer;
    for (int i = 0; i < blockIndex; i++)
    {
        BufferBlock* block = this->blocks[i];
        newBuffer.blocks.push_back(block);
    }
    blocks.erase(blocks.begin(), blocks.begin() + blockIndex);
    newBuffer.head = head;
    newBuffer.length = pos;
    head = blockPtr;
    length -= pos;
    return newBuffer;
}

Buffer::~Buffer()
{
    while (!blocks.empty())
    {
        delete blocks.back();
        blocks.pop_back();
    }
}

size_t Buffer::len()
{
    return length;
}

Buffer::iterator::iterator(Buffer* buffer, size_t pos)
        :buffer(buffer), pos(pos)
{
    ptr = &(*buffer)[pos];
}

Buffer::iterator Buffer::begin()
{
    return Buffer::iterator(this, 0);
}
Buffer::iterator Buffer::end()
{
    return Buffer::iterator(this, length - 1);
}

BufferBlock::BufferBlock()
{
    this->length = 0;
    this->block = new char[BLOCK_SIZE];
}

BufferBlock::~BufferBlock()
{
    delete[] this->block;
}

char& Buffer::iterator::operator*()
{
    return *ptr;
}

Buffer::iterator Buffer::iterator::operator++()
{
    iterator it = *this;
    pos++;
    ptr = &(*buffer)[pos];
    return it;
}

Buffer::iterator Buffer::iterator::operator++(int)
{
    pos++;
    ptr = &(*buffer)[pos];
    return *this;
}

char* Buffer::iterator::operator->()
{
    return ptr;
}
bool Buffer::iterator::operator==(const Buffer::iterator& rhs)
{
    return ptr == rhs.ptr;
}
bool Buffer::iterator::operator!=(const Buffer::iterator& rhs)
{
    return ptr != rhs.ptr;
}
