/*
 * DataTraceParse.cpp
 * This file is part of bpm21
 *
 * Copyright (C) 2021 - Giacomo Bergami
 *
 * bpm21 is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * bpm21 is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with bpm21. If not, see <http://www.gnu.org/licenses/>.
 */


//
// Created by giacomo on 12/03/21.
//

#include <yaucl/bpm/DataTraceParse.h>
#include <yaucl/bpm/TracesLexer.h>
#include <yaucl/bpm/TracesParser.h>

//using namespace yaucl::bpm;
#include <nlohmann/json.hpp>
#define UNESCAPE(x)                  nlohmann::json::parse(x).get<std::string>()

void yaucl::bpm::DataTraceParse::load(const std::string &stream_name, std::istream &stream, trace_visitor *tv) {
    this->tv = tv;
    isPayloadTrace = false;
    trace_count = 0;
    event_count = 0;
    antlr4::ANTLRInputStream input(stream);
    TracesLexer lexer(&input);
    antlr4::CommonTokenStream tokens(&lexer);
    TracesParser parser(&tokens);
    tv->enterLog(stream_name, stream_name);
    visitLog(parser.log());
    tv->exitLog(stream_name, stream_name);
    this->tv = nullptr;
    isPayloadTrace = false;
    //return visitLog(parser.log()).as<yaucl::bpm::log>();
}

antlrcpp::Any yaucl::bpm::DataTraceParse::visitLog(TracesParser::LogContext *ctx) {
    //yaucl::bpm::log log;
    if (ctx) {
        for (const auto& ptr : ctx->trace()) {
            visitTrace(ptr);
        }
    }
    return {};
}

antlrcpp::Any yaucl::bpm::DataTraceParse::visitTrace(TracesParser::TraceContext *ctx) {
    //yaucl::bpm::trace trace;
    if (ctx) {
        size_t event_count = 1;
        size_t traceId = tv->enterTrace(std::to_string(trace_count));
        isPayloadTrace = true;
        visitData_part(ctx->data_part());
        isPayloadTrace = false;
        event_count = 0;
        for (const auto& ptr : ctx->event()) {
            visitEvent(ptr);
        }
        event_count = 0;
        tv->exitTrace(traceId);
        trace_count++;
    }
    return {};
}

antlrcpp::Any yaucl::bpm::DataTraceParse::visitEvent(TracesParser::EventContext *ctx) {
    if (ctx) {
        const auto p1 = std::chrono::system_clock::now();
        unsigned long long int timestamp = std::chrono::duration_cast<std::chrono::hours>(p1.time_since_epoch()).count();
        size_t eid = tv->enterEvent(timestamp,ctx->LABEL()->getText());
        isPayloadTrace = false;
        visitData_part(ctx->data_part());
        isPayloadTrace = false;
        tv->exitEvent(eid);
        event_count++;
    }
    return {};
}

antlrcpp::Any yaucl::bpm::DataTraceParse::visitData_part(TracesParser::Data_partContext *ctx) {
    //std::unordered_map<std::string, std::variant<std::string, double>> map;
    if (ctx) {
        for (const auto& ptr : ctx->field()) {
            visitField(ptr);
        }
    }
    return {};
}

//#include "yaucl/strings/stringutils.h"

antlrcpp::Any yaucl::bpm::DataTraceParse::visitField(TracesParser::FieldContext *ctx) {
    if (ctx) {
        if (ctx->NUMBER()) {
            tv->visitField(ctx->VAR()->getText(), std::stod(ctx->NUMBER()->getText()));
        } else {
            assert(ctx->STRING());
            tv->visitField(ctx->VAR()->getText(), UNESCAPE(ctx->STRING()->getText()));
        }
    }
    return {};
}
