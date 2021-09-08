// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT license.

#pragma once

#include "graph_index.h"
#include "utils.h"
#include "distance.h"



namespace grann {


  template<typename T>
  class Vamana : public GraphIndex<T> {
   public:
    GRANN_DLLEXPORT Vamana(Metric m, const char *filename, std::vector<_u32> &list_of_ids);
    GRANN_DLLEXPORT ~Vamana();

    // checks if data is consolidated, saves graph, metadata and associated
    // tags.
    GRANN_DLLEXPORT void save(const char *filename);
    GRANN_DLLEXPORT void load(const char *filename);

    GRANN_DLLEXPORT void build(Parameters &parameters);

//returns # results found (will be <= res_count)
    _u32 search(const T * query, _u32 res_count, Parameters &search_params, _u32 *indices, float *distances, QueryStats *stats = nullptr);

    /*  Internals of the library */
   protected:
    size_t       _num_steiner_pts;
    unsigned     _start_node;

  };
}  // namespace grann
