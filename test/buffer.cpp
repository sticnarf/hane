#include <iostream>
#include <chrono>
#include <string>
#include <utils/buffer.h>

int main()
{
    std::string s;
    auto begin = std::chrono::steady_clock::now();
    for (int i = 0; i < 100000; i++)
    {
        s += "Hello, buffer!";
        s += " Good morning!\n";
        s += "Hello, buffer!";
        s += " Good night!\n";
    }
    auto end = std::chrono::steady_clock::now();
    std::cout << "Time difference = "
              << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << std::endl;
    begin = std::chrono::steady_clock::now();
    for (int i = 0; i < s.length(); i++)
    {
        char c = s[i];
    }
    end = std::chrono::steady_clock::now();
    std::cout << "Time difference = "
              << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << std::endl;
    begin = std::chrono::steady_clock::now();
    for (; s.length() > 10000;)
    {
        s = s.substr(10000);
    }
    end = std::chrono::steady_clock::now();
    std::cout << "Time difference = "
              << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << std::endl;

    Buffer buffer;
    begin = std::chrono::steady_clock::now();
    for (int i = 0; i < 100000; i++)
    {
        buffer.push("Hello, buffer!", 14);
        buffer.push(" Good morning!\n What the Fuck!", 15);
        buffer.push("Hello, buffer!", 14);
        buffer.push(" Good night!\n What the Fuck!", 13);
    }
    end = std::chrono::steady_clock::now();
    std::cout << "Time difference = "
              << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << std::endl;
    begin = std::chrono::steady_clock::now();
    for (int i = 0; i < buffer.len(); i++)
    {
        char c = buffer[i];
    }
    end = std::chrono::steady_clock::now();
    std::cout << "Time difference = "
              << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << std::endl;
    begin = std::chrono::steady_clock::now();
    for (; buffer.len() > 10000;)
    {
        buffer.split(10000);
    }
    end = std::chrono::steady_clock::now();
    std::cout << "Time difference = "
              << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << std::endl;
}
