#ifndef ARRAY_H
#define ARRAY_H

#include <cstdint>

template <typename T>
class Array {
protected:
    T* data;
    uint32_t length;

public:
    Array() : data(new T[0]), length(0) {}
    Array(uint32_t length) : data(new T[length]), length(length) {}
    Array(const Array& other) : data(new T[other.length]), length(other.length) {
        for (uint32_t i = 0; i < length; i++) data[i] = other.data[i];
    }
    ~Array() { delete[] data; }

    T& operator[](uint32_t index) { return data[index]; }
    const T operator[](uint32_t index) const { return data[index]; }
    Array<T>& operator=(const Array<T>& other) {
        if (this != &other) {
            delete[] data;
            data = new T[other.length];
            length = other.length;
            for (uint32_t i = 0; i < length; i++) data[i] = other.data[i];
            // memcpy(data, other.data, length * sizeof(T));
        }
        return *this;
    }
    uint32_t getLength() const { return length; }
};

template <typename T>
class Array2D : Array<Array<T>> {
public:
    Array2D(uint32_t length, uint32_t innerLength) {
        this->data = new Array<T>[length];
        this->length = length;
        for (uint32_t i = 0; i < length; i++) this->data[i] = Array<T>(innerLength);
    }
    Array2D(uint32_t length) : Array2D(length, 0) {}
    Array2D() : Array<Array<T>>() {}
    Array2D(const Array2D& other) : Array<Array<T>>(other) {}
    ~Array2D() {}

    using Array<Array<T>>::getLength;
    using Array<Array<T>>::operator[];
    // Array<T>& operator[](uint32_t index) { return this->data[index]; }
    // const Array<T> operator[](uint32_t index) const { return this->data[index]; }
    Array2D<T>& operator=(const Array2D<T>& other) {
        if (this != &other) {
            delete[] this->data;
            this->data = new Array<T>[other.length];
            this->length = other.length;
            for (uint32_t i = 0; i < this->length; i++) this->data[i] = other.data[i];
        }
        return *this;
    }
};

/*
template <typename T>
class Array2D {
private:
    T** data;
    uint32_t length, innerLength;
public:
    Array2D(uint32_t length, uint32_t innerLength) {
        this->data = new T*[length];
        this->length = length;
        this->innerLength = innerLength;
        for (uint32_t i = 0; i < length; i++) this->data[i] = new T[innerLength];
    }
    Array2D(uint32_t length) { Array2D(length, 0); }
    Array2D() { Array2D(0, 0); }
    Array2D(const Array2D& other) {
        this->data = new T*[other.length];
        this->length = other.length;
        this->innerLength = other.innerLength;
        for (uint32_t i = 0; i < length; i++) {
            this->data[i] = new T[innerLength];
            for (uint32_t j = 0; j < innerLength; j++)
                this->data[i][j] = other.data[i][j];
        }
    }
    ~Array2D() { delete[] this->data; }

    uint32_t getLength() const { return this->length; }
    uint32_t getInnerLength() const { return this->innerLength; }
    T*& operator[](uint32_t index) { return this->data[index]; }
    const T* operator[](uint32_t index) const { return this->data[index]; }
    Array2D<T>& operator=(const Array2D<T>& other) {
        if (this != &other) {
            delete[] this->data;
            this->data = new T*[other.length];
            this->length = other.length;
            this->innerLength = other.innerLength;
            for (uint32_t i = 0; i < this->length; i++) {
                this->data[i] = new T[innerLength];
                for (uint32_t j = 0; j < this->innerLength; j++)
                    this->data[i][j] = other.data[i][j];
            }
        }
        return *this;
    }
};*/

#endif