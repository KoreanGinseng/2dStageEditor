#pragma once
#include "Exporter.h"
class JsonExporter : public IExporter {
public:
    virtual bool Export(const std::string& file) override;
};

