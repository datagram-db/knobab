#include <iostream>
#include <vector>
#include "include/data_views/RangeQueryIterator.h"


int main() {
    std::pair<int, std::vector<std::pair<std::pair<uint32_t, uint16_t>, double>>> cp;
    cp.first = 1;
    std::vector<size_t> W{1,2,3};

    {
        RangeQueryIterator beg(&cp, true, 3, W, true);
        RangeQueryIterator end(&cp, false, 3, W, true);

        while (beg != end) {
            std::cout << *beg << std::endl;
            beg++;
        }
    }

    std::cout << "-----------------------------------------------------------" << std::endl;

    cp.first = 2;
    cp.second.emplace_back(std::pair<uint32_t, uint16_t>{0,0},1);
    cp.second.emplace_back(std::pair<uint32_t, uint16_t>{1,1},1);
    cp.second.emplace_back(std::pair<uint32_t, uint16_t>{1,2},1);
    cp.second.emplace_back(std::pair<uint32_t, uint16_t>{3,1},1);
    cp.second.emplace_back(std::pair<uint32_t, uint16_t>{4,2},1);
    W.emplace_back(5);

    {
        RangeQueryIterator beg(&cp, true, 3, W, true);
        RangeQueryIterator end(&cp, false, 3, W, true);

        while (beg != end) {
            std::cout << *beg << std::endl;
            beg++;
        }
    }

    std::cout << "Hello, World!" << std::endl;
    return 0;
}
