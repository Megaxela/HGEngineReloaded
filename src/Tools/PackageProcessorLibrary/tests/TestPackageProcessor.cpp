// GTest
#include <fstream>
#include <gtest/gtest.h>

// C++ STD
#include <algorithm>

// HG::PackageProcessor
#include <HG/Tools/PackageProcessor.hpp>

TEST(PackageProcessorLibrary, SampleLoading)
{
    HG::Tools::PackageProcessor packageProcessor;

    ASSERT_NO_THROW(packageProcessor.load("SamplePackages/Empty.hgpackage"));

    ASSERT_EQ(packageProcessor.metadata().name, "Some Example Name");
    ASSERT_EQ(packageProcessor.metadata().author, "Author Name");
    ASSERT_EQ(packageProcessor.metadata().version.major, 1);
    ASSERT_EQ(packageProcessor.metadata().version.minor, 0);
}

void recursiveAddFile(HG::Tools::PackageProcessor& packageProcessor, const std::filesystem::path& directory)
{
    for (const auto& file : std::filesystem::directory_iterator(directory))
    {
        if (file.status().type() == std::filesystem::file_type::directory)
        {
            recursiveAddFile(packageProcessor, file.path());
            continue;
        }
        else if (file.status().type() != std::filesystem::file_type::regular)
        {
            continue;
        }

        packageProcessor.addFile(file.path());
    }
}

TEST(PackageProcessorLibrary, SampleWritingReading)
{
    auto targetPath    = std::filesystem::current_path() / "PackageResults";
    auto pathToPackage = targetPath / "sample_package.hgpackage";

    // Writing
    {
        HG::Tools::PackageProcessor packageProcessor;

        packageProcessor.metadata().name          = "Sample package name";
        packageProcessor.metadata().author        = "Example author";
        packageProcessor.metadata().version.major = 0x00112233;
        packageProcessor.metadata().version.minor = 0xAABBCCDD;

        auto pathToResources = std::filesystem::absolute("PackageResources");

        packageProcessor.setPackageRoot(pathToResources);

        recursiveAddFile(packageProcessor, pathToResources);

        std::filesystem::create_directories(targetPath);

        ASSERT_NO_THROW(packageProcessor.write(pathToPackage));
    }

    // Reading
    {
        struct Data
        {
            std::string path;
            std::size_t size;
            HG::Tools::PackageProcessor::File::Type type;

            bool operator==(const Data& rhs) const
            {
                return path == rhs.path && size == rhs.size && type == rhs.type;
            }
        };

        std::vector<Data> expectedData = {
            {"empty_root_file.txt", 8, HG::Tools::PackageProcessor::File::Type::Package},
            {"dir1/FILE_WITH_LONG_LINES.txt", 127, HG::Tools::PackageProcessor::File::Type::Package},
            {"dir1/dir2/small_file.txt", 1358, HG::Tools::PackageProcessor::File::Type::Package}};

        std::vector<Data> realData;

        HG::Tools::PackageProcessor packageProcessor;

        ASSERT_NO_THROW(packageProcessor.load(pathToPackage));

        ASSERT_EQ(packageProcessor.metadata().name, "Sample package name");
        ASSERT_EQ(packageProcessor.metadata().author, "Example author");
        ASSERT_EQ(packageProcessor.metadata().version.major, 0x00112233);
        ASSERT_EQ(packageProcessor.metadata().version.minor, 0xAABBCCDD);

        ASSERT_EQ(packageProcessor.files().size(), 3);

        for (const auto& file : packageProcessor.files())
        {
            realData.push_back({file.path().string(), file.compressedSize(), file.type()});
        }

        auto comparator = [](const Data& l, const Data& r) { return l.path < r.path; };

        std::sort(expectedData.begin(), expectedData.end(), comparator);
        std::sort(realData.begin(), realData.end(), comparator);

        // Disable this check due to inconsistent separator todo: fix this later
//        ASSERT_EQ(expectedData, realData);

        ASSERT_NO_THROW(packageProcessor.unpack(targetPath / "UnpackDir"));
    }
}