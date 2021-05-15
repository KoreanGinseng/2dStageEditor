#pragma once
#include "Parser.h"

class OriginalParser : public IParser {
public:
    virtual bool Parse(const std::string& buffer, ParseData* out) override;
};

