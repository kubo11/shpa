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
  return Path({Edge{s, v[0], g.get_weight(s, v[0])},
                    Edge{v[0], v[1], g.get_weight(v[0], v[1])},
                    Edge{v[1], f, g.get_weight(v[1], f)}});
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
           a.get_size() != u.get_size() / 2 + 1) ||
          !a.get(*u.begin())) {
        return;
      }
      auto b = u - a;
      auto a_prim = a + std::vector<unsigned int>{s, c};
      auto b_prim = b + std::vector<unsigned int>{c, f};
      std::unordered_map<unsigned int, unsigned int> a_prim_map, a_prim_reverse_map, b_prim_map, b_prim_reverse_map;
      unsigned int a_new = 0, b_new = 0;
      for (unsigned int a_old : a_prim) {
        a_prim_reverse_map[a_new] = a_old;
        a_prim_map[a_old] = a_new++;
      }
      for (unsigned int b_old : b_prim) {
        b_prim_reverse_map[b_new] =  b_old;
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
        auto p = p1 + p2;
        if (minP.get_length() > p.get_length()) {
          minP = p;
        }
      }
    });
  }
  return minP;
}