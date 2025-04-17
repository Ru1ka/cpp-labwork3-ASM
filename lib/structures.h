#pragma once

#include <cinttypes>

const uint16_t DEFAULT_SIZE = 64;
const uint16_t EXPANSION_DEFAULT_SIZE = 10;

struct Args {
    char* input = nullptr;
    char* output = nullptr;
    long long max_iter = 0;
    long long freq = 0;
    bool debug = false;
};

struct Coords {
    uint16_t y = 0;
    uint16_t x = 0;

    Coords(uint16_t y = 0, uint16_t x = 0);
};

struct Vector;

struct Matrix {
    Vector* data = nullptr;
    uint16_t size_y = DEFAULT_SIZE;
    uint16_t size_x = DEFAULT_SIZE;
    uint16_t zero_y = 0;

    Matrix(uint16_t size_y = DEFAULT_SIZE, uint16_t size_x = DEFAULT_SIZE);

    void increaseSizeY(const uint16_t shift, const uint16_t expansion = EXPANSION_DEFAULT_SIZE);
    void increaseSizeX(const uint16_t shift, const uint16_t expansion = EXPANSION_DEFAULT_SIZE);

    Vector& operator[](uint16_t index);
    Matrix& operator=(const Matrix& other);
};

struct Vector {
    Matrix* matrix = nullptr;
    uint64_t* data = nullptr;
    uint16_t size = DEFAULT_SIZE;
    uint16_t zero_x = 0;

    Vector(Matrix* matrix, const uint16_t size = DEFAULT_SIZE);
    Vector() = default;

    void increaseSize(const uint16_t shift, const uint16_t expansion = EXPANSION_DEFAULT_SIZE);
    uint64_t& operator[](uint16_t index);
};



struct RingBuffer {
    Coords* buffer = nullptr;
    uint64_t head = 0;
    uint64_t tail = 0;
    uint32_t size = DEFAULT_SIZE;

    RingBuffer(uint64_t size = DEFAULT_SIZE);
    ~RingBuffer();

    bool isEmpty();
    bool isFull();
    uint64_t len();
    void push(const Coords& value);
    Coords pop();
};