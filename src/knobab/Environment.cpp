//
// Created by giacomo on 07/01/2022.
//

#include "knobab/Environment.h"

semantic_atom_set Environment::getSigmaAll() const {
    semantic_atom_set S = ap.act_atoms;
    for (const auto& ref : ap.interval_map) {
        std::pair<std::string, size_t> cp;
        cp.first = ref.first;
        for (size_t i = 0, N = ap.max_ctam_iteration.at(cp.first); i<N; i++) {
            cp.second = i;
            S.insert(ap.clause_to_atomization_map.at(cp));
        }
    }
    for (auto & ref : db.act_table_by_act_id.secondary_index) {
        auto ptr = ref.first;
        while (ptr) {
            auto lx = db.event_label_mapper.get(ptr->entry.id.parts.act);
            if (!ap.interval_map.contains(lx))
                S.insert(lx);
            ptr = ptr->next;
        };
    }
    return S;
}

void Environment::clear() {
    db.clear();
    ap.clear();
    conjunctive_model.clear();
}

#include <filesystem>

void Environment::load_model(const std::string &model_file) {
    if (!std::filesystem::exists(std::filesystem::path(model_file))) {
        std::cerr << "ERROR: model file does not exist: " << model_file << std::endl;
        exit(1);
    }
    std::ifstream file{model_file};
    conjunctive_model = dmp.load(file, true);
}

void Environment::load_log(log_data_format format, bool loadData, const std::string &filename) {
    load_into_knowledge_base(format, loadData, filename, db);
    db.index_data_structures();
}

void Environment::set_atomization_parameters(const std::string fresh_atom_label, size_t mslength) {
    ap.fresh_atom_label = fresh_atom_label;
    ap.s_max = std::string(mslength, std::numeric_limits<char>::max());
    DataPredicate::MAX_STRING = ap.s_max;
    DataPredicate::msl = mslength;
}

void
Environment::set_grounding_parameters(bool doPreliminaryFill, bool ignoreActForAttributes, bool creamOffSingleValues,
                                      GroundingStrategyConf::pruning_strategy ps) {
    grounding_conf.doPreliminaryFill = doPreliminaryFill;
    grounding_conf.ignoreActForAttributes = ignoreActForAttributes;
    grounding_conf.creamOffSingleValues = creamOffSingleValues;
    grounding_conf.strategy1 = ps;
}

void Environment::init_atomize_tables() {
    collect_data_from_declare_disjunctive_model(ap, grounding);
}

void Environment::doGrounding() {
    grounding = GroundWhereStrategy(grounding_conf,
                                    db,
                                    conjunctive_model);
}

void Environment::print_model(std::ostream &os) const {
    os << "Declarative Model: " << std::endl;
    os << "----------------------------------------" << std::endl;
    for (const auto& def : conjunctive_model)
        os << def << std::endl;

    os << "----------------------------------------" << std::endl;
    os << std::endl;
}

void Environment::print_grounded_model(std::ostream &os) const {
    os << "Grounded Model: " << std::endl;
    os << "----------------------------------------" << std::endl;
    os << grounding << std::endl;
    os << "----------------------------------------" << std::endl;
    os << std::endl;
}

void Environment::print_knowledge_base(std::ostream &os) const {
    os << "Knowledge Base: " << std::endl;
    os << "----------------------------------------" << std::endl;
    db.reconstruct_trace_with_data(os);
    os << "----------------------------------------" << std::endl;
    os << std::endl;
}

void Environment::print_grounding_tables(std::ostream &os) {
    os << "Grounding Tables: " << std::endl;
    os << "----------------------------------------" << std::endl;
    os << ap << std::endl;
    os << "----------------------------------------" << std::endl;
    os << std::endl;
}

void Environment::first_atomize_model() {
    atomize_model(ap, grounding);
}

semantic_atom_set Environment::evaluate_easy_prop_to_atoms(const easy_prop &prop,
                                                           const std::unordered_map<std::string, std::string> &bogus_act_to_atom,
                                                           const std::unordered_map<std::string, semantic_atom_set> &bogus_act_to_set) {
    switch (prop.casusu) {
        case easy_prop::E_P_AND:
            assert(prop.args.size() == 2);
            assert(!prop.isAtomNegated);
            return unordered_intersection(evaluate_easy_prop_to_atoms( prop.args.at(0), bogus_act_to_atom, bogus_act_to_set),
                                          evaluate_easy_prop_to_atoms( prop.args.at(1), bogus_act_to_atom, bogus_act_to_set) );
        case easy_prop::E_P_OR: {
            assert(prop.args.size() == 2);
            assert(!prop.isAtomNegated);
            semantic_atom_set S = evaluate_easy_prop_to_atoms( prop.args.at(0), bogus_act_to_atom, bogus_act_to_set);
            auto tmp = evaluate_easy_prop_to_atoms( prop.args.at(1), bogus_act_to_atom, bogus_act_to_set);
            S.insert(tmp.begin(), tmp.end());
            return S;
        }
        case easy_prop::E_P_ATOM:
            assert(prop.args.empty());
            assert(bogus_act_to_atom.contains(prop.single_atom_if_any));
            if (prop.isAtomNegated) {
                semantic_atom_set S = ap.atom_decomposition(bogus_act_to_atom.at(prop.single_atom_if_any));
                return unordered_difference(S, bogus_act_to_set.at(prop.single_atom_if_any));
            } else {
                return bogus_act_to_set.at(prop.single_atom_if_any);
            }
        case easy_prop::E_P_TRUE:
            return getSigmaAll();
        case easy_prop::E_P_FALSE:
            return {};
    }
}
