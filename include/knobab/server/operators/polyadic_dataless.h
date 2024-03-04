
#pragma once

inline void polydl_or_fast_timed(const Result& lhs, const Result& rhs, Result& out, const PredicateManager *manager = nullptr, const std::vector<size_t>& lengths = {}) {
    or_fast_timed(lhs, rhs, out, nullptr, lengths);
}

inline void polydl_or_fast_untimed(const Result& lhs, const Result& rhs, Result& out, const PredicateManager *manager = nullptr, const std::vector<size_t>& lengths = {}) {
    or_fast_untimed(lhs, rhs, out, nullptr, lengths);
}

inline void polydl_and_fast_timed(const Result& lhs, const Result& rhs, Result& out, const PredicateManager *manager = nullptr, const std::vector<size_t>& lengths = {}) {
    and_fast_timed(lhs, rhs, out, nullptr, lengths);
}

inline void polydl_and_fast_untimed(const Result& lhs, const Result& rhs, Result& out, const PredicateManager *manager = nullptr, const std::vector<size_t>& lengths = {}) {
    and_fast_untimed(lhs, rhs, out, nullptr, lengths);
}

inline void next_and_fast_untimed(const Result& lhs, const Result& rhs, Result& out, const PredicateManager *manager = nullptr, const std::vector<size_t>& lengths = {}) {
    and_fast_untimed(lhs, rhs, out, nullptr, lengths);
}