#pragma once
#include "Parser.h"

class TextParser : public IParser {
public:
    virtual bool Parse(const std::string& buffer, ParseData* out) override;
};

