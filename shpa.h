#ifndef SHPA_SHPA_H
#define SHPA_SHPA_H

#include "graph.h"
#include "path.h"

Path shpa(const Graph& g);
Path hpa3_bf(const Graph& g, unsigned int s, unsigned int f);
Path hpa3(const Graph& g, unsigned int s, unsigned int f);

#endif // SHPA_SHPA_H