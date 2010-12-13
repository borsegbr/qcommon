#ifndef _allocators_H
#define _allocators_H

#include "../all/predef.h"

#include <cstdlib>
#include <exception>


_QTL_BEGIN


template <class T> 
class malloc_allocator
{
public:
    typedef T value_type;
    typedef value_type* pointer;
    typedef const value_type* const_pointer;
    typedef value_type& reference;
    typedef const value_type& const_reference;
    typedef std::size_t size_type;
    typedef std::ptrdiff_t difference_type;

    template <class U>
    struct rebind 
    {
        typedef malloc_allocator<U> other;
    };

public:
    pointer address(reference x) const 
    { 
        return &x; 
    }
    const_pointer address(const_reference x) const 
    { 
        return &x; 
    }

    pointer allocate(size_type n, const_pointer = 0) 
    {
        void* p = std::malloc(n * sizeof(T));
        if (!p)
            throw std::bad_alloc();
        return static_cast<pointer>(p);
    }

    void deallocate(pointer p, size_type = 0) 
    {
        std::free(p);
    }

    size_type max_size() const 
    { 
        return static_cast<size_type>(-1) / sizeof(value_type);
    }

    void construct(pointer p, const value_type& x) 
    { 
        new(p) value_type(x);
    }

    void destroy(pointer p) 
    { 
        p->~value_type(); 
    }

};  /* class malloc_allocator */


_QTL_END

#endif  /* ifndef _allcoators_H */

