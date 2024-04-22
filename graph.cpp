#include "graph.h"

Graph::Graph(unsigned int n) {
  m_incidence_array.reserve(n-1);
  for (size_t i = 0; i < n-1; ++i) {
    m_incidence_array[i].assign(i+1, 0);
  }
}

int Graph::get_weight(unsigned int u, unsigned int v) const {
  if (std::max(u, v) > m_incidence_array.size()) {
    throw std::invalid_argument("Exceeded graph\'s vertex count.");
  }

  if (u == v) {
    return Graph::inf;
  }

  return m_incidence_array[std::max(u, v)-1][std::min(u, v)];
}

FastSet Graph::get_neighbours(unsigned int v) const {
  if (v > m_incidence_array.size()) {
    throw std::invalid_argument("Exceeded graph\'s vertex count.");
  }

  FastSet neighbours(get_rank());
  for (size_t i = 0; i < m_incidence_array[v].size(); ++i){
    if (m_incidence_array[v][i] != Graph::inf) {
      neighbours.set(i, true);
    }
  }

  return neighbours;
}

FastSet Graph::get_vertices() const {
  FastSet vertices(get_rank());
  return ~vertices;
}

unsigned int Graph::get_rank() const {
  return m_incidence_array.empty() ? 0 : m_incidence_array.size()+1;
}

Graph Graph::induce(const std::vector<unsigned int>& vertices) const {
  FastSet set(get_rank(), vertices);
  return induce(set);
}

Graph Graph::induce(const FastSet& vertices) const {
  if (vertices == get_vertices()) {
    return *this;
  }
  if (vertices.get_size() == 0) {
    return Graph();
  }
  Graph induced = *this;
  auto vertices_to_remove = ~vertices;
  for (int i = induced.m_incidence_array.size() - 1; i >= 0; --i) {
    for (auto j = vertices_to_remove.rbegin(); j != vertices_to_remove.rend(); --j) {
      if (*j > i) {
        continue;
      }
      induced.m_incidence_array[i].erase(induced.m_incidence_array[i].begin()+*j);
    }
  }
  for (auto i = vertices_to_remove.rbegin(); i != vertices_to_remove.rend(); --i) {
    if (*i == 0) {
      break;
    }
    induced.m_incidence_array.erase(induced.m_incidence_array.begin()+*i-1);
  }
    if (induced.m_incidence_array.begin()->empty()) {
      induced.m_incidence_array.erase(induced.m_incidence_array.begin());
    }

  return induced;
}

Graph Graph::extend(unsigned int n) const {
  Graph copy = *this;
  auto old_size = copy.m_incidence_array.size();
  copy.m_incidence_array.resize(old_size + n);
  for (size_t i = old_size; i < old_size + n; ++i) {
    copy.m_incidence_array[i]=std::vector<int>(i+1, 0);
  }
  return copy;
}

std::istream& operator>>(std::istream& in, Graph& g) {
  unsigned int n;
  in >> n;
  if (n < 2) {
    throw std::runtime_error("Graph size too small.");
  }

  int tmp;
  g.m_incidence_array.reserve(n-1);
  for (size_t i = 0; i < n-1; ++i) {
    g.m_incidence_array.push_back({});
    g.m_incidence_array[i].reserve(i+1);
    for (size_t j = 0; j < i+1; ++j) {
      in >> tmp;
      g.m_incidence_array[i].push_back(tmp);
    }
  }

  return in;
}

std::ostream& operator<<(std::ostream& out, const Graph& g) {
  for (auto& row : g.m_incidence_array) {
    for (auto& v : row) {
      out << v << " ";
    }
    if (row != g.m_incidence_array.back()) {
      out << std::endl;
    }
  }
  return out;
}

