//
// Created by giacomo on 17/01/23.
//

#include <string>

std::string query_plan = "queryplan \"mdpi23\" {\n"
                         "     template \"Init\"                   := INIT  activation\n"
                         "     template \"End\"                    := END activation\n"
                         "     template \"Exists1\"                := (EXISTS 1 activation)\n"
                         "     template \"Absence1\"               := ABSENCE 1 activation\n"
                         "     template \"Absence2\"               := ABSENCE 2 activation\n"
                         "     template \"Precedence\" args 2      := ((EXISTS  ~ 1 t #2) U (EXISTS 1 t #1 activation)) OR (ABSENCE 1 #2)\n"
                         "     template \"ChainPrecedence\" args 2 := G(((LAST OR t (NEXT EXISTS ~ 1 t #1))) OR t ((NEXT EXISTS 1 t #1 activation) AND t THETA INV (EXISTS 1 t #2 target) ))\n"
                         "     template \"Choice\" args 2          := (EXISTS 1 t #1 activation) OR THETA (EXISTS 1 t #2 activation)\n"
                         "     template \"Response\" args 2        := G ( ((EXISTS ~ 1 t #1)) OR t ((EXISTS 1 t #1 activation) &Ft THETA (EXISTS 1 t #2 target)) )\n"
                         "     template \"ChainResponse\" args 2   := G ( ((EXISTS ~ 1 t #1)) OR t ((EXISTS 1 t #1 activation) AND t THETA (NEXT EXISTS 1 t #2 target)))\n"
                         "     template \"RespExistence\" args 2   := ( ((ABSENCE 1 #1)) OR ((EXISTS 1 #1 activation) AND THETA (EXISTS 1 #2 target)))\n"
                         "     template \"ExlChoice\" args 2       := ((EXISTS 1 t #1 activation) OR THETA (EXISTS 1 t #2 activation)) AND ((ABSENCE 1 #1) OR (ABSENCE 1 #2))\n"
                         "     template \"CoExistence\" args 2     := ( ((ABSENCE 1 #1)) OR ((EXISTS 1 #1 activation) AND THETA (EXISTS 1 #2 target))) AND ( ((ABSENCE 1 #2)) OR ((EXISTS 1 #2 activation) AND THETA INV (EXISTS 1 #1 target)))\n"
                         "     template \"NotCoExistence\" args 2  := ~ ((EXISTS 1 t #1 activation) AND THETA (EXISTS 1 t #2 target)) PRESERVE\n"
                         "\n"
                         "     template \"Succession\" args 2      := (G ( ((EXISTS ~ 1 t #1)) OR t ((EXISTS 1 t #1 activation) &Ft THETA (EXISTS 1 t #2 target)) )) AND (((EXISTS  ~ 1 t #2) U (EXISTS 1 t #1 target)) OR (ABSENCE 1 #2))\n"
                         "     template \"NegSuccession\" args 2   := (G ( ((EXISTS ~ 1 t #1)) OR t ((EXISTS 1 t #1 activation) &Gt  (EXISTS ~ 1 t #2)) ))\n"
                         "     template \"ChainSuccession\" args 2 := G( (((LAST OR t (NEXT EXISTS ~ 1 t #2))) OR t ((NEXT EXISTS 1 t #2 activation) AND t THETA INV (EXISTS 1 t #1 target))) AND t\n"
                         "                                             ( ((EXISTS ~ 1 t #1)) OR t ((EXISTS 1 t #1 activation) AND t THETA (NEXT EXISTS 1 t #2 target)))\n"
                         "                                           )\n"
                         "     template \"AltResponse\" args 2     := G ( (EXISTS ~ 1 t #1) OR t ((EXISTS 1 t #1 activation) AND t THETA (NEXT ((EXISTS ~ 1 t #1) U t (EXISTS 1 t #2 target)) )))\n"
                         "     template \"AltPrecedence\" args 2   := (((EXISTS  ~ 1 t #2) U (EXISTS 1 t #1 activation)) OR (ABSENCE 1 #2)) AND\n"
                         "                                           (G(((EXISTS ~ 1 t #1)) OR t (((EXISTS 1 t #1 activation)) AND t THETA (NEXT (((EXISTS  ~ 1 t #1) U t (EXISTS 1 t #2 target)) OR t (G t (EXISTS  ~ 1 t #1))))  )))\n"
                         "}";

#include "knobab/mining/apriori.h"
#include "knobab/server/query_manager/ServerQueryManager.h"
#include <algorithm>
#include <stdexcept>

/// https://gist.github.com/marcinwol/3283a92331ff64a8f531
template <typename T,
        typename A,
        template <typename , typename > class C>
C<C<T,A>, std::allocator<C<T,A>>> chunker(const C<T,A>& c, const typename C<T,A>::size_type& k){
    if (k <= 0)
        throw std::domain_error("chunker() requires k > 0");
    using INPUT_CONTAINER_TYPE = C<T,A>;
    using INPUT_CONTAINER_VALUE_TYPE = typename INPUT_CONTAINER_TYPE::value_type;
    using OUTPUT_CONTAINER_TYPE = C<INPUT_CONTAINER_TYPE,
            std::allocator<INPUT_CONTAINER_TYPE>
    >;
    OUTPUT_CONTAINER_TYPE out_c;
    auto chunkBeg = begin(c);
    for (auto left=c.size(); left != 0; ) {
        auto const skip = std::min(left,k);
        INPUT_CONTAINER_TYPE sub_container;
        std::back_insert_iterator<INPUT_CONTAINER_TYPE> back_v(sub_container);
        std::copy_n(chunkBeg, skip, back_v);
        out_c.push_back(sub_container);
        left -= skip;
        std::advance(chunkBeg, skip);
    }
    return out_c;
}

std::ostream &operator<<(std::ostream &os, const FeedQueryLoadFromFile &file) {
    os << "load ";
    switch (file.format) {
        case HUMAN_READABLE_YAUCL: os << " HRF "; break;
        case XES1: os << " XES "; break;
        case TAB_SEPARATED_EVENTS: os << " TAB "; break;
    }
    os << std::quoted(file.file) << " ";
    if (file.with_data) os << " with data ";
    if (file.no_stats) os << " no stats ";
    if (file.with_missing) os << " with missing ";
    os << " as " << std::quoted(file.env_name);
    return os;
}

#include <sstream>
#include <iostream>

std::vector<DeclareDataAware> actual(Environment& env,
             double support,
             std::vector<std::string>& templates,
             std::vector<DeclareDataAware>& unary) {
    std::vector<size_t> act_support;
    std::unordered_set<std::string> map_to_multiple_patterns;
    auto actCounting = env.db.doTraceCounting();
    double nTraces = env.db.nTraces();
    for (const auto& [k, v] : actCounting) {
        double toK = ((double )k)/nTraces;
        if (toK >= support) {
            map_to_multiple_patterns.emplace(v);
            unary.emplace_back(DeclareDataAware::unary("Init", v, 1));
            unary.emplace_back(DeclareDataAware::unary("End", v, 1));
            unary.emplace_back(DeclareDataAware::unary("Absence1", v, 1));
            unary.emplace_back(DeclareDataAware::unary("Exists1", v, 1));
        }
    }

    std::vector<DeclareDataAware> result;
    for (const std::string& L : map_to_multiple_patterns) {
        for (const std::string& R : map_to_multiple_patterns) {
            std::vector<size_t> Candidate(2);
            Candidate[0] = env.db.event_label_mapper.get(L);
            Candidate[1] = env.db.event_label_mapper.get(R);
            double toK = ((double)env.db.doTraceCounting(Candidate))/nTraces;
            if (toK >= support) {
                for (const auto& ref : templates) {
                    result.emplace_back(DeclareDataAware::binary_for_testing(ref, L, R));
                }
            }
        }
    }
    return result;
}

/*
 * Implementation from the following paper:
 * Maggi, F.M., Bose, R.P.J.C., van der Aalst, W.M.P. "Efficient Discovery of Understandable Declarative Process Models from Event Logs". In: CAiSE 2012
 * https://link.springer.com/chapter/10.1007/978-3-642-31095-9_18
 */
void apriori(const std::string& logger_file,
             const FeedQueryLoadFromFile& log,
             double support,
             std::vector<std::string>& templates) {
    ServerQueryManager sqm;
    std::stringstream ss;
    constexpr size_t chunk_size = 40;

    // Loading the data
    ss << log;
    sqm.runQuery(ss.str());
    ss.str(std::string());
    ss.clear();
    std::cout << "Loading Outcome: " << sqm.getContent() << std::endl << std::endl;

    // Setting up the operators
    sqm.runQuery(query_plan);
    std::cout << "Operators Setting: " << sqm.getContent() << std::endl << std::endl;

    // Obtaining the model
    Environment& env = sqm.multiple_logs[log.env_name];
    std::vector<DeclareDataAware> unary;
    size_t batch = 1;
    auto binary_clauses = actual(env, support, templates, unary);
    for (const std::vector<DeclareDataAware>& x : chunker(binary_clauses, chunk_size)) {
        ss << "model-check declare " << std::endl;
        for (const auto& d : x) {
            ss << "\t" << std::quoted(d.casusu) << "(" << std::quoted(d.left_act) << " , true , " << std::quoted(d.right_act) << " , true ) " << std::endl;
        }
        ss << " using \"PerDeclareSupport\" over " << std::quoted(log.env_name) << std::endl;
        ss << " plan \"mdpi23\" "  << std::endl;
        ss << " with operators \"Hybrid\" ";
        sqm.runQuery(ss.str());
        ss.str(std::string());
        ss.clear();
        std::cout << "Query Batch #" <<  batch << ": " << sqm.getContent() << std::endl << std::endl;
        batch++;
    }

    for (const std::vector<DeclareDataAware>& x : chunker(unary, chunk_size)) {
        ss << "model-check declare " << std::endl;
        for (const auto& d : x) {
            ss << "\t" << std::quoted(d.casusu) << "( " << std::quoted(d.left_act) << ", true, 1 ) " << std::endl;
        }
        ss << " using \"PerDeclareSupport\" over " << std::quoted(log.env_name) << std::endl;
        ss << " plan \"mdpi23\" "  << std::endl;
        ss << " with operators \"Hybrid\" ";
        sqm.runQuery(ss.str());
        ss.str(std::string());
        ss.clear();
        std::cout << "Unary Batch: " << sqm.getContent() << std::endl << std::endl;
    }

    //Dumping to the logger file
    ss << "benchmarking-log " << std::quoted(logger_file);
    sqm.runQuery(ss.str());
    ss.str(std::string());
    ss.clear();
    std::cout << "Dumper: "<< sqm.getContent() << std::endl << std::endl;
}


void previous_mining(const std::string& logger_file,
             const FeedQueryLoadFromFile& log,
             double support,
             std::vector<std::string>& templates) {
    ServerQueryManager sqm;
    std::stringstream ss;

    // Loading the data
    ss << log;
    sqm.runQuery(ss.str());
    ss.str(std::string());
    ss.clear();
    std::cout << "Loading Outcome: " << sqm.getContent() << std::endl << std::endl;

    Environment& env = sqm.multiple_logs[log.env_name];
    size_t support_int = (size_t)((1.0 - support) * (double)env.db.nAct());

    // Setting up the operators
    sqm.runQuery(query_plan);
    std::cout << "Operators Setting: " << sqm.getContent() << std::endl << std::endl;

    std::vector<DeclareDataAware> unary;
    size_t batch = 1;
    for (const std::string& x : templates) {
        ss << "model-check template " << std::quoted(x) << " logtop " << support_int << std::endl;
        ss << " using \"PerDeclareSupport\" over " << std::quoted(log.env_name) << std::endl;
        ss << " plan \"mdpi23\" "  << std::endl;
        ss << " with operators \"Hybrid\" ";
        sqm.runQuery(ss.str());
        ss.str(std::string());
        ss.clear();
        std::cout << "Query Batch #" <<  batch << ": " << sqm.getContent() << std::endl << std::endl;
        batch++;
    }

    // TODO: the current function by Samuel only supports binary clauses
//    for (const auto& x : {"Init", "End", "Absence1", "Exists1"}) {
//        ss << "model-check template " << std::quoted(x) << " logtop " << support_int << std::endl;
//        ss << " using \"PerDeclareSupport\" over " << std::quoted(log.env_name) << std::endl;
//        ss << " plan \"mdpi23\" "  << std::endl;
//        ss << " with operators \"Hybrid\" ";
//        sqm.runQuery(ss.str());
//        ss.str(std::string());
//        ss.clear();
//        std::cout << "Query Batch #" <<  batch << ": " << sqm.getContent() << std::endl << std::endl;
//        batch++;
//    }

    //Dumping to the logger file
    ss << "benchmarking-log " << std::quoted(logger_file);
    sqm.runQuery(ss.str());
    ss.str(std::string());
    ss.clear();
    std::cout << "Dumper: "<< sqm.getContent() << std::endl << std::endl;
}