#include <iostream>
#include <chrono>
#include <string>
#include <fstream>
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
    int x = 1;
//    std::fstream fs("/tmp/test1");
    for (int i = 0; i < s.length(); i++)
    {
//        fs << s[i];
        x ^= s[i];
    }
//    fs.close();
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
//    std::fstream fs2("/tmp/test2");
//    auto end_iterator = buffer.cend();
//    for (auto it = buffer.cbegin(); it != end_iterator; it++)
//    {
//        x ^= *it;
//        fs2 << *it;
//    }
//    fs2.close();
    for (int i = 0; i < buffer.len(); i++)
    {
        x ^= buffer[i];
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
    std::cout << x;
}
