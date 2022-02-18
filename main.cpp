#include <iostream>
#include <vector>
#include <data_views/VariantIterator.h>

#include <ranges>
#include <data_operations/variant_iterator_operations.h>


int main() {
    std::pair<int, std::vector<std::pair<std::pair<uint32_t, uint16_t>, double>>> cp;
    cp.first = 1;
    std::vector<size_t> W{1,2,3};

    {
        VariantIterator beg(&cp, true, 3, &W, true);
        VariantIterator end(&cp, false, 3, &W, true);

        while (beg != end) {
            std::cout << *beg << std::endl;
            beg++;
        }

        std::cout << "-----------------------------------------------------------" << std::endl;

        //VariantIterator beg2(&cp, true, 3, &W, true);
        //std::cout << *VariantIterator::lower_bound(beg2, end, {0,100})<< std::endl;
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
        VariantIterator beg(&cp, true, 3, &W, true);
        VariantIterator end(&cp, false, 3, &W, true);

        while (beg != end) {
            std::cout << *beg << std::endl;
            beg.pop();
        }
    }
    std::cout << "-----------------------------------------------------------" << std::endl;

    {
        VariantIterator beg(&cp, true, 3, &W, true);
        VariantIterator end(&cp, false, 3, &W, true);

        auto filterB1 = VariantIterator::nextUntimedIterator(beg, end);
        auto filterE1 = VariantIterator::nextUntimedIterator(end, end);

        while (filterB1 != filterE1) {
            std::cout << *filterB1 << std::endl;
            filterB1.pop();
        }


        std::cout << "-----------------------------------------------------------" << std::endl;

        filterB1 = VariantIterator::nextUntimedIterator(beg, end);
        filterE1 = VariantIterator::nextUntimedIterator(end, end);
        auto filterB2 = VariantIterator::nextUntimedIterator(filterB1, filterE1);
        auto filterE2 = VariantIterator::nextUntimedIterator(filterE1, filterE1);


        std::cout << "-----------------------------------------------------------" << std::endl;
        std::cout << *lower_bound(filterB2, filterE2, {3,0})<< std::endl;

        std::cout << "-----------------------------------------------------------" << std::endl;
        while (filterB2!= filterE2) {
            std::cout << *filterB2 << std::endl;
            filterB2.pop();
        }

    }

    std::cout << "Hello, World!" << std::endl;
    return 0;
}
