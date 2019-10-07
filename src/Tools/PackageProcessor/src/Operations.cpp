// C++ STL
#include <functional>
#include <iostream>

// HG::PackageProcessor
#include <ArgumentNames.hpp>
#include <Operations.hpp>

// HG::PackageProcessingLibrary
#include <HG/Tools/PackageProcessor.hpp>

void addFiles(HG::Tools::PackageProcessor& packageProcessor,
              const std::filesystem::path& path,
              const std::filesystem::path& root)
{
    for (const auto& entry : std::filesystem::directory_iterator(path))
    {
        if (entry.status().type() == std::filesystem::file_type::directory)
        {
            addFiles(packageProcessor, entry.path(), root);
            continue;
        }

        if (entry.status().type() != std::filesystem::file_type::regular)
        {
            continue;
        }

        std::cout << "Adding: " << entry.path() << std::endl;

        packageProcessor.addFile(std::move(entry.path()));
    }
}

void Operations::pack(const HG::ToolsCore::CommandLineArguments::ArgumentsMap& args)
{
    HG::Tools::PackageProcessor packageProcessor;

    // Validating arguments
    if (!args.count(ArgumentsNames::Name) || !args.count(ArgumentsNames::Author) ||
        !args.count(ArgumentsNames::MajorVersion) || !args.count(ArgumentsNames::MinorVersion) ||
        !args.count(ArgumentsNames::Path) || !args.count(ArgumentsNames::Output))
    {
        throw std::invalid_argument("Wrong arguments");
    }

    std::cout << "Packing" << std::endl;

    const auto outputArg = std::get<std::string>(args.at(ArgumentsNames::Output));
    const auto pathArg   = std::get<std::string>(args.at(ArgumentsNames::Path));
    const auto nameArg   = std::get<std::string>(args.at(ArgumentsNames::Name));
    const auto authorArg = std::get<std::string>(args.at(ArgumentsNames::Author));
    const auto majorArg  = std::get<int>(args.at(ArgumentsNames::MajorVersion));
    const auto minorArg  = std::get<int>(args.at(ArgumentsNames::MinorVersion));

    std::filesystem::path path(pathArg);

    auto stat = std::filesystem::status(path);

    if (stat.type() != std::filesystem::file_type::directory)
    {
        throw std::invalid_argument("\"" + path.string() + "\" is not a directory.");
    }

    std::cout << "Package name: " << nameArg << std::endl;
    std::cout << "Package author: " << authorArg << std::endl;
    std::cout << "Package version: " << majorArg << '.' << minorArg << std::endl;

    packageProcessor.metadata().name          = std::move(nameArg);
    packageProcessor.metadata().author        = std::move(authorArg);
    packageProcessor.metadata().version.major = majorArg;
    packageProcessor.metadata().version.minor = minorArg;

    std::cout << std::endl;

    auto prelastIter = path.begin();
    std::advance(prelastIter, std::distance(path.begin(), path.end()) - 1);

    std::filesystem::path rootPath;

    for (auto iter = path.begin(); iter != prelastIter; ++iter)
    {
        rootPath /= *iter;
    }

    std::cout << "Package root: " << rootPath << std::endl;
    packageProcessor.setPackageRoot(rootPath);
    addFiles(packageProcessor, path, path);

    packageProcessor.write(outputArg);
}

void Operations::unpack(const HG::ToolsCore::CommandLineArguments::ArgumentsMap& args)
{
    if (!args.count(ArgumentsNames::Path) || !args.count(ArgumentsNames::Output))
    {
        throw std::invalid_argument("Wrong arguments");
    }

    std::cout << "Unpacking" << std::endl;

    auto pathArg   = std::get<std::string>(args.at(ArgumentsNames::Path));
    auto outputArg = std::get<std::string>(args.at(ArgumentsNames::Output));

    HG::Tools::PackageProcessor packageProcessor;

    packageProcessor.load(pathArg);

    packageProcessor.unpack(outputArg);
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

    if (!std::filesystem::exists(path))
    {
        throw std::invalid_argument("\"" + path.string() + "\" does not exists.");
    }

    if (std::filesystem::status(path).type() != std::filesystem::file_type::regular)
    {
        throw std::invalid_argument("\"" + path.string() + "\" is not a file.");
    }

    packageProcessor.load(path);

    const auto& metadata = packageProcessor.metadata();

    // Printing metadata
    std::cout << "Metadata: " << std::endl;

    std::cout << "    Name: " << metadata.name << std::endl;
    std::cout << "    Author: " << metadata.author << std::endl;
    std::cout << "    Version: " << metadata.version.major << '.' << metadata.version.minor << std::endl;

    std::cout << std::endl;
    std::cout << "Amount of files: " << packageProcessor.files().size() << std::endl;

    for (const auto& file : packageProcessor.files())
    {
        std::cout << "    " << file.path() << std::endl;
    }
}
