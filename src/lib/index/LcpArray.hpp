#pragma once

#include "SourceIndex.hpp"
#include "SuffixArray.hpp"

#include <stack>
#include <vector>

// There are more far efficient ways to do this. We'll see how slow this is
// for the genome codebase and optimize if needed.
template<typename T>
std::vector<T> makeLcpArray(
        SourceIndex const& si,
        SuffixArray<T> const& sa)
{
    std::vector<T> lcpData(sa.size());
    T lastValue;

    lcpData[0] = 0;
    sa.foreach(
        [&] (size_t idx, T value) {
            if (idx == 0) {
                lcpData[idx] = 0;
            }
            else {
                lcpData[idx] = si.longestCommonPrefix(value, lastValue);
            }

            lastValue = value;
        });

    return lcpData;
}

struct LcpInterval {
    size_t lcp;
    size_t leftBound;
    size_t rightBound;

    bool operator==(LcpInterval const& rhs) const {
        return lcp == rhs.lcp
            && leftBound == rhs.leftBound
            && rightBound == rhs.rightBound
            ;
    }

    friend std::ostream& operator<<(std::ostream& out, LcpInterval const& lcpi) {
        out << "{lcp = " << lcpi.lcp << ", interval = ["
            << lcpi.leftBound << ", " << lcpi.rightBound << "]}";
        return out;
    }
};

template<typename T, typename Function>
void visitLcpIntervals(std::vector<T> const& lcps, Function f) {
    std::stack<LcpInterval> intervals;

    size_t leftBound(0);

    intervals.emplace(LcpInterval{0, 0, 0});
    for (size_t i = 1; i < lcps.size(); ++i) {
        leftBound = i - 1;

        assert(!intervals.empty());
        while (lcps[i] < intervals.top().lcp) {
            intervals.top().rightBound = i - 1;
            f(intervals.top());
            leftBound = intervals.top().leftBound;
            intervals.pop();
        }
        if (lcps[i] > intervals.top().lcp)
            intervals.emplace(LcpInterval{lcps[i], leftBound, 0});
    }
}
