#include "fast_set.h"

#include <algorithm>
#include <bit>

FastSet::iterator::iterator(const FastSet& set, int pos) : m_set(set), m_pos(pos) {
  if (m_pos == 0) {
    while(m_pos < m_set.get_rank() && !m_set.get(m_pos)) {
      m_pos++;
    }
  }
  else if (m_pos == m_set.get_rank()-1) {
    while (m_pos > -1 && !m_set.get(m_pos)) {
      m_pos--;
    }
  }
}

FastSet::iterator& FastSet::iterator::operator++() {
  if (m_pos == m_set.get_rank()) {
    return *this;
  }
  m_pos++;
  while (m_pos < m_set.get_rank() && !m_set.get(m_pos)) {
    m_pos++;
  }

  return *this;
}

FastSet::iterator FastSet::iterator::operator++(int) {
  FastSet::iterator retval = *this;
  ++(*this);
  return retval;
}

FastSet::iterator& FastSet::iterator::operator--() {
  if (m_pos == -1) {
    return *this;
  }
  m_pos--;
  while (m_pos > -1 && !m_set.get(m_pos)) {
    m_pos--;
  }

  return *this;
}

FastSet::iterator FastSet::iterator::operator--(int) {
  FastSet::iterator retval = *this;
  --(*this);
  return retval;
}

bool FastSet::iterator::operator==(const FastSet::iterator& other) const {
  return m_pos == other.m_pos && m_set == other.m_set;
}

bool FastSet::iterator::operator!=(const FastSet::iterator& other) const {
  return !(*this == other);
}

const FastSet::iterator::reference FastSet::iterator::operator*() const {
  if (m_pos > -1 && m_pos < m_set.get_rank()) {
    return m_pos;
  }

  throw std::runtime_error("Invalid iterator access");
}

FastSet::FastSet() : m_rank(0), m_size(0), m_bits() {}

FastSet::FastSet(unsigned int n) : m_rank(n), m_size(0), m_bits((n - 1) / FastSet::chunk_size + 1, 0) {}

FastSet::FastSet(unsigned int n, const std::vector<unsigned int>& v) : FastSet(n) {
  for (auto& vv : v) {
    set(vv, true);
  }
}

unsigned int FastSet::get_rank() const {
  return m_rank;
}

unsigned int FastSet::get_size() const {
  return m_size;
}

bool FastSet::get(unsigned int n) const {
  if (n > get_rank()) {
    throw std::invalid_argument("Exeeded set bounds.");
  }

  return m_bits[n / FastSet::chunk_size] & (0x01 << (FastSet::chunk_size - 1 - n % FastSet::chunk_size));
}

void FastSet::set(unsigned int n, bool v) {
  if (n > get_rank()) {
    throw std::invalid_argument("Exeeded set bounds.");
  }

  if (v) {
    if (!get(n)) {
      m_bits[n / FastSet::chunk_size] |= (0x01 << (FastSet::chunk_size - 1 - n % FastSet::chunk_size));
      m_size++;
    }
  }
  else {
    if (get(n)) {
      m_bits[n / FastSet::chunk_size] = ~(~m_bits[n / FastSet::chunk_size] | (0x01 << (FastSet::chunk_size - 1 - n % FastSet::chunk_size)));
      m_size--;
    }
  }
}

void FastSet::enumerate_subsets(const std::function<void(const FastSet&)> func) const {
  if (m_size == 0) {
    return;
  }
  func(*this);
  for (const auto it : *this) {
    (*this - it).enumerate_subsets(func);
  }
}

const FastSet::iterator FastSet::begin() const {
  return {*this, 0};
}

const FastSet::iterator FastSet::end() const {
  return {*this, static_cast<int>(get_rank())};
}

const FastSet::iterator FastSet::rbegin() const {
  return {*this, static_cast<int>(get_rank()-1)};
}

const FastSet::iterator FastSet::rend() const {
  return {*this, -1};
}

FastSet FastSet::operator~() const {
  FastSet set = *this;
  for (auto& chunk : set.m_bits) {
    chunk = ~chunk;
  }
  set.m_size = set.m_rank - set.m_size;
  set.m_bits.back() &= (~(0xffffffff >> (set.get_rank() % FastSet::chunk_size)));

  return set;
}

FastSet FastSet::operator&(const FastSet& other) const {
  FastSet set(std::min(get_rank(), other.get_rank()));
  for (int i = 0; i < std::min(get_rank(), other.get_rank()); ++i) {
    set.m_bits[i] = m_bits[i] & other.m_bits[i];
    set.m_size += std::popcount(set.m_bits[i]);
  }
  return set;
}

FastSet FastSet::operator|(const FastSet& other) const {
  FastSet set(std::max(get_rank(), other.get_rank()));
  for (int i = 0; i < std::max(get_rank(), other.get_rank()); ++i) {
    if (i < m_bits.size() && i < other.m_bits.size()) {
      set.m_bits[i] = m_bits[i] | other.m_bits[i];
    }
    else if (i >= other.m_bits.size()) {
      set.m_bits[i] = m_bits[i];
    }
    else {
      set.m_bits[i] = other.m_bits[i];
    }

    set.m_size += std::popcount(set.m_bits[i]);
  }
  return set;
}

FastSet FastSet::operator+(int v) const {
  FastSet set = *this;
  if (v >= set.get_rank()) {
    set.m_bits.resize((v - 1) / FastSet::chunk_size + 1);
    set.m_rank = v+1;
  }

  set.set(v, true);
  return set;
}

FastSet FastSet::operator+(const std::vector<unsigned int>& v) const {
  auto copy = *this;
  auto v_copy = v;
  std::sort(v_copy.begin(), v_copy.end());
  if (v_copy.back() >= copy.get_rank()) {
    copy.m_bits.resize((v_copy.back() - 1) / FastSet::chunk_size + 1);
    copy.m_rank = v_copy.back() + 1;
  }

  for (const auto val : v_copy) {
    copy.set(val, true);
  }
  return copy;
}

FastSet FastSet::operator+(const FastSet& v) const {
  return *this | v;
}

FastSet FastSet::operator-(int v) const {
  FastSet set = *this;
  if (v < get_rank()) {
    set.set(v, false);
  }
  return set;
}

FastSet FastSet::operator-(const std::vector<unsigned int>& v) const {
  FastSet set = *this;
  for (const auto vv : v) {
    if (vv < get_rank()) {
      set.set(vv, false);
    }
  }
  return set;
}

FastSet FastSet::operator-(const FastSet& v) const {
  return *this & ~v;
}

bool FastSet::operator==(const FastSet& other) const {
  return m_rank == other.m_rank && m_size == other.m_size && m_bits == other.m_bits;
}

std::ostream& operator<<(std::ostream& out, const FastSet& s) {
  for (const auto v : s) {
    out << v << " ";
  }
  return out;
}