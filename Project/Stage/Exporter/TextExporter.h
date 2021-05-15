#pragma once
#include "Exporter.h"
class TextExporter : public IExporter {
public:
    virtual bool Export(const std::string& file) override;
};

