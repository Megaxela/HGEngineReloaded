// GTest
#include <gtest/gtest.h>

#include <fstream>

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
    auto targetPath = std::filesystem::current_path() / "PackageResults";
    auto pathToPackage = targetPath / "sample_package.hgpackage";

    // Writing
    {
        HG::Tools::PackageProcessor packageProcessor;

        packageProcessor.metadata().name = "Sample package name";
        packageProcessor.metadata().author = "Example author";
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
        HG::Tools::PackageProcessor packageProcessor;

        ASSERT_NO_THROW(packageProcessor.load(pathToPackage));

        ASSERT_EQ(packageProcessor.metadata().name, "Sample package name");
        ASSERT_EQ(packageProcessor.metadata().author, "Example author");
        ASSERT_EQ(packageProcessor.metadata().version.major, 0x00112233);
        ASSERT_EQ(packageProcessor.metadata().version.minor, 0xAABBCCDD);

        ASSERT_EQ(packageProcessor.files().size(), 3);

        ASSERT_EQ(packageProcessor.files()[0].type(), HG::Tools::PackageProcessor::File::Type::Package);
        ASSERT_EQ(packageProcessor.files()[1].type(), HG::Tools::PackageProcessor::File::Type::Package);
        ASSERT_EQ(packageProcessor.files()[2].type(), HG::Tools::PackageProcessor::File::Type::Package);

        ASSERT_EQ(packageProcessor.files()[0].path().string(), "empty_root_file.txt");
        ASSERT_EQ(packageProcessor.files()[1].path().string(), "dir1/FILE_WITH_LONG_LINES.txt");
        ASSERT_EQ(packageProcessor.files()[2].path().string(), "dir1/dir2/small_file.txt");

        ASSERT_EQ(packageProcessor.files()[0].compressedOffset(), 400);
        ASSERT_EQ(packageProcessor.files()[1].compressedOffset(), 408);
        ASSERT_EQ(packageProcessor.files()[2].compressedOffset(), 535);

        ASSERT_EQ(packageProcessor.files()[0].compressedSize(), 8);
        ASSERT_EQ(packageProcessor.files()[1].compressedSize(), 127);
        ASSERT_EQ(packageProcessor.files()[2].compressedSize(), 1358);

        ASSERT_NO_THROW(packageProcessor.unpack(targetPath / "UnpackDir"));
    }
}