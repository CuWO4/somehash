#ifndef UINT256_H__
#define UINT256_H__

#include <cstdint>

class uint256_t {
public:
  uint256_t() : hi(0), lo(0) {}
  uint256_t(__uint128_t hi, __uint128_t lo) : hi(hi), lo(lo) {}
  uint256_t(uint64_t u64_3, uint64_t u64_2, uint64_t u64_1, uint64_t u64_0) 
    : hi((__uint128_t) u64_3 << 64 | u64_2), lo((__uint128_t) u64_1 << 64 | u64_0) {}
  uint256_t(int8_t x) : hi(x >= 0 ? 0 : -1), lo(x) {}
  uint256_t(uint8_t x) : hi(0), lo(x) {}
  uint256_t(int16_t x) : hi(x >= 0 ? 0 : -1), lo(x) {}
  uint256_t(uint16_t x) : hi(0), lo(x) {}
  uint256_t(int32_t x) : hi(x >= 0 ? 0 : -1), lo(x) {}
  uint256_t(uint32_t x) : hi(0), lo(x) {}
  uint256_t(int64_t x) : hi(x >= 0 ? 0 : -1), lo(x) {}
  uint256_t(uint64_t x) : hi(0), lo(x) {}
  uint256_t(__int128_t x) : hi(x >= 0 ? 0 : -1), lo(x) {}
  uint256_t(__uint128_t x) : hi(0), lo(x) {}

  __uint128_t& hi_u128() { return hi; }
  __uint128_t& lo_u128() { return lo; }
  uint64_t u64_0() const { return lo & u64_mask; }
  uint64_t u64_1() const { return (lo >> 64) & u64_mask; }
  uint64_t u64_2() const { return hi & u64_mask; }
  uint64_t u64_3() const { return (hi >> 64) & u64_mask; }

  uint256_t operator+() const { return *this; }
  uint256_t& operator+=(const uint256_t y) {
    uint64_t carry;
    lo = add_carry(lo, y.lo, carry);
    hi = hi + y.hi + carry;
    return *this;
  }
  friend uint256_t operator+(const uint256_t x, const uint256_t y) {
    uint256_t res = x;
    res += y;
    return res;
  }

  uint256_t operator-() const { return ~*this + 1; }
  uint256_t& operator-=(const uint256_t y) {
    uint256_t::operator+=(-y);
    return *this;
  }
  friend uint256_t operator-(const uint256_t x, const uint256_t y) {
    uint256_t res = x;
    res -= y;
    return res;
  }

  friend uint256_t operator*(const uint256_t x, const uint256_t y) {
    uint256_t result;

    // x.lo * y.lo
    __uint128_t p0_lo, p0_hi;
    mul128(x.lo, y.lo, p0_lo, p0_hi);
    // x.lo * y.hi
    __uint128_t p1_lo, p1_hi;
    mul128(x.lo, y.hi, p1_lo, p1_hi);
    // x.hi * y.lo
    __uint128_t p2_lo, p2_hi;
    mul128(x.hi, y.lo, p2_lo, p2_hi);
    result.lo = p0_lo;
    result.hi = p0_hi + p1_lo + p2_lo;

    return result;
  }
  uint256_t& operator*=(const uint256_t y) {
    return *this = *this * y;
  }

  uint256_t& operator^=(const uint256_t y) {
    lo ^= y.lo;
    hi ^= y.hi;
    return *this;
  }
  friend uint256_t operator^(const uint256_t x, const uint256_t y) {
    uint256_t res = x;
    res ^= y;
    return res;
  }

  uint256_t& operator&=(const uint256_t y) {
    lo &= y.lo;
    hi &= y.hi;
    return *this;
  }
  friend uint256_t operator&(const uint256_t x, const uint256_t y) {
    uint256_t res = x;
    res &= y;
    return res;
  }

  uint256_t& operator|=(const uint256_t y) {
    lo |= y.lo;
    hi |= y.hi;
    return *this;
  }
  friend uint256_t operator|(const uint256_t x, const uint256_t y) {
    uint256_t res = x;
    res |= y;
    return res;
  }

  uint256_t& operator<<=(const uint256_t y) {
    if (y >= 256) return *this = 0;
    uint64_t i = y.u64_0();
    if (i < 128) {
      (hi <<= i) |= (lo >> (128 - i));
      lo <<= i;
    } else {
      hi = lo << (i - 128);
      lo = 0;
    }
    return *this;
  }
  friend uint256_t operator<<(const uint256_t x, const uint256_t y) {
    uint256_t res = x;
    res <<= y;
    return res;
  }

  uint256_t& operator>>=(const uint256_t y) {
    if (y >= 256) return *this = 0;
    uint64_t i = y.u64_0();
    if (i < 128) {
      (lo >>= i) |= (hi << (128 - i));
      hi >>= i;
    } else {
      lo = hi >> (i - 128);
      hi = 0;
    }
    return *this;
  }
  friend uint256_t operator>>(const uint256_t x, const uint256_t y) {
    uint256_t res = x;
    res >>= y;
    return res;
  }

  uint256_t operator~() const {
    return uint256_t(~this->hi, ~this->lo);
  }

  bool operator!() const {
    return !this->hi && !this->lo;
  }

  friend bool operator<(const uint256_t x, const uint256_t y) {
    return x.hi < y.hi || (x.hi == y.hi && x.lo < y.lo);
  }
  friend bool operator>(const uint256_t x, const uint256_t y) {
    return x.hi > y.hi || (x.hi == y.hi && x.lo > y.lo);
  }
  friend bool operator==(const uint256_t x, const uint256_t y) {
    return x.hi == y.hi && x.lo == y.lo;
  }
  friend bool operator!=(const uint256_t x, const uint256_t y) {
    return !(x == y);
  }
  friend bool operator<=(const uint256_t x, const uint256_t y) {
    return x < y || x == y;
  }
  friend bool operator>=(const uint256_t x, const uint256_t y) {
    return x > y || x == y;
  }

  friend bool operator&&(const uint256_t x, const uint256_t y) {
    return x != 0 && y != 0;
  }
  friend bool operator||(const uint256_t x, const uint256_t y) {
    return x != 0 || y != 0;
  }

  uint256_t& operator++() {
    *this += 1;
    return *this;
  }
  uint256_t operator++(int) {
    uint256_t old = *this;
    *this += 1;
    return old;
  }
  uint256_t& operator--() {
    *this -= 1;
    return *this;
  }
  uint256_t operator--(int) {
    uint256_t old = *this;
    *this -= 1;
    return old;
  }

  private:
  __uint128_t hi, lo; 

  static constexpr uint64_t u64_mask = 0xFFFFFFFFFFFFFFFFull;

  static __uint128_t add_carry(__uint128_t x, __uint128_t y, uint64_t& carry) {
    __uint128_t sum = x + y;
    carry = sum < x;
    return sum;
  }

  static void mul128(__uint128_t x, __uint128_t y, __uint128_t& lo, __uint128_t& hi) {
    __uint128_t a_lo = x & u64_mask;
    __uint128_t a_hi = x >> 64;
    __uint128_t b_lo = y & u64_mask;
    __uint128_t b_hi = y >> 64;

    __uint128_t p0 = a_lo * b_lo;
    __uint128_t p1 = a_lo * b_hi;
    __uint128_t p2 = a_hi * b_lo;
    __uint128_t p3 = a_hi * b_hi;

    __uint128_t carry = (p0 >> 64) + (p1 & u64_mask) + (p2 & u64_mask);
    lo = (p0 & u64_mask) | (carry << 64);
    hi = (p1 >> 64) + (p2 >> 64) + p3 + (carry >> 64);
  }
};

#endif // !UINT256_H__
