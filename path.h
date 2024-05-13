#ifndef SHPA_PATH_H
#define SHPA_PATH_H

#include <vector>
#include <iostream>

struct Edge {
  unsigned int beg;
  unsigned int end;
  int weight;

  bool operator==(const Edge& other) const;
};

class Path {
 public:
  static constexpr int inf = INT32_MAX;
  Path();
  Path(const std::vector<Edge>& edges);
  ~Path() = default;

  bool add_edge(const Edge& e);
  int get_length() const;
  std::vector<Edge>& get_edges();
  bool empty() const;
  void trim_front(unsigned int n);
  void trim_back(unsigned int n);

  Path operator+(const Edge& e) const;
  Path operator+(const Path& p) const;
  bool operator==(const Path& other) const;
  friend std::ostream& operator<<(std::ostream& out, const Path& p);

 private:
  int m_length;
  std::vector<Edge> m_edges;
};

#endif  // SHPA_PATH_H
