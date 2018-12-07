// HG::ToolsCore
#include <HG/ToolsCore/CommandLineArguments.hpp>

namespace ArgumentsNames
{
    const char* Command = "command";
    const char* Path = "path";
}

namespace Commands
{
    // todo c++20: std::variant is no longer as shit as it was. std::string can be replaced with const char*
    const std::string Pack = "pack";
    const std::string Unpack = "unpack";
    const std::string Metainfo = "metainfo";
}

int main(int argc, char** argv)
{
    HG::ToolsCore::CommandLineArguments arguments(argv[0]);

    arguments.addArgument({"-v", "--version"})
        .help("prints utility version")
        .action(HG::ToolsCore::CommandLineArguments::Action::Version)
        .version("0.2"); // todo: Add define with version, proxied from engine build system.

    arguments.addArgument({"-c", "--command"})
        .help("command to execute")
        .numberOfArguments(1)
        .required(true)
        .choices({Commands::Pack, Commands::Unpack, Commands::Metainfo})
        .destination(ArgumentsNames::Command);

    arguments.addArgument({"-p", "--path"})
        .help("path to directory with package or with package content")
        .numberOfArguments(1)
        .required(true)
        .destination(ArgumentsNames::Path);

    auto args = arguments.parse(argc, argv);



    return 0;
}