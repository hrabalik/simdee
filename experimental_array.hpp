#ifndef SIMDIFY_ARRAY
#define SIMDIFY_ARRAY

#include "simdify.hpp"
#include <vector>

namespace simd {

    template <typename T = simd_t>
    class vector : public std::vector<T> { // TODO use own allocator
        using Base = std::vector<T>;
        /*
        using value_type = T;
        //using allocator_type = ...;
        using size_type = std::size_t;
        using difference_type = std::ptrdiff_t;
        using reference = value_type&;
        using const_reference = const value_type&;
        //using pointer = std::allocator_traits<allocator_type>::pointer;
        //using const_pointer = std::allocator_traits<allocator_type>::const_pointer;
        //using iterator = ...;
        //using const_iterator = ...;
        //using reverse_iterator = std::reverse_iterator<iterator>;
        //using const_reverse_iterator = std::reverse_iterator<const_iterator>
        */
    public:
        
        const_reference at(size_type i) const {
            std::cerr << "my at got called!" << std::endl;
            return Base::at(i);
        }

        reference at(size_type i) {
            std::cerr << "my at got called!" << std::endl;
            return Base::at(i);
        }

    };
};

#endif