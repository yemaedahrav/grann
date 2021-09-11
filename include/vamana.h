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
    Vamana(Metric m, const char *filename, std::vector<_u32> &list_of_ids);
    //    ~Vamana();

    // checks if data is consolidated, saves graph, metadata and associated
    // tags.
    void save(const char *filename);
    void load(const char *filename);

    void build(Parameters &parameters);

    // returns # results found (will be <= res_count)
    _u32 search(const T *query, _u32 res_count, Parameters &search_params,
                _u32 *indices, float *distances, QueryStats *stats = nullptr);

    /*  Internals of the library */
   protected:
    //  _u64   _num_steiner_pts;
    unsigned _start_node;

    unsigned calculate_entry_point();
    void     get_expanded_nodes(const _u64 node_id, const unsigned l_build,
                                std::vector<unsigned>     init_ids,
                                std::vector<Neighbor> &   expanded_nodes_info,
                                tsl::robin_set<unsigned> &expanded_nodes_ids);

    void occlude_list(std::vector<Neighbor> &pool, const float alpha,
                      const unsigned degree, const unsigned maxc,
                      std::vector<Neighbor> &result);

    void prune_neighbors(const unsigned location, std::vector<Neighbor> &pool,
                         const Parameters &     parameter,
                         std::vector<unsigned> &pruned_list);

    void inter_insert(unsigned n, std::vector<unsigned> &pruned_list,
                      const Parameters &parameters);
  };
}  // namespace grann
