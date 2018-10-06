#ifndef FIXED_VECTOR_FIXED_VECTOR_H
#define FIXED_VECTOR_FIXED_VECTOR_H

#include <type_traits>
#include <cstdio>

template<typename T, size_t N>
class fixed_vector {

    size_t _capacity;
    size_t _size;
    typename std::aligned_storage<sizeof(T), alignof(T)>::type data[N];

public:

    fixed_vector() : _size(0), _capacity(N) {};

    fixed_vector(fixed_vector const &other) : _size(other._size), _capacity(N) {
        // constructor only for the same size vector
        for (size_t i = 0; i < _size; i++) {
            new(reinterpret_cast<T *>(data + i)) T(other[i]);
        }
    }

    template<size_t M>
    fixed_vector(fixed_vector<T, M> const &other) : _size(other.size()), _capacity(N) {
        if (_size > capacity()) {
            throw std::length_error("assigned capacity reached (copy vector into smaller one)");
        }
        for (size_t i = 0; i < _size; i++) {
            new(reinterpret_cast<T *>(data + i)) T(other[i]);
        }
    }

    fixed_vector &operator=(fixed_vector const &other) {
        for (size_t i = 0; i < _size; i++) {
            reinterpret_cast<const T *>(data + i)->~T();
        }
        for (size_t i = 0; i < other.size(); i++) {
            new(reinterpret_cast<T *>(data) + i)T(other[i]);
        }
        _size = other.size();
        return *this;
    }

    template<size_t M>
    fixed_vector &operator=(fixed_vector<T, M> const &other) {
        if (other.size() > capacity()) {
            throw std::length_error("assigned capacity reached (copy vector into smaller one)");
        }
        for (size_t i = 0; i < _size; i++) {
            reinterpret_cast<const T *>(data + i)->~T();
        }
        for (size_t i = 0; i < other.size(); i++) {
            new(reinterpret_cast<T *>(data) + i)T(other[i]);
        }
        _size = other.size();
        return *this;
    }

    ~fixed_vector() {
        for (size_t i = 0; i < _size; i++) {
            reinterpret_cast<const T *>(data + i)->~T();
        }
    };

    size_t size() const {
        return _size;
    }

    size_t capacity() const {
        return _capacity;
    }

    bool empty() const {
        return _size == 0;
    }

    T &operator[](size_t index) {
        return *reinterpret_cast<T *>(data + index);
    }

    T const &operator[](size_t index) const {
        return *reinterpret_cast<const T *>(data + index);
    }

    void push_back(T const &value) {
        if (_size >= _capacity) {
            throw std::length_error("assigned capacity reached");
        }
        new(reinterpret_cast<T *>(data + _size++)) T(value);
    }

    void pop_back() {
        if (_size != 0) {
            reinterpret_cast<const T *>(data + --_size)->~T();
        }
    }

    T &back() {
        return operator[](_size - 1);
    }

    T const &back() const {
        return operator[](_size - 1);
    }

    T &front() {
        return operator[](0);
    }

    T const &front() const {
        return operator[](0);
    }

    void clear() {
        for (size_t i = 0; i < _size; i++) {
            reinterpret_cast<const T *>(data + i)->~T();
        }
        _size = 0;
    }

    T *begin() {
        return reinterpret_cast<T *>(data);
    }

    T *end() {
        return reinterpret_cast<T *>(data + _size);
    }

    std::reverse_iterator<T *> rbegin() {
        return std::__make_reverse_iterator<T *>(end());
    }

    std::reverse_iterator<T *> rend() {
        return std::__make_reverse_iterator<T *>(begin());
    }

    T const *begin() const {
        return reinterpret_cast<T const *>(data);
    }

    T const *end() const {
        return reinterpret_cast<T const *>(data + _size);
    }

    std::reverse_iterator<T const *> rbegin() const {
        return std::__make_reverse_iterator<T const *>(end());
    }

    std::reverse_iterator<T const *> rend() const {
        return std::__make_reverse_iterator<T const *>(begin());
    }

    T *erase(T const *it) {
        return erase(it, it + 1);
    }

    T *erase(T const *l, T const *r) {
        T *_l = const_cast<T *>(l);
        T *_r = const_cast<T *>(r);
        for (; _r != end(); _l++, _r++) {
            *_l = *_r;
        }
        for (; _l != _r; _l++) {
            reinterpret_cast<const T *>(_l)->~T();
            _size--;
        }
        return const_cast<T *>(l);;
    }

    T *insert(T const *index, T const &value) {
        if (_size >= _capacity) {
            throw std::length_error("assigned capacity reached");
        }
        T *index1 = end();
        for (; index1 != index; index1--) {
            *index1 = *(index1 - 1);
        }
        *const_cast<T *>(index) = value;
        _size++;
        return const_cast<T *>(index);
    }


    void print() {
        std::string s;
        std::cout << size() << " " << capacity() << "\t";
        for (T *it = begin(); it != end(); it++) {
            std::cout << *it << " ";
        }
        std::cout << std::endl;
    }

};

template<typename T, size_t N, size_t M>
void swap(fixed_vector<T, N> &a, fixed_vector<T, M> &b) {
    fixed_vector<T, N> t(a);
    a = b;
    b = t;
}

#endif //FIXED_VECTOR_FIXED_VECTOR_H
