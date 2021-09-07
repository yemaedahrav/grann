# DiskANN

The goal of the project is to build scalable, performant and cost-effective approximate nearest neighbor search algorithms.
The initial release has the in-memory version of the [DiskANN paper](https://papers.nips.cc/paper/9527-rand-nsg-fast-accurate-billion-point-nearest-neighbor-search-on-a-single-node.pdf) published in NeurIPS 2019. 
This code reuses and builds upon some of the [code for NSG](https://github.com/ZJULearning/nsg) algoritm.

This project has adopted the [Microsoft Open Source Code of Conduct](https://opensource.microsoft.com/codeofconduct/).
For more information see the [Code of Conduct FAQ](https://opensource.microsoft.com/codeofconduct/faq/) or
contact [opencode@microsoft.com](mailto:opencode@microsoft.com) with any additional questions or comments.

See [guidelines](CONTRIBUTING.md) for contributing to this project.



## Linux build:

Install the following packages through apt-get, and Intel MKL either by downloading the installer or using [apt](https://software.intel.com/en-us/articles/installing-intel-free-libs-and-python-apt-repo) (we tested with build 2019.4-070).
```
sudo apt install cmake g++ libaio-dev libgoogle-perftools-dev clang-format-4.0 libboost-dev
```

Build
```
mkdir build && cd build && cmake .. && make -j 
```

## Windows build:

The Windows version has been tested with the Enterprise editions of Visual Studio 2017 and Visual Studio 2019. It should work with the Community and Professional editions as well without any changes. 

**Prerequisites:**

* Install CMAKE (v3.15.2 or later) from https://cmake.org
* Install MKL from https://software.intel.com/en-us/mkl
* Install/download Boost from https://www.boost.org

* Environment variables: 
    * Set a new System environment variable, called INTEL_ROOT to the "windows" folder under your MKL installation
	   (For instance, if your install folder is "C:\Program Files (x86)\IntelSWtools", set INTEL_ROOT to "C:\Program Files (x86)\IntelSWtools\compilers_and_libraries\windows")
    * Set environment variable BOOST_ROOT to your boost folder.

**Build steps:**
-	Open a new command prompt window
-	Create a "build" directory under grann
-	Change to the "build" directory and run  
```
<full-path-to-cmake>\cmake -G "Visual Studio 16 2019" -B. -A x64 ..
```
OR 
```
<full-path-to-cmake>\cmake -G "Visual Studio 15 2017" -B. -A x64 ..
```

**Note: Since VS comes with its own (older) version of cmake, you have to specify the full path to cmake to ensure that the right version is used.**
-	This will create a “grann” solution file in the "build" directory
-	Open the "grann" solution and build the “grann” project. 
- 	Then build all the other binaries using the ALL_BUILD project that is part of the solution
- 	Generated binaries are stored in the grann/x64/Debug or grann/x64/Release directories.

To build from command line, change to the "build" directory and use msbuild to first build the "diskpriority_io" and "grann_dll" projects. And then build the entire solution, as shown below.
```
msbuild src\dll\grann.vcxproj
msbuild grann.sln
```
Check msbuild docs for additional options including choosing between debug and release builds.


## Usage:

We now detail the main binaries using which one can build and search indices which reside in memory as well as SSD-resident indices.

**Usage for SSD-based indices**
===============================

To generate an SSD-friendly vamana, use the `tests/build_disk_vamana` program. 
----------------------------------------------------------------------------

```
./tests/build_disk_vamana  [data_type<float/int8/uint8>]  [data_file.bin]  [vamana_prefix_path]  [R]  [L]  [B]  [M]  [T]. 
```

The arguments are as follows:

(i) data_type:  The datatype is the type of dataset you wish to build an vamana. We support byte indices (signed int8 or unsigned uint8) or float indices. 

(ii) data_file: The input data over which to build an vamana, in .bin format. The first 4 bytes represent number of points as integer. The next 4 bytes represent the dimension of data as integer. The following n*d*sizeof(T) bytes contain the contents of the data one data point in time. sizeof(T) is 1 for byte indices, and 4 for float indices. This will be read by the program as int8_t for signed indices, uint8_t for unsigned indices or float for float indices.

(iii) vamana_prefix_path: the vamana will generate a few files, all beginning with the specified prefix path. For example, if you provide ~/vamana_test as the prefix path, build  generates files such as ~/vamana_test_pq_pivots.bin, ~/vamana_test_pq_compressed.bin, ~/vamana_test_disk.vamana, etc. There may be between 8 and 10 files generated with this prefix depending on how we construct the vamana.

(iv) R: the degree of our graph vamana, typically between 60 and 150. Again, larger values will result in bigger indices (with longer vamanaing times), but better search quality. Try to ensure that the L value is at least the R value unless you need to build indices really quickly, but can somewhat compromise on quality. 

(v) L: the size of search list we maintain during vamana building. Typical values are between 75 to 200. Larger values will take more time to build but result in indices that provide higher recall for the same search parameters.

(vi) B: bound on the memory footprint of the vamana at search time. Once built, the vamana will use up only the specified RAM limit, the rest will reside on disk. This will dictate how aggressively we compress the data vectors to store in memory. Larger will yield better performance at search time.

(vii) M: Limit on the memory allowed for building the vamana. If you specify a value less than what is required to build the vamana in one pass, the vamana is  built using a divide and conquer approach so that  sub-graphs will fit in the RAM budget. The sub-graphs are  stitched together to build the overall vamana. This approach can be upto 1.5 times slower than building the vamana in one shot. Try to allocate as much memory as possible for vamana build as your RAM allows.

(viii) T: number of threads used by the vamana build process. Since the code is highly parallel, the  vamanaing time improves almost linearly with the number of threads (subject to the cores available on the machine).

To search the SSD-vamana, use the `tests/search_disk_vamana` program. 
----------------------------------------------------------------------------

```
./tests/search_disk_vamana  [vamana_type<float/int8/uint8>]  [vamana_prefix_path]  [num_nodes_to_cache]  [num_threads]  [beamwidth (use 0 to optimize internally)]  [query_file.bin]  [truthset.bin (use "null" for none)]  [K]  [result_output_prefix]  [L1]  [L2] etc.
```

The arguments are as follows:

(i) data type: same as (i) above in building vamana.

(ii) vamana_prefix_path: same as (iii) above in building vamana.

(iii) num_nodes_to_cache: our program stores the entire graph on disk. For faster search performance, we provide the support to cache a few nodes (which are closest to the starting point) in memory. 

(iv) num_threads: search using specified number of threads in parallel, one thread per query. More will result in more IOs, so find the balance depending on the bandwidth of the SSD.

(v) beamwidth: maximum number of IO requests each query will issue per iteration of search code. Larger beamwidth williult in fewer IO round-trips per query, but might result in slightly higher number of IO requests to SSD per query. Specifying 0 will optimize the beamwidth depending on the number of threads performing search.

(vi) query_file.bin: search on these queries, same format as data file (ii) above. The query file must be the same type as specified in (i).

(vii) truthset.bin file. Must be in the following format, or specify "null": n, the number of queries (4 bytes) followed by d, the number of ground truth elements per query (4 bytes), followed by n*d entries per query representing the d closest IDs per query in integer format,  followed by n*d entries representing the corresponding distances (float). Total file size is 8 + 4*n*d + 4*n*d. The groundtruth file, if not available, can be calculated using our program, tests/utils/compute_groundtruth. If you just want to measure the latency numbers of search and output the nearest neighbors without calculating recall, enter "null".

(viii) K: measure recall@k, meaning the accuracy of retrieving top-k nearest neighbors.

(ix) result output prefix: search results will be stored in files with specified prefix, in bin format.

(x, xi, ...) various search_list sizes to perform search with. Larger will result in slower latencies, but higher accuracies. Must be atleast the recall@ value in (vi).


**Usage for in-memory indices**
================================

To generate vamana, use the `tests/build_memory_vamana` program. 
--------------------------------------------------------------

```
./tests/build_memory_vamana  [data_type<int8/uint8/float>]  [data_file.bin]  [output_vamana_file]  [R]  [L]  [alpha]  [num_threads_to_use]
```

The arguments are as follows:

(i) data_type: same as (i) above in building disk vamana.

(ii) data_file: same as (ii) above in building disk vamana, the input data file in .bin format of type int8/uint8/float.

(iii) output_vamana_file: memory vamana will be saved here.

(iv) R: max degree of vamana: larger is typically better, range (50-150). Preferrably ensure that L is at least R.

(v) L: candidate_list_size for building vamana, larger is better (typical range: 75 to 200)

(vi) alpha: float value which determines how dense our overall graph will be, and diameter will be log of n base alpha (roughly). Typical values are between 1 to 1.5. 1 will yield sparsest graph, 1.5 will yield denser graphs.

(vii) number of threads to use: vamanaing uses specified number of threads.


To search the generated vamana, use the `tests/search_memory_vamana` program:
---------------------------------------------------------------------------

```
./tests/search_memory_vamana  [vamana_type<float/int8/uint8>]  [data_file.bin]  [memory_vamana_path]  [query_file.bin]  [truthset.bin (use "null" for none)] [K]  [result_output_prefix]  [L1]  [L2] etc. 
```

The arguments are as follows:

(i) data type: same as (i) above in building vamana.

(ii) memory_vamana_path: enter path of vamana built (argument (iii) above in building memory vamana).

(iii) query_bin: search on these queries, same format as data file (ii) above. The query file must be the same type as specified in (i).

(iv) Truthset file. Must be in the following format: n, the number of queries (4 bytes) followed by d, the number of ground truth elements per query (4 bytes), followed by n*d entries per query representing the d closest IDs per query in integer format,  followed by n*d entries representing the corresponding distances (float). Total file size is 8 + 4*n*d + 4*n*d. The groundtruth file, if not available, can be calculated using our program, tests/utils/compute_groundtruth.

(v) K: search for recall@k, meaning accuracy of retrieving top-k nearest neighbors.

(vi) result output prefix: will search and store the computed results in the files with specified prefix in bin format.

(vii, viii, ...) various search_list sizes to perform search with. Larger will result in slower latencies, but higher accuracies. Must be atleast the recall@ value in (vi).
