#pragma once

#include <cstddef>
#include <istream>
#include <string>
#include <vector>

struct SourceFile {
    std::string name;
    size_t firstByte;
    size_t lastByte;
    std::vector<size_t> lineStartPositions;

    size_t globalToFileOffset(size_t offset);
    bool operator==(SourceFile const& rhs) const;

    size_t size() const;

    static std::vector<SourceFile> fromJson(std::istream& in);
};