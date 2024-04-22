#ifndef SHPA_FAST_SET_H
#define SHPA_FAST_SET_H

#include <cstdint>
#include <vector>
#include <iostream>
#include <functional>

class FastSet {
 public:
  class iterator : public std::iterator<std::bidirectional_iterator_tag, unsigned int, unsigned int, const unsigned int*, unsigned int> {
    friend class FastSet;
   public:
    iterator(const FastSet& set, int pos);
    ~iterator() = default;
    iterator& operator++();
    iterator operator++(int);
    iterator& operator--();
    iterator operator--(int);
    bool operator==(const iterator& other) const;
    bool operator!=(const iterator& other) const;
    const reference operator*() const;
   private:
    int m_pos;
    const FastSet& m_set;
  };

  FastSet();
  FastSet(unsigned int n);
  FastSet(unsigned int n, const std::vector<unsigned int>& v);
  ~FastSet() = default;

  unsigned int get_rank() const;
  unsigned int get_size() const;
  bool get(unsigned int n) const;
  void set(unsigned int n, bool v);
  void enumerate_subsets(const std::function<void(const FastSet&)>) const;

  const iterator begin() const;
  const iterator end() const;
  const iterator rbegin() const;
  const iterator rend() const;

  FastSet operator~() const;
  FastSet operator&(const FastSet& other) const;
  FastSet operator|(const FastSet& other) const;
  FastSet operator+(int v) const;
  FastSet operator+(const std::vector<unsigned int>& v) const;
  FastSet operator+(const FastSet& v) const;
  FastSet operator-(int v) const;
  FastSet operator-(const std::vector<unsigned int>& v) const;
  FastSet operator-(const FastSet& v) const;
  bool operator==(const FastSet& other) const;
  friend std::ostream& operator<<(std::ostream& out, const FastSet& s);

 private:
  static constexpr unsigned int chunk_size = 32;
  std::vector<uint32_t> m_bits;
  unsigned int m_rank;
  unsigned int m_size;
};

#endif  // SHPA_FAST_SET_H
