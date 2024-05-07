# %%
import hnswlib
import numpy as np
import numpy as np
import h5py
import os
import requests
import tempfile
import time
import struct
import itertools

from core_utils import read_float_binary_file,read_i8_binary_file,read_u8_binary_file,write_float_binary_file,read_int32_binary_file,compute_recall



# %%

[nqgt, nkgt, gt] = read_int32_binary_file('/home/rakri/datasets/wikipedia_large/wikipedia_gt_unfilt.bin')
#[nqgt, nkgt, gt] = read_int32_binary_file('/home/rakri/wiki_rnd100k_gs')
[nq, ndq, queries] = read_float_binary_file('/home/rakri/datasets/wikipedia_large/wikipedia_query.bin')
[nb, nd, dataset] = read_float_binary_file('/home/rakri/datasets/wikipedia_large/wikipedia_base.bin')
#[nb, nd, dataset] = read_float_binary_file('/home/rakri/wiki_rnd100k_data.bin')

print ("data loaded")

#names and parameters
dataset_name='wiki_large'
Mb = 48
efb = 400
index_name = '/home/rakri/indices/hnsw_'+dataset_name+'M='+str(Mb)+'_efC='+str(efb) 

# for hnsw, it is only ef_search
search_params = [(10), (20), (30), (40), (50), (60), (70), (80), (90), (100)]

# %%
# Create an HNSW index
index = hnswlib.Index(space='l2', dim=nd)



index.init_index(max_elements=nb, M = Mb, ef_construction = efb, random_seed = 100)

print ("index initialized")
if os.path.isfile(index_name):
    print("Index exists, so loading")
    index = hnswlib.Index(space='l2', dim=nd)
    index.load_index(index_name, max_elements = nb)
else:
    index.set_num_threads(48)
    start = time.time()
    index.add_items(dataset)
    end = time.time()
    print("Index built in", end-start, " seconds.")
    index.save_index(index_name)

# %%
# Search for the nearest neighbor of a query vector

index.set_num_threads(1)
nk=1
print("ef\tRecall\t\ttime")
for param in search_params:
    (ef_search) = param
    index.set_ef(ef_search)
    start = time.time()
    neighbors, distances = index.knn_query(queries, k=nk)
    end = time.time()
    recall = compute_recall(neighbors, gt[:,:nk])
    print(ef_search,"\t",recall,"\t",1000000*(end-start)/nq)
# Print the nearest neighbor
#print(neighbors)
#print(distances)


