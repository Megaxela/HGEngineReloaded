#pragma once

#include <HG/ToolsCore/CommandLineArguments.hpp>

namespace Operations
{
    void pack(const HG::ToolsCore::CommandLineArguments::ArgumentsMap& args);

    void unpack(const HG::ToolsCore::CommandLineArguments::ArgumentsMap& args);

    void info(const HG::ToolsCore::CommandLineArguments::ArgumentsMap& args);
};


