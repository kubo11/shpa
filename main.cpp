#include <iostream>
#include <fstream>

#include "graph.h"

void test(Graph& g, std::ostream& out) {
  out << "G:" << std::endl;
  out << g << std::endl;
  out << "G induced by {1, 2, 3}:" << std::endl;
  out << g.induce({1, 2, 3}) << std::endl;
  out << "G induced by {0, 4}:" << std::endl;
  out << g.induce({0, 4}) << std::endl;
  out << "G extended by 2 vertices:" << std::endl;
  out << g.extend(2) << std::endl;
  out << "Set of G's vertices:" << std::endl;
  out << g.get_vertices() << std::endl;
  out << "Set of G's vertices - {0}:" << std::endl;
  out << g.get_vertices() - 0 << std::endl;
  out << "Set of G's vertices - {1, 2}:" << std::endl;
  out << g.get_vertices() - std::vector<unsigned int>{1, 2} << std::endl;
  out << "Set of G's vertices + {100, 200}:" << std::endl;
  out << g.get_vertices() + std::vector<unsigned int>{100, 200};
  out << "All subsets of {2, 5, 80}:" << std::endl;
  FastSet(100, {2, 5, 80}).enumerate_subsets([&out](const auto& s) { out << s << std::endl; });
}

int main(int argc, char** argv) {
    if (argc < 2 || argc > 3) {
      throw std::invalid_argument("Wrong number of arguments!");
    }

    std::ifstream in(argv[1]);
    Graph g;
    in >> g;
    in.close();

    if (argc == 2) {
      test(g, std::cout);
    }
    else {
      std::ofstream out(argv[2]);
      test(g, out);
      out.close();
    }

    return 0;
}
