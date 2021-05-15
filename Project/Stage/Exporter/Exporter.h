#pragma once
#include <string>
#include <memory>

class IExporter {
public:

    virtual bool Export(const std::string& file) = 0;
};

using ExporterPtr = std::shared_ptr<IExporter>;