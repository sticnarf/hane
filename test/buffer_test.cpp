#include <iostream>
#include <chrono>
#include <string>
#include <fstream>
#include <cstdlib>
#include <cassert>
#include <ctime>
#include <random>
#include <algorithm>
#include <utils/buffer.hpp>

std::random_device rd;
std::mt19937 mt(rd());
std::uniform_int_distribution<> dis(0, 1280000);

BufferPtr buffer;

std::string rawString;

void prepareData() {
    buffer = std::make_shared<Buffer>();
    rawString = "";
    for (int i = 0; i < 1000 || rawString.length() < 25000; i++) {
        auto len = static_cast<size_t>(dis(mt) % 10000);
        std::string subStr;
        for (int j = 0; j < len; j++) {
            subStr.push_back((char) (dis(mt) % 128));
        }
        rawString += subStr;
        buffer->push(subStr.c_str(), len);
    }
}

void cleanData() {
}

void pushTest() {
    prepareData();

    std::string result = buffer->toString(10000, 20000);
    std::string expected = rawString.substr(10000, 10000);

    assert(expected == result);
    assert(rawString == buffer->toString());

    cleanData();
}

void splitTest() {
    prepareData();

    size_t len;
    for (int ptr = 0; ptr < rawString.length(); ptr += len) {
        len = std::min(buffer->len(), static_cast<size_t>(dis(mt) % 15000));
        BufferPtr subBuf = buffer->split(len);
        assert(subBuf->toString() == rawString.substr(ptr, len));
    }

    cleanData();
}

int main() {

    pushTest();

    splitTest();

}
