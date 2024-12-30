# HNSW
cd build/tests
./build_hnsw float l2 /nvmessd1/fbv4/prec1M.fbin /nvmessd1/fbv4/avarhade/grann/hnsw_prec1M_L100_R64_a1.2 64 100 0 0.1 2 48 >> ../../results/hnsw_results.txt
./search_hnsw float l2 /nvmessd1/fbv4/avarhade/grann/hnsw_prec1M_L100_R64_a1.2 2 1 /nvmessd1/fbv4/queries.fbin /nvmessd1/fbv4/prec1M_gt100.bin 50 /hnsw_results 50 75 100 150 200 >> ../../results/hnsw_results.txt

# DiskANN
# cd build/tests
# ./build_vamana float l2 /nvmessd1/fbv4/prec1M.fbin 0 /nvmessd1/fbv4/avarhade/grann/diskann_prec1M_L100_R64_a1.2 64 100 1.2 48 >> ../../results/diskann_results.txt
# ./search_vamana float l2 /nvmessd1/fbv4/avarhade/grann/diskann_prec1M_L100_R64_a1.2 1 /nvmessd1/fbv4/queries.fbin /nvmessd1/fbv4/prec1M_gt100.bin 50 /diskann_results 0 50 75 100 150 200 >> ../../results/diskann_results.txt 