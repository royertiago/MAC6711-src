#ifndef XORSHIFT_HPP
#define XORSHIFT_HPP

/* Xorshift random number generator.
 * This class is a direct implementation of the version with period 2**128-1
 * found it the paper http://www.jstatsoft.org/v08/i14/paper.
 *
 * The values a, b and c should be chosen in a special way;
 * the paper suggest the triples [5, 14, 1], [15, 4, 21], [23, 24, 3], [5, 12, 29].
 *
 * For convenience, a typedef to xorshift using the second triple is given.
 */

#include <cstdint>
#include <chrono>

template < std::uint32_t a, std::uint32_t b, std::uint32_t c >
struct xorshift_t {
    // State bytes
    std::uint32_t x, y, z, w;

    /* Default constructor.
     * Initialize every state bytes based on the current time.
     */
    xorshift_t();

    /* Constructor that allows the initial seed specification.
     */
    xorshift_t( std::uint32_t x, std::uint32_t y,
                std::uint32_t z, std::uint32_t w )
    :
        x(x), y(y), z(z), w(w)
    {}

    /* Generates a new random number
     * and advances the generator's internal state.
     */
    std::uint32_t operator()();
};

// Convenience typedef
using xorshift = xorshift_t<15, 4, 21>;


template < std::uint32_t a, std::uint32_t b, std::uint32_t c >
xorshift_t< a, b, c >::xorshift_t() {
    // seed is a 64-bit number.
    auto seed = std::chrono::system_clock::now().time_since_epoch().count();
    x = z = seed & ((1llu << 32) - 1);
    y = w = seed >> 32;
}

template < std::uint32_t a, std::uint32_t b, std::uint32_t c >
std::uint32_t xorshift_t< a, b, c >::operator()() {
    std::uint32_t t = x ^ (x << a);
    x = y; y = z; z = w;
    return w = (w ^ (w >> c)) ^ (t ^ (t >> b));
}

#endif // XORSHIFT_HPP
