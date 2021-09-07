﻿// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT license.

#pragma once
#include <algorithm>
#include <fcntl.h>
#include <cassert>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>
#include <memory>
#include <random>
#include <set>
#ifdef __APPLE__
#else
#include <malloc.h>
#endif

#ifdef _WINDOWS
#include <Windows.h>
typedef HANDLE FileHandle;
#else
#include <unistd.h>
typedef int FileHandle;
#endif

#include "cached_io.h"
#include "essentials.h"
#include "utils.h"
#include "windows_customizations.h"
#include "gperftools/malloc_extension.h"

namespace grann {
  const size_t   TRAINING_SET_SIZE = 100000;
  const double   SPACE_FOR_CACHED_NODES_IN_GB = 0.25;
  const double   THRESHOLD_FOR_CACHING_IN_GB = 1.0;
  const uint32_t NUM_NODES_TO_CACHE = 250000;
  const uint32_t WARMUP_L = 20;
  const uint32_t NUM_KMEANS_REPS = 12;

  template<typename T>
  class PQFlashIndex;

  GRANN_DLLEXPORT double calculate_recall(
      unsigned num_queries, unsigned *gold_std, float *gs_dist, unsigned dim_gs,
      unsigned *our_results, unsigned dim_or, unsigned recall_at);

GRANN_DLLEXPORT double calculate_range_search_recall(unsigned num_queries, std::vector<std::vector<_u32>> &groundtruth,
                          std::vector<std::vector<_u32>> &our_results);

  GRANN_DLLEXPORT void read_idmap(const std::string &    fname,
                                    std::vector<unsigned> &ivecs);

#ifdef EXEC_ENV_OLS
  template<typename T>
  GRANN_DLLEXPORT T *load_warmup(MemoryMappedFiles &files,
                                   const std::string &cache_warmup_file,
                                   uint64_t &warmup_num, uint64_t warmup_dim,
                                   uint64_t warmup_aligned_dim);
#else
  template<typename T>
  GRANN_DLLEXPORT T *load_warmup(const std::string &cache_warmup_file,
                                   uint64_t &warmup_num, uint64_t warmup_dim,
                                   uint64_t warmup_aligned_dim);
#endif

  GRANN_DLLEXPORT int merge_shards(const std::string &vamana_prefix,
                                     const std::string &vamana_suffix,
                                     const std::string &idmaps_prefix,
                                     const std::string &idmaps_suffix,
                                     const _u64 nshards, unsigned max_degree,
                                     const std::string &output_vamana,
                                     const std::string &medoids_file);

  template<typename T>
  GRANN_DLLEXPORT int build_merged_vamana_vamana(
      std::string base_file, grann::Metric _compareMetric, unsigned L,
      unsigned R, double sampling_rate, double ram_budget,
      std::string mem_vamana_path, std::string medoids_file,
      std::string centroids_file);

  template<typename T>
  GRANN_DLLEXPORT uint32_t optimize_beamwidth(
      std::unique_ptr<grann::PQFlashIndex<T>> &_pFlashIndex, T *tuning_sample,
      _u64 tuning_sample_num, _u64 tuning_sample_aligned_dim, uint32_t L,
      uint32_t nthreads, uint32_t start_bw = 2);

  template<typename T>
  GRANN_DLLEXPORT bool build_disk_vamana(const char *    dataFilePath,
                                          const char *    vamanaFilePath,
                                          const char *    vamanaBuildParameters,
                                          grann::Metric _compareMetric);

  template<typename T>
  GRANN_DLLEXPORT void create_disk_layout(const std::string base_file,
                                            const std::string mem_vamana_file,
                                            const std::string output_file);

}  // namespace grann
