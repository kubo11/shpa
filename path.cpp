#include "path.h"

bool Edge::operator==(const Edge& other) const {
  return beg == other.beg && end == other.end && weight == other.weight;
}

Path::Path() : m_length(Path::inf), m_edges() {}

Path::Path(const std::vector<Edge>& edges) : m_length(0), m_edges(edges) {
  for (const auto& e : m_edges) {
    m_length += e.weight;
  }
}

bool Path::add_edge(const Edge& e) {
  if (m_edges.empty()) {
    m_edges.push_back(e);
    m_length = e.weight;
    return true;
  }
  else if (m_edges.back().end == e.beg) {
    m_edges.push_back(e);
    m_length += e.weight;
    return true;
  }

  return false;
}

int Path::get_length() const {
  return m_length;
}

std::vector<Edge>& Path::get_edges() {
  return m_edges;
}

bool Path::empty() const {
  return m_edges.empty();
}

void Path::trim_front(unsigned int n) {
  while (!m_edges.empty() && n > 0) {
    m_length -= m_edges.front().weight;
    m_edges.erase(m_edges.begin());
    --n;
  }
}

void Path::trim_back(unsigned int n) {
  while (!m_edges.empty() && n > 0) {
    m_length -= m_edges.back().weight;
    m_edges.pop_back();
    --n;
  }
}

Path Path::operator+(const Edge& e) const {
  Path p = *this;
  p.add_edge(e);
  return p;
}

Path Path::operator+(const Path& p) const {
  if (m_edges.empty()) {
    return p;
  }
  if (p.m_edges.empty()) {
    return *this;
  }
  if (m_edges.back().end != p.m_edges.begin()->beg) {
    return Path();
  }

  Path pp = *this;
  pp.m_edges.insert(pp.m_edges.end(), p.m_edges.begin(), p.m_edges.end());
  pp.m_length += p.m_length;
  return pp;
}

bool Path::operator==(const Path& other) const {
  return m_length == other.m_length && m_edges == other.m_edges;
}

std::ostream& operator<<(std::ostream& out, const Path& p) {
  if (p.m_length == Path::inf) {
    out << "inf";
  }
  else {
    out << p.m_length << std::endl;
  }
  for (const auto& e : p.m_edges) {
    out << e.beg << " ";
  }
  if (!p.m_edges.empty()) {
    out << p.m_edges.back().end;
  }

  return out;
}

