#ifndef SHPA_SHPA_H
#define SHPA_SHPA_H

#include "graph.h"
#include "path.h"

Path shpa(const Graph& g);
Path hpa3_bf(const Graph& g, unsigned int s, unsigned int f);
Path hpa3(const Graph& g, unsigned int s, unsigned int f);
Path process_subgraphs(const Graph& g, const FastSet& a_prim,
                       const FastSet& b_prim, unsigned int s, unsigned int c,
                       unsigned int f);

#endif // SHPA_SHPA_H