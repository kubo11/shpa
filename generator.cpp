#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

char* get_cmd_opt(char** beg, char** end, const std::string& opt) {
  char** it = std::find(beg, end, opt);
  if (it != end && ++it != end)
    return *it;
  return 0;
}

int main(int argc, char** argv) {
  srand(time(nullptr));
  if (argc > 9 || argc % 2 == 0) {
    std::cerr << "Wrong number of args" << std::endl;
    return 1;
  }

  char* val;
  std::string output = "test.txt";
  int vertices = 8, min_weight = 1, max_weight = 100;

  if (val = get_cmd_opt(argv, argv + argc, "-v")) {
    vertices = std::stoi(val);
  }
  if (val = get_cmd_opt(argv, argv + argc, "-o")) {
    output = val;
  }
  else {
    output = std::string("test_") + std::to_string(vertices) + std::string(".txt");
  }
  if (val = get_cmd_opt(argv, argv + argc, "-m")) {
    min_weight = std::stoi(val);
  }
  if (val = get_cmd_opt(argv, argv + argc, "-x")) {
    max_weight = std::stoi(val);
  }

  std::ofstream file(output, std::ios::trunc);
  if (!file.good()) {
    std::cerr << "Could not open: " << output << std::endl;
    return 1;
  }

  file << vertices << std::endl;
  file << min_weight + rand() % (max_weight - min_weight);
  for (int i = 1; i < vertices - 1; ++i) {
    file << std::endl << min_weight + rand() % (max_weight - min_weight);
    for (int j = 1; j < i + 1; ++j) {
      file << " " << min_weight + rand() % (max_weight - min_weight);
    }
  }

  file.close();
  return 0;
}