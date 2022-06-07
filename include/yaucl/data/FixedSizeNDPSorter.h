//
// Created by giacomo on 30/04/2022.
//

#ifndef KNOBAB_SERVER_FIXEDSIZENDPSORTER_H
#define KNOBAB_SERVER_FIXEDSIZENDPSORTER_H

#include <yaucl/data/MemoryMappingFile.h>
#include <filesystem>
#include <algorithm>
#include <queue>
#include "new_iovec.h"


template <typename T> class FixedSizeNDPSorter {

    size_t runs_size;
    int partition(std::vector<T> &array, const int low, const int high) {
        // select the rightmost element as pivot
        T& pivot = array[high];

        // pointer for greater element
        int i = (low - 1);

        // traverse each element of the array
        // compare them with the pivot
        for (int j = low; j < high; j++) {
            if (array[j] <= pivot) {

                // if element smaller than pivot is found
                // swap it with the greater element pointed by i
                i++;

                // swap element at i with element at j
                std::swap(array[i], array[j]);
            }
        }

        // swap pivot with the greater element at i
        std::swap(array[i + 1], array[high]);

        // return the partition point
        return (i + 1);
    }

    void do_quicksort(std::vector<T> &array, const int low, const int high) {
        if (low < high) {

            // find the pivot element such that
            // elements smaller than pivot are on left of pivot
            // elements greater than pivot are on righ of pivot
            int pi = partition(array, low, high);

            // recursive call on the left of pivot
            do_quicksort(array, low, pi - 1);

            // recursive call on the right of pivot
            do_quicksort(array, pi + 1, high);
        }
    }

    void do_quicksort(std::vector<T>& arr) {
        do_quicksort(arr, 0, arr.size()-1);
    }

public:

    FixedSizeNDPSorter(size_t size_runs) : runs_size{size_runs} {}

    void sort(const std::filesystem::path& filename, const std::filesystem::path& tmp_path) {
        constexpr size_t Tsize = sizeof(T);
        yaucl::data::MemoryMappedFile file{filename};
        size_t full = file.file_size();
        size_t min_runs_size = std::min(file.file_size(), runs_size);
        size_t num_ways = (size_t)std::ceil((double)file.file_size() / (double)min_runs_size);
        size_t N = full / Tsize;
        size_t Nblock = min_runs_size / Tsize;

        std::vector<yaucl::data::MemoryMappedFile> externalFiles(num_ways);
        std::vector<size_t> externalFileCounter;
        std::vector<T> in_memory_file_copy;

        for (size_t i = 0; i<num_ways; i++) {
            size_t M = (((i+1)*Nblock) > N) ? (N)-i*Nblock : Nblock;
            for (size_t j = 0; j<M; j++) {
                auto& ref = file.at<T>(i*Nblock+j);
                if (j >= in_memory_file_copy.size())
                    in_memory_file_copy.emplace_back(ref);
                else
                    in_memory_file_copy[j] = ref;
            }
            in_memory_file_copy.resize(M);
            do_quicksort(in_memory_file_copy);
            auto tmpFile = tmp_path / (std::to_string(i)+".tmp");
            {
                std::fstream toSort = std::fstream(tmpFile, std::ios::out | std::ios::binary);
                for (size_t pos = 0, O = in_memory_file_copy.size(); pos<O; pos++) {
                    toSort.write((char*)&in_memory_file_copy[pos], Tsize);
                }
            }
            externalFiles[i].open(tmpFile);
            externalFileCounter.emplace_back(0);
        }

        std::priority_queue<std::pair<T, size_t>,
                            std::vector<std::pair<T, size_t>>,
                            std::greater<std::pair<T, size_t>>> minheap;
        auto k = externalFiles.size();
        size_t i;
        for (i = 0; i < k; i++){
            auto count = externalFileCounter.at(i);
            auto& val = externalFiles.at(i).at<T>(count);
            minheap.emplace(val, i);
        }

        auto out = tmp_path / ("result.tmp");
        std::fstream tmpSort = std::fstream(out, std::ios::out | std::ios::binary);

        // Now one by one get the minimum element from min
        // heap and replace it with next element.
        // run till all filled input files reach EOF
        while (!minheap.empty()) {
            // Get the minimum element and store it in output file
            auto& ref = minheap.top();
            size_t currFile = ref.second;
            auto& currF = externalFiles.at(currFile);
            tmpSort.write((char*)&ref.first, Tsize);
            size_t currFileOffset = (++externalFileCounter[ref.second]);
            minheap.pop();

            // Find the next element that will replace current
            // root of heap. The next element belongs to same
            // input file as the current min element.
            if (currFileOffset != currF.cast_size<T>()) {
                minheap.emplace(currF.at<T>(currFileOffset), currFile);
            } else {
                currF.close();
            }
        }

        tmpSort.close();
        std::filesystem::remove(filename);
        std::filesystem::rename(out, filename);
    }
};

template <typename T> class FixedSizeReaderWriter {
    std::fstream file;
    std::filesystem::path filename;
    bool isWrite, isRead;
    yaucl::data::FixedSizeArrayElements<T> reader;

    void prepareWrite() {
        if (isRead) {
            reader.close();
            isRead = false;
        }
        if (!isWrite) {
            file = std::fstream(filename, std::ios::out | std::ios::binary);
            isWrite = true;
        }
    }
    void prepareRead() {
        if (isWrite) {
            file.close();
            isWrite = false;
        }
        if (!isRead) {
            reader.open(filename);
            isRead = true;
        }
    }


public:
    FixedSizeReaderWriter(const std::filesystem::path& file) : filename{file}, isWrite{false}, isRead{false}{}

    void put(const T& data) {
        prepareWrite();
        file.write((char*)&data, sizeof(T));
    }
    void put(const new_iovec& data) {
        prepareWrite();
        file.write((char*)data.iov_base, data.iov_len);
    }
    size_t size() {
        prepareRead();
        return reader.size();
    }
    T get(size_t i) {
        prepareRead();
        return reader[i];
    }
    void sort(size_t size_runs, const std::filesystem::path& tmp_path) {
        close();
        FixedSizeNDPSorter<T> sorter(size_runs);
        sorter.sort(filename, tmp_path);
    }
    void close() {
        if (isWrite) {
            file.close();
            isWrite = false;
        }
        if (isRead) {
            reader.close();
            isRead = false;
        }
    }
};


#endif //KNOBAB_SERVER_FIXEDSIZENDPSORTER_H