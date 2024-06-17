#include "shpa.h"
#include <unordered_map>

Path shpa(const Graph& g) {
  auto g_prim = g.extend(2);
  unsigned int s = g_prim.get_rank() - 2, f = g_prim.get_rank() - 1;
  g_prim.set_weight(s, f, Graph::inf);
  auto p = hpa3(g_prim, s, f);
  p.trim_back(1);
  p.trim_front(1);
  return p;
}

Path hpa3_bf(const Graph& g, unsigned int s, unsigned int f) {
  if (g.get_rank() < 2 || g.get_rank() > 4) {
    return Path();
  }
  if (g.get_rank() == 2) {
    return Path({Edge{s, f, g.get_weight(s, f)}});
  }
  if (g.get_rank() == 3) {
    unsigned int v =
        *(g.get_vertices() - std::vector<unsigned int>{s, f}).begin();
    return Path({Edge{s, v, g.get_weight(s, v)}, Edge{v, f, g.get_weight(v, f)}});
  }
  unsigned int v[2] = {0}, i = 0;
  for (auto vv : (g.get_vertices() - std::vector<unsigned int>{s, f})) {
    v[i++] = vv;
  }
  auto p1 = Path({Edge{s, v[0], g.get_weight(s, v[0])},
                  Edge{v[0], v[1], g.get_weight(v[0], v[1])},
                  Edge{v[1], f, g.get_weight(v[1], f)}});
  auto p2 = Path({Edge{s, v[1], g.get_weight(s, v[1])},
                  Edge{v[1], v[0], g.get_weight(v[1], v[0])},
                  Edge{v[0], f, g.get_weight(v[0], f)}});
  return p1.get_length() < p2.get_length() ? p1 : p2;
}

Path hpa3(const Graph& g, unsigned int s, unsigned int f) {
  Path minP;
  if (g.get_rank() <= 4) {
    return hpa3_bf(g, s, f);
  }
  auto v_prim = g.get_vertices() - std::vector<unsigned int>{s, f};
  for (auto c : v_prim) {
    auto u = v_prim - c;
    u.enumerate_subsets([&u, &g, &minP, s, f, c](const auto& a){
      if ((a.get_size() != u.get_size() / 2 &&
           (a.get_size() != u.get_size() / 2 + 1 || u.get_size() <= 2)) ||
          !a.get(*u.begin())) {
        return;
      }
      auto b = u - a;
      auto p1 = process_subgraphs(g, a + std::vector<unsigned int>{s, c},
                                  b + std::vector<unsigned int>{c, f}, s, c, f);
      auto p2 = process_subgraphs(g, b + std::vector<unsigned int>{s, c},
                                  a + std::vector<unsigned int>{c, f}, s, c, f);
      if (!p1.empty() && minP.get_length() > p1.get_length()) {
        minP = p1;
      }
      if (!p2.empty() && minP.get_length() > p2.get_length()) {
        minP = p2;
      }
    });
  }
  return minP;
}

Path process_subgraphs(const Graph& g, const FastSet& a_prim,
    const FastSet& b_prim, unsigned int s, unsigned int c,
    unsigned int f) {
  std::unordered_map<unsigned int, unsigned int> a_prim_map, a_prim_reverse_map,
      b_prim_map, b_prim_reverse_map;
  unsigned int a_new = 0, b_new = 0;
  for (unsigned int a_old : a_prim) {
    a_prim_reverse_map[a_new] = a_old;
    a_prim_map[a_old] = a_new++;
  }
  for (unsigned int b_old : b_prim) {
    b_prim_reverse_map[b_new] = b_old;
    b_prim_map[b_old] = b_new++;
  }
  auto p1 = hpa3(g.induce(a_prim), a_prim_map.at(s), a_prim_map.at(c));
  auto p2 = hpa3(g.induce(b_prim), b_prim_map.at(c), b_prim_map.at(f));
  if (!p1.empty() && !p2.empty()) {
    for (auto& e : p1.get_edges()) {
      e.beg = a_prim_reverse_map.at(e.beg);
      e.end = a_prim_reverse_map.at(e.end);
    }
    for (auto& e : p2.get_edges()) {
      e.beg = b_prim_reverse_map.at(e.beg);
      e.end = b_prim_reverse_map.at(e.end);
    }
    return p1 + p2;
  }
  return Path();
}