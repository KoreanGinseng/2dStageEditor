#pragma once
#include "Exporter.h"
class OriginalExporter : public IExporter {
public:
    virtual bool Export(const std::string& file) override;
};

