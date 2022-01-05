//
// Created by giacomo on 03/01/2022.
//

#include <knobab/algorithms/atomization_pipeline.h>
#include <cassert>
#include <yaucl/structures/StringPrevNext.h>
#include <yaucl/structures/set_operations.h>

std::string AtomizingPipeline::generate_fresh_atom() {
    return fresh_atom_label+std::to_string(count_fresh_atoms++);
}

void AtomizingPipeline::clear() {
    count_fresh_atoms = 0;

    map1.clear();
    act_atoms.clear();
    atom_universe.clear();
    act_universe.clear();

    double_bulk_map.clear();
    double_map.clear();
    string_bulk_map.clear();
    string_map.clear();

    interval_map.clear();
    clause_to_atomization_map.clear();
    Mcal.clear();
    max_ctam_iteration.clear();
}

semantic_atom_set AtomizingPipeline::atom_decomposition(const std::string &act, bool isNegated) {
    semantic_atom_set S;
    auto it = interval_map.find(act);
    if (it == interval_map.end()) {
        // If the atom is not associated to an interval, then return the atom itself
        assert(act_atoms.contains(act));
        S.insert(act);
    } else {
        std::pair<std::string, size_t> cp; // convenience element, so not to re-allocate the memory all the time
        cp.first = act;
        for (size_t i = 0, N = max_ctam_iteration.at(act); i<N; i++) {
            cp.second = i;
            S.insert(clause_to_atomization_map.at(cp));
        }
    }
    return isNegated ? unordered_difference(atom_universe, S) : S;
}

semantic_atom_set AtomizingPipeline::interval_decomposition(const DataPredicate &pred, bool isNegated) {
    semantic_atom_set S;
    if (pred.isStringPredicate()) {
        const auto V = std::get<0>(pred.decompose_single_variable_into_intervals());
        auto& ref = string_map.at(pred.label).at(pred.var);
        for (const auto& cp : V) {
            for (const auto& I : ref.findInterval(cp.first, cp.second)) {
                DataPredicate dp{pred.label, pred.var, I.first, I.second};
                assert(Mcal.contains(dp));
                auto v = Mcal.at(dp);
                S.insert(v.begin(), v.end());
            }
        }
    } else {
        const auto V = std::get<1>(pred.decompose_single_variable_into_intervals());
        auto& ref = double_map.at(pred.label).at(pred.var);
        for (const auto& cp : V) {
            for (const auto& I : ref.findInterval(cp.first, cp.second)) {
                DataPredicate dp{pred.label, pred.var, I.first, I.second};
                assert(Mcal.contains(dp));
                auto v = Mcal.at(dp);
                S.insert(v.begin(), v.end());
            }
        }
    }
    return isNegated ? unordered_difference(atom_universe, S) : S;
}

std::ostream &operator<<(std::ostream &os, const AtomizingPipeline &pipeline) {
        os.precision(17);
        os << "Sigma " << std::endl << std::endl;
        os << " * Single acts: " << std::endl;
        os << "================" << std::endl;
        for (const auto& ref : pipeline.act_atoms) {
            os << "    " << ref << std::endl;
        }
        os << std::endl << std::endl;
        os << " * Sigma Boxes: " << std::endl;
        os << "================" << std::endl;
        std::pair<std::string, size_t> cp;
        for (const auto& k : pipeline.interval_map) {
            os << "   - " << k.first << std::endl;
            cp.first = k.first;
            for (size_t i = 0, N = pipeline.max_ctam_iteration.at(cp.first); i<N; i++) {
                cp.second = i;
                os << "        * "
                   << pipeline.clause_to_atomization_map.at(cp)
                   << " --> "
                   << k.second.at(i)
                   << std::endl;
            }
        }


        os << std::endl << std::endl;
        os << " * Predicate Conversions: " << std::endl;
        os << "==========================" << std::endl;
        for (const auto& k : pipeline.Mcal) {
            os << "   - " << k.first<< " --> " << k.second << std::endl;
        }
        return os;
}

void collect_data_from_declare_disjunctive_model(AtomizingPipeline &pipeline_data, const CNFDeclareDataAware &disjoint_model) {
    // old: pipeline_scratch, init_pipeline(3)
    std::cout << "Collecting the atoms from the formula" << std::endl;
    for (const auto& conjunction : disjoint_model.singleElementOfConjunction) {
        for (const auto& declare_clause : conjunction.elementsInDisjunction) {
            pipeline_data.act_universe.insert(declare_clause.left_act);
            if (!declare_clause.right_act.empty())
                pipeline_data.act_universe.insert(declare_clause.right_act);
            assert(declare_clause.conjunctive_map.empty());
            for (const auto& itemList : declare_clause.dnf_left_map) {
                for (const auto& item : itemList) {
                    auto& formula_numeric_atom = item.second;
                    assert(formula_numeric_atom.BiVariableConditions.empty());
                    assert(formula_numeric_atom.varRHS.empty());
                    if (formula_numeric_atom.casusu == TTRUE) continue;
                    pipeline_data.map1[formula_numeric_atom.label][formula_numeric_atom.var].insert(formula_numeric_atom);
                    auto x = formula_numeric_atom.decompose_single_variable_into_intervals();
                    if (formula_numeric_atom.isStringPredicate()) {
                        auto  V = std::get<0>(x);
                        pipeline_data.string_bulk_map[formula_numeric_atom.label][formula_numeric_atom.var].insert(V.begin(), V.end());
                    } else {
                        auto  V = std::get<1>(x);
                        pipeline_data.double_bulk_map[formula_numeric_atom.label][formula_numeric_atom.var].insert(V.begin(), V.end());
                    }
                }
            }
        }
    }

    // init_pipeline(4)
    std::cout << "Collecting the atoms associated to no interval" << std::endl;
    for (const auto& act : pipeline_data.act_universe) {
        auto it1 = pipeline_data.double_bulk_map.find(act);
        bool test1 = (it1 == pipeline_data.double_bulk_map.end()) || (it1->second.empty());
        auto it2 = pipeline_data.string_bulk_map.find(act);
        bool test2 = (it2 == pipeline_data.string_bulk_map.end()) || (it2->second.empty());
        if (test1 && test2) {
            pipeline_data.act_atoms.insert(act);
            pipeline_data.atom_universe.insert(act);
        }
    }

    if ((!pipeline_data.double_bulk_map.empty()) || (!(pipeline_data.string_bulk_map.empty()))) {
        //decompose_and_atomize();

        std::cout << "Generating the distinct intervals from the elements" << std::endl;
        std::cout << " - strings " << std::endl;
        for (auto ref = pipeline_data.string_bulk_map.begin(); ref != pipeline_data.string_bulk_map.cend(); ){
            for (auto& ref2 : ref->second) {
                std::vector<DataPredicate> result;

                // old: perform_insertion
                pipeline_data.string_map[ref->first][ref2.first] =
                        ref2.second.finalize<StringPrevNext>(pipeline_data.s_min, pipeline_data.s_max);
                auto ref2Second = pipeline_data.string_map[ref->first][ref2.first];

                for (const auto& I : ref2Second.collect_intervals2()) {
                    result.emplace_back(ref->first, ref2.first, I.first, I.second);
                }
                pipeline_data.interval_map[ref->first].emplace_back(result);
            }
            ref = pipeline_data.string_bulk_map.erase(ref); // Clearing the map from bulk insertions while iterating.
        }

        std::cout << " - doubles " << std::endl;
        for (auto ref = pipeline_data.double_bulk_map.begin(); ref != pipeline_data.double_bulk_map.cend(); ){
            for (auto& ref2 : ref->second) {
                std::vector<DataPredicate> result;

                // old: perform_insertion
                pipeline_data.double_map[ref->first][ref2.first] =
                        ref2.second.finalize<DoublePrevNext>(pipeline_data.d_min, pipeline_data.d_max);
                auto ref2Second = pipeline_data.double_map[ref->first][ref2.first];

                for (const auto& I : ref2Second.collect_intervals2()) {
                    result.emplace_back(ref->first, ref2.first, I.first, I.second);
                }
                pipeline_data.interval_map[ref->first].emplace_back(result);
            }
            ref = pipeline_data.double_bulk_map.erase(ref); // Clearing the map from bulk insertions while iterating.
        }

        std::cout << "Generating the interval composition box" << std::endl;
        for (auto& ref: pipeline_data.interval_map) {
            std::vector<std::vector<DataPredicate>> W;
            for (const auto& v : cartesian_product(ref.second)) {
                std::vector<DataPredicate> V;
                V.insert(V.end(), v.begin(), v.end());
                W.emplace_back(V);
            }
            ref.second = W;
            for (size_t i = 0, N = W.size(); i<N; i++) {
                std::string FA = pipeline_data.generate_fresh_atom();
                pipeline_data.atom_universe.insert(FA);
                pipeline_data.clause_to_atomization_map[std::make_pair(ref.first, i)] = FA;
                for (const DataPredicate& pred : W.at(i)) {
                    pipeline_data.Mcal[pred].emplace_back(FA);
                }
            }
            pipeline_data.max_ctam_iteration[ref.first] = W.size();
        }
    }
}