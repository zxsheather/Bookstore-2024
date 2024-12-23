#pragma once
#include<exception>

class InvalidOperationException : public std::exception {
public:
    const char* what() const noexcept override {
        return "Invalid";
    }
};