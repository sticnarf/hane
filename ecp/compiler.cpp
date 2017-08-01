/**
 * This compiler reads template from STDIN and outputs the C++ code to STDOUT.
 * Also, two command line arguments must be given:
 * the associated header file which is to be included,
 * and the template class name.
 */

#include <iostream>
#include <string>
#include <cstdio>
#include <vector>
#include <utility>
#include <random>
#include <spdlog/fmt/fmt.h>

enum Type {
    Text,
    Statement,
    Expression
};

struct CppBuilder {
    std::string headerFile, className, templateText, result;
    std::vector<std::pair<Type, std::string>> nodes;

    void build();

    void buildMacros();

    void buildRenderFunction();

    void buildRenderBody();

    void insertText(const std::string &text);

    void insertStatement(const std::string &statement);

    void insertExpression(const std::string &expression);

    void buildNodes(size_t begin = 0);

    size_t findEndIdentifier(size_t begin);
} builder;

void CppBuilder::build() {
    buildMacros();
    buildRenderFunction();
}

void CppBuilder::buildMacros() {
    // Include the header file
    result += fmt::format("#include \"{}\"\n", headerFile);
}

void CppBuilder::buildRenderFunction() {
    // To implement the render method
    result += fmt::format("std::string {}::render() {{\n", className);
    buildRenderBody();
    result += "}\n";
}

void CppBuilder::buildRenderBody() {
    buildNodes();

    // Create the result string
    result += "std::string result;\n";

    for (auto node : nodes) {
        switch (node.first) {
            case Text:
                insertText(node.second);
                break;
            case Statement:
                insertStatement(node.second);
                break;
            case Expression:
                insertStatement(node.second);
                break;
        }
    }
}

void CppBuilder::buildNodes(size_t begin) {
    size_t pos = templateText.find("<%", begin);

    if (pos == std::string::npos) {
        if (begin < (templateText.length() - 1))
            nodes.push_back({Text, templateText.substr(begin)});
        return;
    }
    if (pos > begin)
        nodes.push_back({Text, templateText.substr(begin, pos - begin)});

    Type type = templateText[pos + 1] == '=' ? Expression : Statement;
    size_t end = findEndIdentifier(pos + 2);
    size_t beg = pos + (type == Expression ? 2 : 3);
    nodes.push_back({type, templateText.substr(beg, end - beg)});

    buildNodes(end + 2);
}

size_t CppBuilder::findEndIdentifier(size_t begin) {
    // TODO
    return 0;
}

std::string randomToken() {
    static std::random_device rd;
    static std::mt19937 mt(rd());
    static const char alphanum[] = "0123456789"
            "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
            "abcdefghijklmnopqrstuvwxyz";
    static std::uniform_int_distribution<> dis(0, sizeof(alphanum) - 2);

    const int len = 8;
    char result[len + 1];

    for (int i = 0; i < len; i++)
        result[i] = alphanum[dis(mt)];
    result[len] = '\0';

    return std::string(result);
}

void CppBuilder::insertText(const std::string &text) {
    std::string token = randomToken();

    result += fmt::format("result += R\"{0}({1}){0}\";\n", token, text);
}

void CppBuilder::insertStatement(const std::string &statement) {
    // TODO
}

void CppBuilder::insertExpression(const std::string &expression) {
    // TODO
}


int main(int argc, char *argv[]) {
    if (argc < 3) {
        std::cerr << "No enough command line arguments!" << std::endl;
        return 1;
    }

    builder.headerFile = argv[1];
    builder.className = argv[2];

    char buf[2048];
    while (fgets(buf, 2048, stdin) != nullptr) {
        builder.templateText += buf;
    }

    builder.build();

    std::cout << builder.result << std::endl;
    return 0;
}