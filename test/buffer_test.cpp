
#include <iostream>
#include <chrono>
#include <string>
#include <fstream>
#include <cstdlib>
#include <cassert>
#include <ctime>
#include <algorithm>
#include <utils/buffer.h>

Buffer* buffer;

std::string rawString;

void prepareData()
{
    buffer = new Buffer;
    rawString = "";
    for (int i = 0; i < 1000 || rawString.length() < 25000; i++)
    {
        int len = rand() % 10000;
        std::string subStr;
        for (int j = 0; j < len; j++)
        {
            subStr.push_back((char) (rand() % 128));
        }
        rawString += subStr;
        buffer->push(subStr.c_str(), len);
    }
}

void cleanData()
{
    delete buffer;
}

void pushTest()
{
    prepareData();

    std::string result = buffer->toString(10000, 20000);

    assert(rawString.substr(10000, 10000) == result);
    assert(rawString == buffer->toString());

    cleanData();
}

void splitTest()
{
    prepareData();

    int len;
    for (int ptr = 0; ptr < rawString.length(); ptr += len)
    {
        len = std::min((int)buffer->len(), rand() % 15000);
        BufferPtr subBuf = buffer->split(len);
        assert(subBuf->toString() == rawString.substr(ptr, len));
    }

    cleanData();
}

int main()
{
    srand(time(NULL));

    pushTest();

    splitTest();

    for(;;);
}
