//
// Created by kira on 4/23/20.
//

#ifndef PROJECT_LIB_SHMEM_INCLUDE_SHMEM_H_
#define PROJECT_LIB_SHMEM_INCLUDE_SHMEM_H_

#include <functional>
#include <memory>
#include <sys/mman.h>
#include <semaphore.h>
#include <map>

#include "base_exception.h"

using exception::Exception;

namespace shmem {
struct AllocState {
    char *start;
    char *end;
    sem_t *memory_lock;
};

class SharedMem {
    explicit SharedMem(size_t size) : _size(size), _pid_of_creator(getpid()) {
        _mmap = ::mmap(nullptr, _size,
                       PROT_WRITE | PROT_READ,
                       MAP_SHARED | MAP_ANONYMOUS, -1, 0);
        if (_mmap == MAP_FAILED) {
            throw Exception("error creating mmap");
        }

        // initialize state
        _state = static_cast<AllocState *>(_mmap);
        _state->start = static_cast<char *>(_mmap) + sizeof(*_state);
        _state->end = static_cast<char *>(_mmap) + _size;
        if (::sem_init(_state->memory_lock, 1, 1) == -1) {
            throw Exception("error on sem_init");
        }

        // initialize semaphore
        _semaphore = reinterpret_cast<sem_t *>(_state->start);
        if (::sem_init(_semaphore, 1, 1) == -1) {
            throw Exception("error on sem_init");
        }
        _state->start += sizeof(*_semaphore);
    }

    ~SharedMem() {
        if (getpid() == _pid_of_creator) {
            sem_destroy(_semaphore);
        }
        ::munmap(_mmap, _size);
    }

 public:
    [[nodiscard]] sem_t *semaphore() const {
        return _semaphore;
    }

    [[nodiscard]] AllocState *state() const {
        return _state;
    }
    static size_t set_size(size_t size) {
        static size_t mem_size = size;
        return mem_size;
    }
    static SharedMem &get_instance() {
        constexpr size_t DEFAULT_MMAP_SIZE = 1024;
        static SharedMem instance(set_size(DEFAULT_MMAP_SIZE));
        return instance;
    }

 private:
    int _pid_of_creator;
    void *_mmap;
    AllocState *_state;
    sem_t *_semaphore;
    size_t _size;
};


class Lock {
 public:
    Lock() : _sem(SharedMem::get_instance().semaphore()) {
        ::sem_wait(_sem);
    }

    ~Lock() {
        ::sem_post(_sem);
    }

 private:
    sem_t *_sem;
};

template <class T>
class ShAlloc {
 public:
    using value_type = T;

    ShAlloc() {
        _state = SharedMem::get_instance().state();
    }

    template <class U>
    explicit ShAlloc(const ShAlloc<U> &other) : _state(other.state()) {}

    T *allocate(size_t n) {
        Lock _;
        char *result = _state->start;
        if (result + n * sizeof(T)> _state->end) {
            throw std::bad_alloc();
        }
        _state->start += n * sizeof(T);
        return reinterpret_cast<T *>(result);
    }

    void deallocate(T *ptr, size_t n) {
        Lock _;
        if (_state->start - n * sizeof(T) == reinterpret_cast<char *>(ptr)) {
            _state->start -= n * sizeof(T);
        }
    }

    [[nodiscard]] AllocState *state() const {
        return _state;
    }

 private:
    AllocState *_state;
};

template <class K, class V, class Compare = std::less<K>>
class map {
    using PairAlloc = ShAlloc<std::pair<const K, V>>;
    using Container = std::map<K, V, Compare, PairAlloc>;
 public:
    explicit map(size_t n) {
        Lock _;
        SharedMem::set_size(n);
        SharedMem &memory = SharedMem::get_instance();
        _container = reinterpret_cast<Container *>(memory.state()->start);
        memory.state()->start += sizeof(Container);
        _container = new(_container) Container(PairAlloc());
    }
    V operator[](K && key) {
        Lock _;
        return _container->operator[](std::forward<K>(key));
    }
    void erase(K && key) {
        Lock _;
        _container->erase(key);
    }
    auto begin() {  // here for example purposes
        return _container->begin();
    }
    auto end() {
        return _container->end();
    }
 private:
    Container *_container;
};

using string = std::basic_string<char, std::char_traits<char>, ShAlloc<char>>;
}  // namespace shmem

#endif //PROJECT_LIB_SHMEM_INCLUDE_SHMEM_H_
