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

#define SPAN        ("_____span")

/**
 * This class is the one performing the catch24 mining using the imported code
 */
class fish {
    std::unordered_map<std::string, catch24fun > funmap;
public:
    fish();

    inline statistics_payload eval(const std::span<double>& dim_values,
                                   const std::span<double>& time_values,
                                   size_t begin, size_t end) const {
        statistics_payload m;
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
        m.emplace("values_max", *my_max_element(dim_values.begin() + begin, dim_values.begin() + (end + 1)));
        m.emplace("values_min", *my_min_element(dim_values.begin() + begin, dim_values.begin() + (end + 1)));
        m.emplace("time_acf_first_min", call_c_int_function(time_values, begin, end, CO_FirstMin_ac));
        m.emplace("time_periodicity", call_c_int_function(time_values, begin, end, PD_PeriodicityWang_th0_01));
        m.emplace("time_max", *my_max_element(time_values.begin() + begin, time_values.begin() + (end + 1)));
        m.emplace("time_min", *my_min_element(time_values.begin() + begin, time_values.begin() + (end + 1)));
        return m;
    }

};
#endif //DTMINING_FISH_H
