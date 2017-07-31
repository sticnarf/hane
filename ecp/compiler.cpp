/**
 * This compiler reads template from STDIN and outputs the C++ code to STDOUT.
 * Also, two command line arguments must be given:
 * the associated header file which is to be included,
 * and the template class name.
 */

#include <iostream>
#include <string>
#include <cstdio>

std::string headerFile, className, templateText;

int main(int argc, char *argv[]) {
    if (argc < 3) {
        std::cerr << "Not enough command line arguments!" << std::endl;
        return 1;
    }
    headerFile = argv[1];
    className = argv[2];

    char buf[2048];
    while(fgets(buf, 2048, stdin) != nullptr) {
        templateText += buf;
    }

    std::cout << templateText << std::endl;
}