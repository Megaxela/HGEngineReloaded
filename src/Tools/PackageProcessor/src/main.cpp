// C++ STL
#include <filesystem>
#include <functional>
#include <iostream>

// HG::PackageProcessor
#include <ArgumentNames.hpp>
#include <Operations.hpp>

// HG::ToolsCore
#include <HG/ToolsCore/CommandLineArguments.hpp>


namespace Commands
{
    // todo c++20: std::variant is no longer as shit as it was. std::string can be replaced with const char*
    const std::string Pack = "pack";
    const std::string Unpack = "unpack";
    const std::string Info = "info";
}

int main(int argc, char** argv)
{
    const std::unordered_map<
        std::string,
        std::function<
            void(const HG::ToolsCore::CommandLineArguments::ArgumentsMap&)
        >
    > operations = {
        {Commands::Pack, &Operations::pack},
        {Commands::Info, &Operations::info},
        {Commands::Unpack, &Operations::unpack}
    };

    HG::ToolsCore::CommandLineArguments arguments(argv[0]);

    arguments.addArgument({"--version"})
        .help("prints utility version")
        .action(HG::ToolsCore::CommandLineArguments::Action::Version)
        .version("0.2"); // todo: Add define with version, proxied from engine build system.

    arguments.addArgument({"-c", "--command"})
        .help("command to execute")
        .numberOfArguments(1)
        .required(true)
        .choices({Commands::Pack, Commands::Unpack, Commands::Info})
        .destination(ArgumentsNames::Command);


    arguments.addArgument({"-p", "--path"})
        .help("path to directory with package or with package content")
        .numberOfArguments(1)
        .required(true)
        .destination(ArgumentsNames::Path);

    arguments.addArgument({"-o", "--output"})
        .help("path to command output")
        .numberOfArguments(1)
        .required(false)
        .destination(ArgumentsNames::Output);

    arguments.addArgument({"-a", "--author"})
        .help("required for `pack` operation. describes package author")
        .numberOfArguments(1)
        .required(false)
        .destination(ArgumentsNames::Author);

    arguments.addArgument({"-n", "--name"})
        .help("required for `pack` operation. describes package name")
        .numberOfArguments(1)
        .required(false)
        .destination(ArgumentsNames::Name);


    arguments.addArgument({"-v", "--major"})
        .help("required for `pack` operation. describes package major version")
        .numberOfArguments(1)
        .required(false)
        .type(HG::ToolsCore::CommandLineArguments::Type::Integer)
        .destination(ArgumentsNames::MajorVersion);

    arguments.addArgument({"-m", "--minor"})
        .help("required for `pack` operation. describes package minor version")
        .numberOfArguments(1)
        .required(false)
        .type(HG::ToolsCore::CommandLineArguments::Type::Integer)
        .destination(ArgumentsNames::MinorVersion);

    auto args = arguments.parse(argc, argv);

    try
    {
        auto command = std::get<std::string>(args.find(ArgumentsNames::Command)->second);

        operations.find(command)->second(args);
    }
    catch (const std::invalid_argument& e)
    {
        std::cout << "Invalid argument: \"" << e.what() << "\". See help below." << std::endl;
        arguments.showHelp();
        return 1;
    }
    catch (const std::runtime_error& e)
    {
        std::cout << "Error: \"" << e.what() << "\"" << std::endl;
        return 2;
    }

    return 0;
}