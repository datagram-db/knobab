//
// Created by giacomo on 05/10/24.
//

#ifndef DTMINING_FISH_H
#define DTMINING_FISH_H


#include "C/DN_HistogramMode_5.h"
#include "C/DN_HistogramMode_10.h"
#include "C/DN_Mean.h"
#include "C/DN_Spread_Std.h"
#include "C/CO_AutoCorr.h"
#include "C/DN_OutlierInclude.h"
#include "C/FC_LocalSimple.h"
#include "C/IN_AutoMutualInfoStats.h"
#include "C/MD_hrv.h"
#include "C/SB_BinaryStats.h"
#include "C/SB_MotifThree.h"
#include "C/SC_FluctAnal.h"
#include "C/SP_Summaries.h"
#include "C/SB_TransitionMatrix.h"
#include "C/PD_PeriodicityWang.h"


#include <vector>
#include <span>

typedef double (*catch24fun) (const double y[], const int);
typedef int (*catch24intfun) (const double y[], const int);

static inline double call_c_function(const std::span<double>& cppV, size_t begin, size_t end, catch24fun f) {
    if (cppV.empty())
        return 0.0;
    else if (begin == end)
        return cppV[begin];
    else
        return (*f)(cppV.data()+begin, end-begin+1);
}

static inline double call_c_int_function(const std::span<double>& cppV, size_t begin, size_t end, catch24intfun f) {
    if (cppV.empty())
        return 0.0;
    else if (begin == end)
        return cppV[begin];
    else
        return (*f)(cppV.data()+begin, end-begin+1);
}

#include <unordered_map>
#include <string>

using statistics_payload = std::unordered_map<std::string,double>;

template<class ForwardIt>
ForwardIt my_max_element(ForwardIt first, ForwardIt last)
{
    if (first == last)
        return last;

    ForwardIt largest = first;

    while (++first != last)
        if (*largest < *first)
            largest = first;

    return largest;
}

template<class ForwardIt>
ForwardIt my_min_element(ForwardIt first, ForwardIt last)
{
    if (first == last)
        return last;

    ForwardIt smallest = first;

    while (++first != last)
        if (*first < *smallest)
            smallest = first;

    return smallest;
}

#include <algorithm>
#include <numeric>

template<typename ForwardIt>
std::pair<double,double> my_mean_variance(ForwardIt first, ForwardIt last) {
    auto sz = std::distance(first, last);
    if (sz <= 1) {
        return {0.0,0.0};
    }

    // Calculate the mean
    const auto mean = std::accumulate(first, last, 0.0) / sz;

    // Now calculate the variance
    auto variance_func = [&mean, &sz](auto accumulator, const auto& val) {
        return accumulator + ((val - mean)*(val - mean) / (sz - 1));
    };

    return {mean,std::accumulate(first, last, 0.0, variance_func)};
}


#include <ranges>

template<typename ForwardIt>
double my_median(ForwardIt first, ForwardIt last) {
    auto sz = std::distance(first, last);
    if (sz <= 0) {
        return 0.0;
    } else if (sz == 1) {
        return *first;
    } else if (sz == 2) {
        return ((*first) + (*(first+1)))/2.0;
    }

    static std::vector<std::size_t> indexes;
    if (indexes.size() < sz)
        indexes.resize(sz, 0);
    std::iota(indexes.begin(), indexes.end(), 0); // 0z in C++23

    auto proj = [&first](std::size_t i) -> double { return *((first)+i); };
    std::ranges::sort(indexes, std::less<>{}, proj);
    if ((sz % 2) == 1) {
        return *(first + (sz/2));
    } else {
        auto i = sz / 2;
        return (*(first + (i - 1)) + *(first + (i))) / 2.0;
    }
}

#define SPAN        ("_____span")

/**
 * This class is the one performing the catch24 mining using the imported code
 */
class fish {
    std::unordered_map<std::string, catch24fun > funmap;
    bool isDataless{false};
public:
    fish();

    void setDataless() {
        funmap.clear();
        isDataless = true;
    }

    inline const bool dataless() const {
        return isDataless;
    }

    inline statistics_payload eval(const std::span<double>& dim_values,
                                   const std::span<double>& time_values,
                                   size_t begin, size_t end) const {
        statistics_payload m;

        auto itv = dim_values.begin() + begin;
        auto itt = time_values.begin() + begin;
        auto env = dim_values.begin() + (end + 1);
        auto ent = time_values.begin() + (end + 1);

        if (!isDataless) {
            for (const auto&[k,f] : funmap) {
                auto tmp = call_c_function(dim_values, begin, end, f);
                if (!std::isnan(tmp)) {
                    m.emplace("values_"+k, tmp);
                }
            }
            for (const auto&[k,f] : funmap) {
                m.emplace("time_"+k, call_c_function(time_values, begin, end, f));
            }
            m.emplace("values_acf_first_min", call_c_int_function(dim_values, begin, end, CO_FirstMin_ac));
            m.emplace("values_periodicity", call_c_int_function(dim_values, begin, end, PD_PeriodicityWang_th0_01));
            m.emplace("time_acf_first_min", call_c_int_function(time_values, begin, end, CO_FirstMin_ac));
            m.emplace("time_periodicity", call_c_int_function(time_values, begin, end, PD_PeriodicityWang_th0_01));
        } else {
            auto cp = my_mean_variance(itv, env);
            m.emplace("values_mean", cp.first);
            m.emplace("values_var", cp.second);
            m.emplace("values_stdev", std::sqrt(cp.second));
            m.emplace("values_median", my_median(itv, env));
            cp = my_mean_variance(itt, ent);
            m.emplace("time_mean", cp.first);
            m.emplace("time_var", cp.second);
            m.emplace("time_stdev", std::sqrt(cp.second));
            m.emplace("time_median", my_median(itt, ent));
        }
        m.emplace("values_max", *my_max_element(itv, env));
        m.emplace("values_min", *my_min_element(itv, env));
        m.emplace("time_max", *my_max_element(itt, ent));
        m.emplace("time_min", *my_min_element(itt, ent));
        return m;
    }

};
#endif //DTMINING_FISH_H
