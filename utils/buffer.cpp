#include "utils/buffer.h"
#include <cstring>
#include <algorithm>
#include <stdexcept>

Buffer::Buffer()
        :length(0), head(0)
{
//    printf("Buffer constructor %p\n", this);
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
            block->nextBlock = new BufferBlock;
            block = block->nextBlock;
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

BufferPtr Buffer::split(size_t pos)
{
    if (pos > length)
    {
        throw std::out_of_range("Buffer access out of range");
    }

    size_t actualPos = pos + head;
    size_t blockIndex = actualPos >> BLOCK_SIZE_EXP;
    size_t blockPtr = actualPos - (blockIndex << BLOCK_SIZE_EXP);

    BufferPtr newBuffer = std::shared_ptr<Buffer>(new Buffer);

    for (int i = 0; i < blockIndex; i++)
    {
        BufferBlock* block = this->blocks[i];
        newBuffer->blocks.push_back(block);
    }

    BufferBlock* lastBlock = new BufferBlock;

    newBuffer->blocks.back()->nextBlock = lastBlock;

    lastBlock->length = blockPtr;
    memcpy(lastBlock->block, this->blocks[blockIndex]->block, blockPtr);

    newBuffer->blocks[0] = lastBlock;
    newBuffer->head = head;
    newBuffer->length = pos;

    blocks.erase(blocks.begin(), blocks.begin() + blockIndex);

    head = blockPtr;
    length -= pos;

    return newBuffer;
}

Buffer::~Buffer()
{
    printf("Buffer destructor %p\n", this);
    while (!blocks.empty())
    {
        printf("delete Block %p\n", blocks.back());
        delete blocks.back();
        blocks.pop_back();
    }
}

size_t Buffer::len()
{
    return length;
}

Buffer::iterator::iterator(Buffer* buffer, size_t pos)
        :buffer(buffer), pos(pos) { }

Buffer::iterator Buffer::begin()
{
    return Buffer::iterator(this, 0);
}

Buffer::iterator Buffer::end()
{
    return Buffer::iterator(this, length - 1);
}

size_t Buffer::find(const char* pattern, size_t len)
{
    size_t i = 0;
    for (; i < this->len(); i++)
    {
        for (size_t j = 0; j < len; j++)
        {
            if ((*this)[i + j] != pattern[j])
            {
                break;
            }
            else if (j == (len - 1))
            {
                return i;
            }
        }
    }
    return i;
}

size_t Buffer::find(char c)
{
    size_t i = 0;
    for (; i < this->len(); i++)
    {
        if ((*this)[i] == c)
        {
            break;
        }
    }
    return i;
}

std::string Buffer::toString()
{
    return toString(0, len());
}

std::string Buffer::toString(size_t from, size_t to)
{
    std::string s;
    s.reserve(to - from);

    size_t actualFrom = from + head;
    size_t fromBlockIndex = actualFrom >> BLOCK_SIZE_EXP;
    size_t fromBlockPtr = actualFrom - (fromBlockIndex << BLOCK_SIZE_EXP);

    size_t actualTo = to + head;
    size_t toBlockIndex = actualTo >> BLOCK_SIZE_EXP;
    size_t toBlockPtr = actualTo - (fromBlockIndex << BLOCK_SIZE_EXP);

    if (fromBlockIndex == toBlockIndex)
    {
        s.append(blocks[fromBlockIndex]->block + fromBlockPtr, toBlockPtr - fromBlockPtr);
    }
    else
    {
        s.append(blocks[fromBlockIndex]->block + fromBlockPtr, blocks[fromBlockIndex]->length - fromBlockPtr);
        for (int i = fromBlockIndex + 1; i < toBlockIndex - 1; i++)
        {
            s.append(blocks[i]->block, blocks[i]->length);
        }
        s.append(blocks[toBlockIndex]->block, toBlockPtr);
    }

    return s;
}

BufferBlock::BufferBlock()
{
    this->length = 0;
    this->block = new char[BLOCK_SIZE];
    this->nextBlock = nullptr;
}

BufferBlock::~BufferBlock()
{
    delete[] this->block;
}

char& Buffer::iterator::operator*()
{
    return (*buffer)[pos];
}

Buffer::iterator Buffer::iterator::operator++()
{
    iterator it = *this;
    pos++;
    return it;
}

Buffer::iterator Buffer::iterator::operator++(int)
{
    pos++;
    return *this;
}

char* Buffer::iterator::operator->()
{
    return &(*buffer)[pos];
}

bool Buffer::iterator::operator==(const Buffer::iterator rhs)
{
    return buffer == rhs.buffer && pos == rhs.pos;
}

bool Buffer::iterator::operator!=(const Buffer::iterator rhs)
{
    return buffer != rhs.buffer || pos != rhs.pos;
}
