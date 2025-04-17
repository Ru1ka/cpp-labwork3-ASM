#include "structures.h"

#include <cassert>
#include <iostream>
#include <stdexcept>

Vector::Vector(Matrix* matrix, const uint16_t size) : matrix(matrix), size(size) { data = new uint64_t[size](); }

void Vector::increaseSize(const uint16_t shift, const uint16_t expansion) {
    uint16_t new_size = size + expansion;
    uint64_t* new_data = new uint64_t[new_size]();
    for (uint16_t i = 0; i < size; ++i) {
        new_data[shift + i] = data[i];
    }

    delete[] data;
    size = new_size;
    data = new_data;
}

uint64_t& Vector::operator[](uint16_t index) {
    while (index >= size) {
        (*matrix).increaseSizeX(0);
    }
    return data[index];
}

Matrix::Matrix(uint16_t size_y, uint16_t size_x) : size_y(size_y), size_x(size_x) {
    data = new Vector[size_y];
    for (uint16_t i = 0; i < size_y; ++i) {
        data[i] = Vector(this, size_x);
    }
}

void Matrix::increaseSizeY(const uint16_t shift, const uint16_t expansion) {
    uint16_t new_size = size_y + expansion;
    Vector* new_data = new Vector[new_size];
    for (uint16_t i = 0; i < new_size; ++i) {
        if (i >= shift && i < shift + size_y) {
            new_data[i] = data[i - shift];
        } else {
            new_data[i] = Vector(this, size_x);
        }
    }

    delete[] data;
    size_y = new_size;
    data = new_data;
}

void Matrix::increaseSizeX(const uint16_t shift, const uint16_t expansion) {
    for (uint16_t i = 0; i < size_y; ++i) {
        data[i].increaseSize(shift, expansion);
    }
    size_x += expansion;
}

Vector& Matrix::operator[](uint16_t index) {
    while (index >= size_y) {
        Matrix::increaseSizeY(0);
    }
    Vector& result = data[index];
    return data[index];
}

Coords::Coords(uint16_t y, uint16_t x) : y(y), x(x) {};

RingBuffer::RingBuffer(uint64_t size) : head(0), tail(0), size(size) { buffer = new Coords[size]; }

RingBuffer::~RingBuffer() { delete[] buffer; }

bool RingBuffer::isEmpty() { return head == tail; }

bool RingBuffer::isFull() { return ((tail + 1) % size) == head; }

uint64_t RingBuffer::len() {
    if (tail >= head) {
        return tail - head;
    } else {
        return (size - head) + tail;
    }
}

void RingBuffer::push(const Coords& value) {
    if (isFull()) {
        uint32_t new_size = size * 1.5;
        Coords* new_buffer = new Coords[new_size];

        uint64_t current_count = len();
        for (uint64_t i = 0; i < current_count; ++i) {
            new_buffer[i] = buffer[(head + i) % size];
        }

        delete[] buffer;
        buffer = new_buffer;
        size = new_size;
        head = 0;
        tail = current_count;
    }

    buffer[tail] = value;
    tail = (tail + 1) % size;
}

Coords RingBuffer::pop() {
    Coords value = buffer[head];
    head = (head + 1) % size;
    return value;
}
