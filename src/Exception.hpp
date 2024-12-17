#pragma once
#include<exception>

class InvalidOpertionException : public std::exception {
public:
    const char* what() const noexcept override {
        return "Invalid operation";
    }
};