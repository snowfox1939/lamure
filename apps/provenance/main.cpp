#include "lamure/pro/common.h"
#include "lamure/pro/data/DenseCache.h"
#include "lamure/pro/data/SparseCache.h"
#include <chrono>
#include <lamure/pro/data/DenseStream.h>

using namespace std;

char *get_cmd_option(char **begin, char **end, const string &option)
{
    char **it = find(begin, end, option);
    if(it != end && ++it != end)
        return *it;
    return 0;
}

bool cmd_option_exists(char **begin, char **end, const string &option) { return find(begin, end, option) != end; }
bool check_file_extensions(string name_file, const char *pext)
{
    string ext(pext);
    if(name_file.substr(name_file.size() - ext.size()).compare(ext) != 0)
    {
        cout << "Please specify " + ext + " file as input" << endl;
        return true;
    }
    return false;
}

int main(int argc, char *argv[])
{
    if(argc == 1 || !cmd_option_exists(argv, argv + argc, "-s") || !cmd_option_exists(argv, argv + argc, "-d"))
    {
        cout << "Usage: " << argv[0] << " <flags> -s <project>.sparse.prov  -d <project>.dense.prov" << endl << endl;
        return -1;
    }

    string name_file_sparse = string(get_cmd_option(argv, argv + argc, "-s"));
    string name_file_dense = string(get_cmd_option(argv, argv + argc, "-d"));
    // string name_file_lod = string(get_cmd_option(argv, argv + argc, "-l"));

    if(check_file_extensions(name_file_sparse, "sparse.prov") && check_file_extensions(name_file_dense, "dense.prov"))
    {
        throw std::runtime_error("File format is incompatible");
    }

    prov::ifstream in_sparse(name_file_sparse, std::ios::in | std::ios::binary);
    prov::ifstream in_sparse_meta(name_file_sparse + ".meta", std::ios::in | std::ios::binary);
    prov::ifstream in_dense(name_file_dense, std::ios::in | std::ios::binary);
    prov::ifstream in_dense_meta(name_file_dense + ".meta", std::ios::in | std::ios::binary);
    //prov::ifstream in_lod_meta(name_file_lod + ".meta", std::ios::in | std::ios::binary);

    prov::SparseCache cache_sparse(in_sparse, in_sparse_meta);
    prov::DenseCache cache_dense(in_dense, in_dense_meta);

    if(in_sparse.is_open())
    {
        auto start = std::chrono::high_resolution_clock::now();
        cache_sparse.cache();
        auto end = std::chrono::high_resolution_clock::now();
        printf("Caching sparse data took: %f ms\n", std::chrono::duration<double, std::milli>(end - start));
        in_sparse.close();
    }

    if(in_dense.is_open())
    {
        auto start = std::chrono::high_resolution_clock::now();
        cache_dense.cache();
        auto end = std::chrono::high_resolution_clock::now();
        printf("Caching dense data took: %f ms\n", std::chrono::duration<double, std::milli>(end - start));
        in_dense.close();
    }

    in_dense.open(name_file_dense, std::ios::in | std::ios::binary);

    prov::DenseStream stream_dense = prov::DenseStream(in_dense);

    if(in_dense.is_open()) {

        auto start = std::chrono::high_resolution_clock::now();

        for (uint32_t i = 0; i < cache_dense.get_points().size(); i++) {
            stream_dense.access_at_implicit(i);
        }

        auto end = std::chrono::high_resolution_clock::now();
        printf("Streaming dense data one-by-one took: %f ms\n", std::chrono::duration<double, std::milli>(end - start));

        start = std::chrono::high_resolution_clock::now();

        std::vector<prov::DensePoint> vector = stream_dense.access_at_implicit_range(0, (uint32_t) cache_dense.get_points().size());

        end = std::chrono::high_resolution_clock::now();
        printf("Streaming dense data by range took: %f ms\n", std::chrono::duration<double, std::milli>(end - start));
    }

    in_dense.close();

//    prov::LoDMetaStream stream_lod = prov::LoDMetaStream(in_lod_meta);
//
//    if(in_lod_meta.is_open()) {
//
//        auto start = std::chrono::high_resolution_clock::now();
//
//        for (uint32_t i = 0; i < cache_dense.get_points().size(); i++) {
//            stream_lod.access_at_implicit(i);
//        }
//
//        auto end = std::chrono::high_resolution_clock::now();
//        printf("Streaming %lu LoD deviations one-by-one took: %f ms\n", cache_dense.get_points().size(), std::chrono::duration<double, std::milli>(end - start));
//    }
//
//    in_lod_meta.close();

    return 0;
}