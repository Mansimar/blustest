// Copyright (c) Microsoft Corporation.
// Licensed under the MIT license.

/*
 * Simple benchmark that runs a mixture of point lookups and inserts on ALEX.
 */

#include "BPTree.hpp"

#include <iomanip>
#include "utils.h"


// Modify these if running your own workload
#define KEY_TYPE long long
#define PAYLOAD_TYPE double
#define DOUBLE_KEY_TYPE double
#define INDEX_PAYLOAD_TYPE int

/*
 * Required flags:
 * --keys_file              path to the file that contains keys
 * --keys_file_type         file type of keys_file (options: binary or text)
 * --init_num_keys          number of keys to bulk load with
 * --total_num_keys         total number of keys in the keys file
 * --batch_size             number of operations (lookup or insert) per batch
 *
 * Optional flags:
 * --insert_frac            fraction of operations that are inserts (instead of
 * lookups)
 * --lookup_distribution    lookup keys distribution (options: uniform or zipf)
 * --time_limit             time limit, in minutes
 * --print_batch_stats      whether to output stats for each batch
 */


void LoadData(BPTree<int64_t>& index, int64_t* keys, int num_keys, string keys_file_path){
        int load_end_point = 0;


    std::cout<<"Running benchmark workload "<<"\n";
    std::string benchmarkName = "parameters.values[0].GetValue<string>()";
    int init_num_keys = load_end_point;
    int total_num_keys = 100000;
    int batch_size = 10000;
    double insert_frac = 0.5;
    string lookup_distribution = "zipf";
    int i = init_num_keys;
    long long cumulative_inserts = 0;
    long long cumulative_lookups = 0;
    int num_inserts_per_batch = static_cast<int>(batch_size * insert_frac);
    int num_lookups_per_batch = batch_size - num_inserts_per_batch;
    double cumulative_insert_time = 0;
    double cumulative_lookup_time = 0;
    double time_limit = 0.5;
    bool print_batch_stats = true;
    


    auto workload_start_time = std::chrono::high_resolution_clock::now();
    int batch_no = 0;
    PAYLOAD_TYPE sum = 0;
    std::cout << std::scientific;
    std::cout << std::setprecision(3);
    while (true) {
        batch_no++;

        // Do lookups
        double batch_lookup_time = 0.0;
        if (i > 0) {
        int64_t* lookup_keys = nullptr;
        if (lookup_distribution == "uniform") {
            lookup_keys = get_search_keys(keys, i, num_lookups_per_batch);
        } else if (lookup_distribution == "zipf") {
            lookup_keys = get_search_keys_zipf(keys, i, num_lookups_per_batch);
        } else {
            std::cerr << "--lookup_distribution must be either 'uniform' or 'zipf'"
                    << std::endl;
            //return 1;
        }
        auto lookups_start_time = std::chrono::high_resolution_clock::now();
        // for (int j = 0; j < num_lookups_per_batch; j++) {
        //     DOUBLE_KEY_TYPE key = lookup_keys[j];
        //     INDEX_PAYLOAD_TYPE* payload = index.get_payload(key);
        //     if (payload) {
        //     sum += *payload;
        //     }
        // }
        auto lookups_end_time = std::chrono::high_resolution_clock::now();
        batch_lookup_time = std::chrono::duration_cast<std::chrono::nanoseconds>(
                                lookups_end_time - lookups_start_time)
                                .count();
        cumulative_lookup_time += batch_lookup_time;
        cumulative_lookups += num_lookups_per_batch;
        delete[] lookup_keys;
        }

        // Do inserts
        int num_actual_inserts =
            std::min(num_inserts_per_batch, total_num_keys - i);
        int num_keys_after_batch = i + num_actual_inserts;
        auto inserts_start_time = std::chrono::high_resolution_clock::now();
        for (; i < num_keys_after_batch; i++) {
            // index.insert({keys[i], i});
            index.Insert(keys[i], i);
        }
        auto inserts_end_time = std::chrono::high_resolution_clock::now();
        double batch_insert_time =
            std::chrono::duration_cast<std::chrono::nanoseconds>(inserts_end_time -
                                                                inserts_start_time)
                .count();
        cumulative_insert_time += batch_insert_time;
        cumulative_inserts += num_actual_inserts;

        if (print_batch_stats) {
        int num_batch_operations = num_lookups_per_batch + num_actual_inserts;
        double batch_time = batch_lookup_time + batch_insert_time;
        long long cumulative_operations = cumulative_lookups + cumulative_inserts;
        double cumulative_time = cumulative_lookup_time + cumulative_insert_time;
        std::cout << "Batch " << batch_no
                    << ", cumulative ops: " << cumulative_operations
                    << "\n\tbatch throughput:\t"
                    << num_lookups_per_batch / batch_lookup_time * 1e9
                    << " lookups/sec,\t"
                    << num_actual_inserts / batch_insert_time * 1e9
                    << " inserts/sec,\t" << num_batch_operations / batch_time * 1e9
                    << " ops/sec"
                    << "\n\tcumulative throughput:\t"
                    << cumulative_lookups / cumulative_lookup_time * 1e9
                    << " lookups/sec,\t"
                    << cumulative_inserts / cumulative_insert_time * 1e9
                    << " inserts/sec,\t"
                    << cumulative_operations / cumulative_time * 1e9 << " ops/sec"
                    << std::endl;
        }

        // Check for workload end conditions
        if (num_actual_inserts < num_inserts_per_batch) {
        // End if we have inserted all keys in a workload with inserts
        break;
        }
        double workload_elapsed_time =
            std::chrono::duration_cast<std::chrono::nanoseconds>(
                std::chrono::high_resolution_clock::now() - workload_start_time)
                .count();
        if (workload_elapsed_time > time_limit * 1e9 * 60) {
        break;
        }
    }

    long long cumulative_operations = cumulative_lookups + cumulative_inserts;
    double cumulative_time = cumulative_lookup_time + cumulative_insert_time;
    std::cout << "Cumulative stats: " << batch_no << " batches, "
                << cumulative_operations << " ops (" << cumulative_lookups
                << " lookups, " << cumulative_inserts << " inserts)"
                << "\n\tcumulative throughput:\t"
                << cumulative_lookups / cumulative_lookup_time * 1e9
                << " lookups/sec,\t"
                << cumulative_inserts / cumulative_insert_time * 1e9
                << " inserts/sec,\t"
                << cumulative_operations / cumulative_time * 1e9 << " ops/sec"
                << std::endl;

    //delete[] keys;
    // delete[] values;
}

void runLookupBenchmarkOneBatchAlex(BPTree<int64_t>& index, int num_keys, int64_t* keys){
    std::cout<<"Running benchmark with one batch";
    /*
    My rationale here - I will run the benchmark for one batch - read a defined number of keys and count the time needed to do that.
    */

   // Create a random number generator
    // std::random_device rd;
    // std::mt19937 g(rd());

//    vector<double> payloads;
//    vector<int64_t>keys;
//    for(int i=0;i<results.size();i++){
//         vector<unique_ptr<Base>>& vec = results.at(i);
//         keys.push_back(dynamic_cast<BigIntData*>(vec[0].get())->value);
//         payloads.push_back(dynamic_cast<DoubleData*>(vec[1].get())->value);
//     }
    int64_t sum = 0;
    // for(int i=0;i<payloads.size();i++){
    //     sum += payloads[i];
    // }
    // std::cout<<"Sum of payloads "<<sum<<"\n";
    // std::cout<<"Average "<<sum/payloads.size()<<"\n";

    // std::shuffle(keys.begin(), keys.end(), g);
    std::cout<<"Keys have been shuffled!\n";
    auto start = std::chrono::high_resolution_clock::now();
    int total = 0;
    for(int i=0;i<num_keys;i++){
        auto key = keys[i];
        
        float value;
        bool found = index.Search(key, value);
        if (found) {
            sum += value;
            total += 1;
            // cout << "Value found: " << value << endl;
        } else {
            // cout << "Value not found." << endl;
        }
        // if (it != big_int_alex_index.end()) {
        //     double payload = it.payload();
        //     sum+=payload;
        // }
    }
    std::cout<<"Average : "<<sum/num_keys<<"\n";
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed_seconds = end - start;
    std::cout << "Time taken to lookup "<<num_keys <<" keys is "<< elapsed_seconds.count() << " seconds\n";
    std::cout<<"Checking Correctness: \n";

    start = std::chrono::high_resolution_clock::now();
    // auto res = con.Query(query);
    // if(!res->HasError()){
    //     res->Print();
    // }
    cout << "Total Value found: " << total << endl;
    end = std::chrono::high_resolution_clock::now();
    elapsed_seconds = end - start;
    std::cout << "Time taken to avg from DuckDB is "<< num_keys <<" keys is "<< elapsed_seconds.count() << " seconds\n";
}



int main(int argc, char* argv[]) {
    int load_end_point = 0;

    //longitudes
    // BPTree<double> index;
    // std::string keys_file_path = "/Users/Mansimar/Downloads/longlat-200M.bin.data";

    //longitudes
    // BPTree<double> index;
    // std::string keys_file_path = "/Users/Mansimar/Downloads/longitudes-200M.bin.data";

    //lognormal
    // BPTree<int64_t> index;
    // std::string keys_file_path = "/Users/Mansimar/Downloads/lognormal-190M.bin.data";

    //ycsb
    BPTree<int64_t> index;
    std::string keys_file_path = "/Users/Mansimar/Downloads/ycsb-200M.bin.data";

    // auto keys = new double[100000];
    auto keys = new int64_t[100000];

    std::string keys_file_type = "binary";
    if (keys_file_type == "binary") {
        std::cout<<"Loading binary data "<<std::endl;
        load_binary_data(keys, 100000, keys_file_path);
    } else if (keys_file_type == "text") {
        load_text_data(keys, 100000, keys_file_path);
    } else {
        std::cerr << "--keys_file_type must be either 'binary' or 'text'"
                << std::endl;
        //return 1;
    }

    LoadData(index, keys, 100000, keys_file_path);
    runLookupBenchmarkOneBatchAlex(index, 100000, keys);

    return 0;
}



