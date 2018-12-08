// C++ STL
#include <fstream>
#include <unordered_map>

// HG::Tools
#include <HG/Tools/PackageProcessor.hpp>

#include <zlib.h>

// HG::Utils
constexpr const uint32_t PACKAGE_MAGIC = 0xDEC0DE00;
constexpr const std::size_t HEADER_SIZE = 32; // bytes
constexpr const std::size_t METADATA_HEADER_SIZE = 8; // bytes
constexpr const std::size_t FILE_ENTRY_HEADER_SIZE = 32; // bytes
constexpr const std::size_t CRC_BUFFER_SIZE = 64 * 1024;
constexpr const std::size_t CRC_BEGIN_COUNT_POS = 8;
constexpr const std::size_t ALIGNMENT = 16;

constexpr const uint32_t PACKER_VERSION = 1;
constexpr const uint32_t AMOUNT_OF_METADATA_FIELDS = 4;

template<typename T>
T align(T val, T alignment)
{
    return alignment - (val + alignment) % alignment;
}

namespace MetadataCodes
{
    constexpr const uint32_t Name   = 0x00000001;
    constexpr const uint32_t Author = 0x00000002;
    constexpr const uint32_t Major  = 0x00000003;
    constexpr const uint32_t Minor  = 0x00000004;
}

namespace FileEntryTypes
{
    constexpr const uint32_t Directory = 0x00000001;
    constexpr const uint32_t File      = 0x00000002;
}

HG::Tools::PackageProcessor::PackageProcessor() :
    m_pathToOpenedPackage(),
    m_metadata(),
    m_entries()
{

}

HG::Tools::PackageProcessor::Metadata& HG::Tools::PackageProcessor::metadata()
{
    return m_metadata;
}

void HG::Tools::PackageProcessor::clear()
{
    m_pathToOpenedPackage.clear();
    m_pathToPackageRoot.clear();
    m_metadata.name.clear();
    m_metadata.author.clear();
    m_metadata.version.major = 0;
    m_metadata.version.minor = 0;
    m_entries.clear();
}

void HG::Tools::PackageProcessor::load(std::filesystem::path path)
{
    // Opening file
    std::ifstream file(path, std::ios::binary);

    if (!file.is_open())
    {
        clear();
        // todo: add special cross platform error getter
        throw std::invalid_argument(std::string("Can't open file: ") + strerror(errno));
    }

    m_pathToOpenedPackage = std::move(path);

    bytearray<> buffer;
    buffer.container().resize(HEADER_SIZE);

    // Reading package header
    auto actuallyRead = file.readsome(reinterpret_cast<char*>(buffer.container().data()), HEADER_SIZE);

    if (actuallyRead != HEADER_SIZE)
    {
        clear();
        throw std::invalid_argument(std::string("File is smaller than package header"));
    }

    std::size_t pointer = 0;

    // Validating
    if (buffer.read<uint32_t>(pointer, endianness::little) != PACKAGE_MAGIC)
    {
        clear();
        throw std::invalid_argument(std::string("Package has wrong magic bytes"));
    }
    pointer += 4;

    // Saving crc32 for future use
    auto crc32 = buffer.read<uint32_t>(pointer, endianness::little);
    pointer += 4;

    // Perform validating here
    validateCRC(file, crc32);

    // Getting total file size
    auto fileSize = buffer.read<uint64_t>(pointer, endianness::little);
    pointer += 8;

    // Getting version of packer
    auto version = buffer.read<uint32_t>(pointer, endianness::little);
    pointer += 4;

    if (version > PACKER_VERSION)
    {
        clear();
        throw std::invalid_argument(
            "Package was packed with newer version of packer (current: " +
            std::to_string(PACKER_VERSION) +
            ", package: " +
            std::to_string(version)
        );
    }

    // Getting number of metadata fields
    auto numberOfMetadataFields = buffer.read<uint32_t>(pointer, endianness::little);
    pointer += 4;

    // Getting number of file fields
    auto numberOfFileFields = buffer.read<uint32_t>(pointer, endianness::little);
    pointer += 4;

    // Skip 4 reserved bytes
    pointer += 4;

    // Reading metadata fields
    // todo c++20: replace with view
    for (uint32_t index = 0; index < numberOfMetadataFields; ++index)
    {
        // Zeroing pointer, because of rewriting buffer
        pointer = 0;

        // Reading metadata header to buffer
        buffer.container().resize(METADATA_HEADER_SIZE);

        actuallyRead = file.readsome(reinterpret_cast<char*>(buffer.container().data()), METADATA_HEADER_SIZE);

        if (actuallyRead != METADATA_HEADER_SIZE)
        {
            clear();
            throw std::invalid_argument("Metadata header read smaller than expected");
        }

        // Reading field type
        auto type = buffer.read<uint32_t>(pointer, endianness::little);
        pointer += 4;

        // Reading value length
        auto valueLength = buffer.read<uint32_t>(pointer, endianness::little);
        pointer += 4;

        // Reading metadata value to buffer
        buffer.container().resize(valueLength);

        actuallyRead = file.readsome(reinterpret_cast<char*>(buffer.container().data()), valueLength);

        if (actuallyRead != valueLength)
        {
            clear();
            throw std::invalid_argument("Metadata value read smaller than expected");
        }

        // Parsing into values
        switch (type)
        {
        case MetadataCodes::Name:
            m_metadata.name.resize(valueLength);
            std::memcpy(m_metadata.name.data(), buffer.container().data(), valueLength);
            break;
        case MetadataCodes::Author:
            m_metadata.author.resize(valueLength);
            std::memcpy(m_metadata.author.data(), buffer.container().data(), valueLength);
            break;
        case MetadataCodes::Major:
            m_metadata.version.major = buffer.read<uint32_t>(pointer, endianness::little);
            break;
        case MetadataCodes::Minor:
            m_metadata.version.minor = buffer.read<uint32_t>(pointer, endianness::little);
            break;
        default:
            // Just skip unknown metadata headers
            break;
        }

        pointer += valueLength;
        auto alignment = align(pointer, ALIGNMENT); // Alignment

        // Moving to alignment
        file.seekg(alignment, std::ios::cur);
    }

    struct FileEntry
    {
        std::string name;
        std::filesystem::path path;
    };

    std::unordered_map<
        uint32_t, // ID
        FileEntry
    > entries;

    // Reading file structures fields
    for (uint32_t index = 0; index < numberOfFileFields; ++index)
    {
        pointer = 0;

        // Reading file header
        buffer.container().resize(FILE_ENTRY_HEADER_SIZE);

        actuallyRead = file.readsome(reinterpret_cast<char*>(buffer.container().data()), FILE_ENTRY_HEADER_SIZE);

        if (actuallyRead != FILE_ENTRY_HEADER_SIZE)
        {
            clear();
            throw std::invalid_argument("File entry header read smaller than expected");
        }

        // Reading type
        auto type = buffer.read<uint8_t>(pointer, endianness::little);
        pointer += 1;

        // Skipping 3 reserved bytes
        pointer += 3;

        // Reading entry ID
        auto id = buffer.read<uint32_t>(pointer, endianness::little);
        pointer += 4;

        // Reading parent ID
        auto parentId = buffer.read<uint32_t>(pointer, endianness::little);
        pointer += 4;

        // Name length
        auto nameLength = buffer.read<uint8_t>(pointer, endianness::little);
        pointer += 4;

        // Reading name
        buffer.container().resize(nameLength);

        std::string stringName;
        stringName.resize(nameLength);

        actuallyRead = file.readsome(stringName.data(), nameLength);

        if (actuallyRead != nameLength)
        {
            clear();
            throw std::invalid_argument("File entry name read smaller than expected");
        }

        std::filesystem::path entryPath(".");

        if (parentId != 0)
        {
            auto parentEntryIterator = entries.find(parentId);

            if (parentEntryIterator == entries.end())
            {
                clear();
                throw std::invalid_argument("Wrong file entries sequence");
            }

            entryPath /= parentEntryIterator->second.path;
        }

        entryPath /= stringName;

        switch (type)
        {
        case FileEntryTypes::File:
            // todo: Add to loaded files data
            break;
        case FileEntryTypes::Directory:
            entries.insert({id, {std::move(stringName), std::move(entryPath)}});
            break;
        default:
            throw std::invalid_argument("Unknown file entry type found");
        }

        pointer += nameLength;
        auto alignment = align(pointer, ALIGNMENT);

        // Moving to alignment
        file.seekg(alignment, std::ios::cur);
    }
}

void HG::Tools::PackageProcessor::write(std::filesystem::path path)
{
    std::ofstream file(path, std::ios::binary);

    if (!file.is_open())
    {
        // todo: add special cross platform error getter
        throw std::invalid_argument(std::string("Can't open file: ") + strerror(errno));
    }

    bytearray<> buffer;

    std::size_t fileSize = 0;

    // Build header
    buffer.reserve(METADATA_HEADER_SIZE);

    // Pushing magic
    buffer.push_back<uint32_t>(PACKAGE_MAGIC, endianness::little);

    // Pushing dummy crc32
    buffer.push_back<uint32_t>(0, endianness::little);

    // Pushing dummy result file size
    buffer.push_back<uint64_t>(0, endianness::little);

    // Pushing packer version
    buffer.push_back<uint32_t>(PACKER_VERSION, endianness::little);

    // Pushing amount of metadata fields
    buffer.push_back<uint32_t>(AMOUNT_OF_METADATA_FIELDS, endianness::little);

    struct WriteEntryInfo
    {
        uint32_t id = 0;
        bool wasWritten = false;
    };

    std::unordered_map<
        std::filesystem::path,
        WriteEntryInfo
    > entries;

    uint32_t idCounter = 1;

    // Counting amount of file entries
    for (const auto& entry : m_entries)
    {
        std::filesystem::path bufferPath;

        for (const auto& pathPart : entry.path())
        {
            bufferPath /= pathPart;

            auto entryIterator = entries.find(bufferPath);

            if (entryIterator == entries.end())
            {
                entries.insert({bufferPath, {idCounter++, false}});
                continue;
            }
        }
    }

    // Pushing amount of file entries fields
    buffer.push_back<uint32_t>(static_cast<uint32_t>(entries.size()), endianness::little);

    // Pushing empty reserved field
    buffer.push_back<uint32_t>(0, endianness::little);

    assert(buffer.size() == METADATA_HEADER_SIZE);

    // Writing header to file
    file.write(reinterpret_cast<const char*>(buffer.container().data()), METADATA_HEADER_SIZE);

    // todo: make some more convenient algorithm
    // Writing name metadata
    buffer.clear();

    // Pushing type
    buffer.push_back<uint32_t>(MetadataCodes::Name, endianness::little);

    // Pushing value length
    buffer.push_back<uint32_t>(static_cast<uint32_t>(m_metadata.name.size()), endianness::little);

    // Pushing actual name
    buffer.push_back_multiple(m_metadata.name.begin(), m_metadata.name.end());

    // Write metadata to file
    file.write(reinterpret_cast<const char*>(buffer.container().data()), buffer.size());

    // Writing name metadata
    buffer.clear();

    // Pushing type
    buffer.push_back<uint32_t>(MetadataCodes::Author, endianness::little);

    // Pushing value length
    buffer.push_back<uint32_t>(static_cast<uint32_t>(m_metadata.author.size()), endianness::little);

    // Pushing actual name
    buffer.push_back_multiple(m_metadata.author.begin(), m_metadata.author.end());

    // Write metadata to file
    file.write(reinterpret_cast<const char*>(buffer.container().data()), buffer.size());

    // Writing name metadata
    buffer.clear();

    // Pushing type
    buffer.push_back<uint32_t>(MetadataCodes::Major, endianness::little);

    // Pushing value length
    buffer.push_back<uint32_t>(4, endianness::little);

    // Pushing actual name
    buffer.push_back<uint32_t>(m_metadata.version.major, endianness::little);

    // Write metadata to file
    file.write(reinterpret_cast<const char*>(buffer.container().data()), buffer.size());

    // Writing name metadata
    buffer.clear();

    // Pushing type
    buffer.push_back<uint32_t>(MetadataCodes::Minor, endianness::little);

    // Pushing value length
    buffer.push_back<uint32_t>(4, endianness::little);

    // Pushing actual name
    buffer.push_back<uint32_t>(m_metadata.version.minor, endianness::little);

    // Write metadata to file
    file.write(reinterpret_cast<const char*>(buffer.container().data()), buffer.size());

    // Writing file structures
    for (const auto& entry : m_entries)
    {
        // Cutting path and filename
        auto pathToEntry = entry.path().remove_filename();
        auto entryFilename = entry.path().filename();

        std::filesystem::path bufferPath;
        uint32_t parentId = 0;
        for (const auto& pathPart : pathToEntry)
        {
            bufferPath /= pathPart;

            auto writeInfoIter = entries.find(bufferPath);

            // Not checking for existing, cause of previous filler
            if (writeInfoIter->second.wasWritten)
            {
                continue;
            }

            // Clearing buffer
            buffer.clear();

            // Pushing type
            buffer.push_back<uint8_t>(FileEntryTypes::Directory, endianness::little);

            // Pushing 3 reserved bytes
            buffer.push_back_multiple<uint8_t>(0, 3, endianness::little);

            // Pushing ID
            buffer.push_back<uint32_t>(writeInfoIter->second.id, endianness::little);

            // Pushing parent ID
            buffer.push_back<uint32_t>(parentId, endianness::little);

            // Pushing name size
            buffer.push_back<uint32_t>(static_cast<uint32_t>(pathPart.string().size()), endianness::little);

            // Pushing dummy data offset
            buffer.push_back<uint64_t>(0, endianness::little);

            // Pushing dummy compressed data size
            buffer.push_back<uint64_t>(0, endianness::little);

            // Pushing name
            auto pathString = pathPart.string();
            buffer.push_back_multiple(pathString.begin(), pathString.end());

            // Pushing alignment
            buffer.push_back_multiple(0, align(buffer.size(), ALIGNMENT));

            // Writing file entry to file
            file.write(reinterpret_cast<const char*>(buffer.container().data()), buffer.size());

            writeInfoIter->second.wasWritten = true;

            // Saving current path part as parent id
            parentId = writeInfoIter->second.id;
        }

        bufferPath /= entryFilename;

        auto writeInfoIter = entries.find(bufferPath);

        // Not checking for existance, cause of previous filler

        // If this condition will be true - something
        // really strange is going on
        if (writeInfoIter->second.wasWritten)
        {
            continue;
        }

        // Writing file
        buffer.clear();

        // Pushing type
        buffer.push_back<uint8_t>(FileEntryTypes::File, endianness::little);

        // Pushing 3 reserved bytes
        buffer.push_back_multiple<uint8_t>(0, 3, endianness::little);

        // Pushing ID
        buffer.push_back<uint32_t>(writeInfoIter->second.id, endianness::little);

        // Pushing parent ID
        buffer.push_back<uint32_t>(parentId);

        // Pushing name length
        buffer.push_back<uint32_t>(static_cast<uint32_t>(entryFilename.string().size()), endianness::little);

        // Pushing dummy data offset
        buffer.push_back<uint64_t>(0, endianness::little);

        // Pushing dummy data size
        buffer.push_back<uint64_t>(0, endianness::little);

        // Pushing name
        auto filenameString = entryFilename.string();
        buffer.push_back_multiple(filenameString.begin(), filenameString.end());

        // Pushing alignment
        buffer.push_back_multiple(0, align(buffer.size(), ALIGNMENT));

        // Writing file entry to file
        file.write(reinterpret_cast<const char*>(buffer.container().data()), buffer.size());

        writeInfoIter->second.wasWritten = true;
    }
}

void HG::Tools::PackageProcessor::validateCRC(std::ifstream& file, uint32_t crc)
{
    // Read whole file with huge chunks. Then return carriage
    auto calculatedCRC = crc32(0L, nullptr, 0);

    char buffer[CRC_BUFFER_SIZE];

    auto lastCarriage = file.tellg();
    file.seekg(CRC_BEGIN_COUNT_POS);

    long read = 0;
    while ((read = file.readsome(buffer, CRC_BUFFER_SIZE)))
    {
        calculatedCRC = crc32(calculatedCRC, (Bytef*) buffer, static_cast<uInt>(read));
    }

    file.seekg(lastCarriage);

    // Validating
    if (crc != calculatedCRC)
    {
        throw std::invalid_argument("Invalid control sum.");
    }
}
