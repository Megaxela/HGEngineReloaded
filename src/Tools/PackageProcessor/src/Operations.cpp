// C++ STL
#include <iostream>

// HG::PackageProcessor
#include <Operations.hpp>
#include <ArgumentNames.hpp>

// HG::PackageProcessingLibrary
#include <HG/Tools/PackageProcessor.hpp>

void Operations::pack(const HG::ToolsCore::CommandLineArguments::ArgumentsMap& args)
{
    HG::Tools::PackageProcessor packageProcessor;

    // Getting directory with files
    auto value = args.find(ArgumentsNames::Path);

    if (value == args.end())
    {
        throw std::invalid_argument("No path argument");
    }

    std::filesystem::path path(std::get<std::string>(value->second));

    auto stat = std::filesystem::status(path);

    if (stat.type() != std::filesystem::file_type::directory)
    {
        throw std::invalid_argument("\"" + path.string() + "\" is not a directory.");
    }


}

void Operations::unpack(const HG::ToolsCore::CommandLineArguments::ArgumentsMap& args)
{

}

void Operations::info(const HG::ToolsCore::CommandLineArguments::ArgumentsMap& args)
{
    HG::Tools::PackageProcessor packageProcessor;

    // Getting path to package
    auto value = args.find(ArgumentsNames::Path);

    if (value == args.end())
    {
        throw std::invalid_argument("No path argument");
    }

    std::filesystem::path path(std::get<std::string>(value->second));

    if (std::filesystem::status(path).type() != std::filesystem::file_type::regular)
    {
        throw std::invalid_argument("\"" + path.string() + "\n is not a file.");
    }

    packageProcessor.load(path);

    const auto& metadata = packageProcessor.metadata();

    // Printing metadata
    std::cout << "Metadata: " << std::endl;

    std::cout << "    Name: " << metadata.name << std::endl;
    std::cout << "    Author: " << metadata.author << std::endl;
    std::cout << "    Version major: " << metadata.version.major << std::endl;
    std::cout << "    Version minor: " << metadata.version.minor << std::endl;

    // Printing files

}
