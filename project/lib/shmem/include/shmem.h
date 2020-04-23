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
    size_t count;
};

class SharedMem {
 public:
    explicit SharedMem(size_t size) : _size(size){
        _mmap = ::mmap(nullptr, _size,
                       PROT_WRITE | PROT_READ,
                       MAP_SHARED | MAP_ANONYMOUS, -1, 0);
        if (_mmap == MAP_FAILED) {
            throw Exception("Error creaing mmap");
        }

        // initialize state
        _state = static_cast<AllocState *>(_mmap);
        _state->start = static_cast<char *>(_mmap) + sizeof(*_state);
        _state->end = static_cast<char *>(_mmap) + _size;
        _state->count = 0;

        // initialize semaphore
        _semaphore = reinterpret_cast<sem_t *>(_state->start);
        ::sem_init(_semaphore, 1, 1);
        _state->start += sizeof(*_semaphore);
    }

    ~SharedMem() {
        ::munmap(_mmap, _size);
    }

    [[nodiscard]] void *ptr() const {
        return _mmap;
    }

    [[nodiscard]] sem_t *semaphore() const {
        return _semaphore;
    }

    [[nodiscard]] AllocState *state() const {
        return _state;
    }

 private:
    void *_mmap;
    AllocState *_state;
    sem_t *_semaphore;
    size_t _size;
};

class Lock {
 public:
    explicit Lock(SharedMem &sh_mem) : _sem(*sh_mem.semaphore()) {
        ::sem_wait(&_sem);
    }

    ~Lock() {
        ::sem_post(&_sem);
    }
 private:
    sem_t& _sem;
};

template <class T>
class ShAlloc {
 public:
    using value_type = T;

    ShAlloc() = default;

    explicit ShAlloc(AllocState* state) : _state(state) {}

    template <class U>
    explicit ShAlloc(const ShAlloc<U> & other) : _state(other.state()) {}

    T *allocate(size_t n) {
        char *result = _state->start;
        if (result + n * sizeof(T)> _state->end) {
            throw std::bad_alloc();
        }
        _state->start += n * sizeof(T);
        return reinterpret_cast<T *>(result);
    }

    void deallocate(T *ptr, size_t n) {
        if (_state->start - n * sizeof(T) == reinterpret_cast<char *>(ptr)) {
            _state->start -= n * sizeof(T);
        }
    }

    [[nodiscard]] AllocState *state() const {
        return _state;
    }

 private:
    AllocState *_state = nullptr;
};

template <class K, class V, class Compare = std::less<K>>
class map {
    using PairAlloc = ShAlloc<std::pair<const K, V>>;
    using Container = std::map<K, V, Compare, PairAlloc>;
 public:
    explicit map(size_t n) : _memory(n) {
        _container = reinterpret_cast<Container *>(_memory.state()->start);
        _memory.state()->start += sizeof(Container);
        _container = new(_container) Container(PairAlloc(_memory.state()));
    }
    Lock &&lock() {
        return std::move(Lock(_memory));
    }
    V &operator[](K && key) {
        return _container->operator[](std::forward<K>(key));
    }
    void erase(K && key) {
        _container->erase(key);
    }
    auto begin() {
        return _container->begin();
    }
    auto end() {
        return _container->end();
    }
    auto cbegin() {
        return _container->cbegin();
    }
    auto cend() {
        return _container->cend();
    }
    auto rbegin() {
        return _container->rbegin();
    }
    auto rend() {
        return _container->rend();
    }
    auto rcbegin() {
        return _container->rcbegin();
    }
    auto rcend() {
        return _container->rcend();
    }

 private:
    Container *_container;
    SharedMem _memory;
};

using string = std::basic_string<char, std::char_traits<char>, ShAlloc<char>>;
}  // namespace shmem

#endif //PROJECT_LIB_SHMEM_INCLUDE_SHMEM_H_
