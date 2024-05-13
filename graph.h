#ifndef SHPA_GRAPH_H
#define SHPA_GRAPH_H

#include <vector>
#include <fstream>
#include <bitset>

#include "fast_set.h"

class Graph {
 public:
  static constexpr int inf = INT32_MAX;

  Graph() = default;
  Graph(unsigned int n);
  ~Graph() = default;

  int get_weight(unsigned int u, unsigned int v) const;
  void set_weight(unsigned int u, unsigned int v, int w);
  FastSet get_neighbours(unsigned int v) const;
  FastSet get_vertices() const;
  unsigned int get_rank() const;
  Graph induce(const std::vector<unsigned int>& vertices) const;
  Graph induce(const FastSet& vertices) const;
  Graph extend(unsigned int n) const;

  friend std::istream& operator>>(std::istream& in, Graph& g);
  friend std::ostream& operator<<(std::ostream& out, const Graph& g);


 private:
  std::vector<std::vector<int>> m_incidence_array;
};


#endif //SHPA_GRAPH_H
