//
// Created by Sam on 24/01/2022.
//

#ifndef KNOBAB_AGGREGATORS_H
#define KNOBAB_AGGREGATORS_H

class Aggregators {
public:
    template<typename Input1, typename Input2, typename Output>
    static Output maxSimilarity(Input1 t1, Input2 t2){
        return t1 > t2 ? t1 : t2;
    }

    template<typename Input1, typename Input2, typename Output>
    static Output joinSimilarity(Input1 t1, Input2 t2){
        return t1 * t2;
    }
};


#endif //KNOBAB_AGGREGATORS_H
