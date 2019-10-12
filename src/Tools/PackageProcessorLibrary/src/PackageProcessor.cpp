// C++ STL
#include <fstream>
#include <iostream>
#include <unordered_map>

// HG::Tools
#include <HG/Tools/PackageProcessor.hpp>

// Zlib
#include <zlib.h>

// HG::Utils
#include <HG/Utils/filesystem_ext.hpp>
#include <HG/Utils/zlib.hpp>

// HG::Utils
constexpr const std::uint32_t PACKAGE_MAGIC        = 0xDEC0DE00;
constexpr const std::size_t HEADER_SIZE            = 32; // bytes
constexpr const std::size_t METADATA_HEADER_SIZE   = 8;  // bytes
constexpr const std::size_t FILE_ENTRY_HEADER_SIZE = 32; // bytes
constexpr const std::size_t CRC_BUFFER_SIZE        = 64 * 1024;
constexpr const std::size_t CRC_BEGIN_COUNT_POS    = 8;
constexpr const std::size_t ALIGNMENT              = 16;
constexpr const std::size_t DEFLATE_CHUNK_SIZE     = CRC_BUFFER_SIZE;
constexpr const std::size_t COPY_CHUNK_SIZE        = CRC_BUFFER_SIZE;
constexpr const std::size_t INFLATE_CHUNK_SIZE     = CRC_BUFFER_SIZE;

constexpr const std::uint32_t PACKER_VERSION            = 1;
constexpr const std::uint32_t AMOUNT_OF_METADATA_FIELDS = 4;

template <typename T>
T align(T val, T alignment)
{
    return alignment - (val + alignment) % alignment;
}

namespace MetadataCodes
{
constexpr const std::uint32_t Name   = 0x00000001;
constexpr const std::uint32_t Author = 0x00000002;
constexpr const std::uint32_t Major  = 0x00000003;
constexpr const std::uint32_t Minor  = 0x00000004;
} // namespace MetadataCodes

namespace FileEntryTypes
{
constexpr const std::uint32_t Directory = 0x00000001;
constexpr const std::uint32_t File      = 0x00000002;
} // namespace FileEntryTypes

namespace HG::Tools
{
PackageProcessor::PackageProcessor() : m_pathToOpenedPackage(), m_metadata(), m_entries()
{
}

PackageProcessor::Metadata& PackageProcessor::metadata()
{
    return m_metadata;
}

void PackageProcessor::clear()
{
    m_pathToOpenedPackage.clear();
    m_pathToPackageRoot.clear();
    m_metadata.name.clear();
    m_metadata.author.clear();
    m_metadata.version.major = 0;
    m_metadata.version.minor = 0;
    m_entries.clear();
}

void PackageProcessor::load(std::filesystem::path path)
{
    // Opening file
    std::ifstream file(path.string(), std::ios::binary);

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
    if (buffer.read<std::uint32_t>(pointer, endianness::big) != PACKAGE_MAGIC)
    {
        clear();
        throw std::invalid_argument(std::string("Package has wrong magic bytes"));
    }
    pointer += 4;

    // Saving crc32 for future use
    auto crc32 = buffer.read<std::uint32_t>(pointer, endianness::big);
    pointer += 4;

    // Perform validating here
    validateCRC(file, crc32);

    // Getting total file size
    auto fileSize = buffer.read<std::uint64_t>(pointer, endianness::big);
    pointer += 8;

    // Getting version of packer
    auto version = buffer.read<std::uint32_t>(pointer, endianness::big);
    pointer += 4;

    if (version > PACKER_VERSION)
    {
        clear();
        throw std::invalid_argument("Package was packed with newer version of packer (current: " +
                                    std::to_string(PACKER_VERSION) + ", package: " + std::to_string(version));
    }

    // Getting number of metadata fields
    auto numberOfMetadataFields = buffer.read<std::uint32_t>(pointer, endianness::big);
    pointer += 4;

    // Getting number of file fields
    auto numberOfFileFields = buffer.read<std::uint32_t>(pointer, endianness::big);
    pointer += 4;

    // Skip 4 reserved bytes
    pointer += 4;

    // Reading metadata fields
    // todo c++20: replace with view
    for (std::uint32_t index = 0; index < numberOfMetadataFields; ++index)
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
        auto type = buffer.read<std::uint32_t>(pointer, endianness::big);
        pointer += 4;

        // Reading value length
        auto valueLength = buffer.read<std::uint32_t>(pointer, endianness::big);
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
            m_metadata.version.major = buffer.read<std::uint32_t>(0, endianness::big);
            break;
        case MetadataCodes::Minor:
            m_metadata.version.minor = buffer.read<std::uint32_t>(0, endianness::big);
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

    std::unordered_map<std::uint32_t, // ID
                       FileEntry>
        entries;

    // Reading file structures fields
    for (std::uint32_t index = 0; index < numberOfFileFields; ++index)
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
        auto type = buffer.read<std::uint8_t>(pointer, endianness::big);
        pointer += 1;

        // Skipping 3 reserved bytes
        pointer += 3;

        // Reading entry ID
        auto id = buffer.read<std::uint32_t>(pointer, endianness::big);
        pointer += 4;

        // Reading parent ID
        auto parentId = buffer.read<std::uint32_t>(pointer, endianness::big);
        pointer += 4;

        // Name length
        auto nameLength = buffer.read<std::uint32_t>(pointer, endianness::big);
        pointer += 4;

        auto dataOffset = buffer.read<std::uint64_t>(pointer, endianness::big);
        pointer += 8;

        auto dataSize = buffer.read<std::uint64_t>(pointer, endianness::big);
        pointer += 8;

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

        std::filesystem::path entryPath;

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
            m_entries.emplace_back(entryPath, File::Type::Package, dataOffset, dataSize);
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

void PackageProcessor::write(std::filesystem::path path)
{
    std::ofstream file(path.string(), std::ios::binary);

    if (!file.is_open())
    {
        // todo: add special cross platform error getter
        throw std::invalid_argument(std::string("Can't open file: ") + strerror(errno));
    }

    bytearray<> buffer;

    std::size_t fileSize = 0;

    // Build header
    buffer.reserve(HEADER_SIZE);

    // Pushing magic
    buffer.push_back<std::uint32_t>(PACKAGE_MAGIC, endianness::big);

    // Pushing dummy crc32
    buffer.push_back<std::uint32_t>(0, endianness::big);

    // Pushing dummy result file size
    buffer.push_back<std::uint64_t>(0, endianness::big);

    // Pushing packer version
    buffer.push_back<std::uint32_t>(PACKER_VERSION, endianness::big);

    // Pushing amount of metadata fields
    buffer.push_back<std::uint32_t>(AMOUNT_OF_METADATA_FIELDS, endianness::big);

    struct WriteEntryInfo
    {
        std::uint32_t id                = 0;
        bool wasWritten                 = false;
        std::size_t offsetToOffsetField = 0;
    };

    std::unordered_map<std::filesystem::path, WriteEntryInfo> entries;

    std::uint32_t idCounter = 1;

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
    buffer.push_back<std::uint32_t>(static_cast<std::uint32_t>(entries.size()), endianness::big);

    // Pushing empty reserved field
    buffer.push_back<std::uint32_t>(0, endianness::big);

    assert(buffer.size() == HEADER_SIZE);

    // Writing header to file
    file.write(reinterpret_cast<const char*>(buffer.container().data()), buffer.size());

    // todo: make some more convenient algorithm
    // Writing name metadata
    buffer.clear();

    // Pushing type
    buffer.push_back<std::uint32_t>(MetadataCodes::Name, endianness::big);

    // Pushing value length
    buffer.push_back<std::uint32_t>(static_cast<std::uint32_t>(m_metadata.name.size()), endianness::big);

    // Pushing actual name
    buffer.push_back_multiple(m_metadata.name.begin(), m_metadata.name.end());

    // Pushing 16 bytes alignment
    buffer.push_back_multiple<std::uint8_t>(0, align(buffer.size(), ALIGNMENT));

    // Write metadata to file
    file.write(reinterpret_cast<const char*>(buffer.container().data()), buffer.size());

    // Writing name metadata
    buffer.clear();

    // Pushing type
    buffer.push_back<std::uint32_t>(MetadataCodes::Author, endianness::big);

    // Pushing value length
    buffer.push_back<std::uint32_t>(static_cast<std::uint32_t>(m_metadata.author.size()), endianness::big);

    // Pushing actual name
    buffer.push_back_multiple(m_metadata.author.begin(), m_metadata.author.end());

    // Pushing 16 bytes alignment
    buffer.push_back_multiple<std::uint8_t>(0, align(buffer.size(), ALIGNMENT));

    // Write metadata to file
    file.write(reinterpret_cast<const char*>(buffer.container().data()), buffer.size());

    // Writing name metadata
    buffer.clear();

    // Pushing type
    buffer.push_back<std::uint32_t>(MetadataCodes::Major, endianness::big);

    // Pushing value length
    buffer.push_back<std::uint32_t>(4, endianness::big);

    // Pushing actual name
    buffer.push_back<std::uint32_t>(m_metadata.version.major, endianness::big);

    // Pushing 16 bytes alignment
    buffer.push_back_multiple<std::uint8_t>(0, align(buffer.size(), ALIGNMENT));

    // Write metadata to file
    file.write(reinterpret_cast<const char*>(buffer.container().data()), buffer.size());

    // Pushing 16 bytes alignment
    buffer.push_back_multiple<std::uint8_t>(0, align(buffer.size(), ALIGNMENT));

    // Writing name metadata
    buffer.clear();

    // Pushing type
    buffer.push_back<std::uint32_t>(MetadataCodes::Minor, endianness::big);

    // Pushing value length
    buffer.push_back<std::uint32_t>(4, endianness::big);

    // Pushing actual name
    buffer.push_back<std::uint32_t>(m_metadata.version.minor, endianness::big);

    // Pushing 16 bytes alignment
    buffer.push_back_multiple<std::uint8_t>(0, align(buffer.size(), ALIGNMENT));

    // Write metadata to file
    file.write(reinterpret_cast<const char*>(buffer.container().data()), buffer.size());

    // Writing file structures
    for (const auto& entry : m_entries)
    {
        // Cutting path and filename
        auto pathToEntry   = entry.path().remove_filename();
        auto entryFilename = entry.path().filename();

        std::filesystem::path bufferPath;
        std::uint32_t parentId = 0;
        for (const auto& pathPart : pathToEntry)
        {
            if (pathPart.empty())
            {
                continue;
            }

            bufferPath /= pathPart;

            auto writeInfoIter = entries.find(bufferPath);

            // Not checking for existing, cause of previous filler
            if (writeInfoIter->second.wasWritten)
            {
                // Saving current path part as parent id
                parentId = writeInfoIter->second.id;
                continue;
            }

            // Clearing buffer
            buffer.clear();

            // Pushing type
            buffer.push_back<std::uint8_t>(FileEntryTypes::Directory, endianness::big);

            // Pushing 3 reserved bytes
            buffer.push_back_multiple<std::uint8_t>(0, 3, endianness::big);

            // Pushing ID
            buffer.push_back<std::uint32_t>(writeInfoIter->second.id, endianness::big);

            // Pushing parent ID
            buffer.push_back<std::uint32_t>(parentId, endianness::big);

            // Pushing name size
            buffer.push_back<std::uint32_t>(static_cast<std::uint32_t>(pathPart.string().size()), endianness::big);

            // Pushing dummy data offset
            buffer.push_back<std::uint64_t>(0, endianness::big);

            // Pushing dummy compressed data size
            buffer.push_back<std::uint64_t>(0, endianness::big);

            // Pushing name
            auto pathString = pathPart.string();
            buffer.push_back_multiple(pathString.begin(), pathString.end());

            // Pushing alignment
            buffer.push_back_multiple<std::uint8_t>(0, align(buffer.size(), ALIGNMENT));

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
        buffer.push_back<std::uint8_t>(FileEntryTypes::File, endianness::big);

        // Pushing 3 reserved bytes
        buffer.push_back_multiple<std::uint8_t>(0, 3, endianness::big);

        // Pushing ID
        buffer.push_back<std::uint32_t>(writeInfoIter->second.id, endianness::big);

        // Pushing parent ID
        buffer.push_back<std::uint32_t>(parentId);

        // Pushing name length
        buffer.push_back<std::uint32_t>(static_cast<std::uint32_t>(entryFilename.string().size()), endianness::big);

        writeInfoIter->second.offsetToOffsetField = std::size_t(file.tellp()) + FILE_ENTRY_HEADER_SIZE / 2;

        // Pushing dummy data offset
        buffer.push_back<std::uint64_t>(0, endianness::big);

        // Pushing dummy data size
        buffer.push_back<std::uint64_t>(0, endianness::big);

        // Pushing name
        auto filenameString = entryFilename.string();
        buffer.push_back_multiple(filenameString.begin(), filenameString.end());

        // Pushing alignment
        buffer.push_back_multiple<std::uint8_t>(0, align(buffer.size(), ALIGNMENT));

        // Writing file entry to file
        file.write(reinterpret_cast<const char*>(buffer.container().data()), buffer.size());

        writeInfoIter->second.wasWritten = true;
    }

    std::ifstream basePackageFile;

    for (auto& entry : m_entries)
    {
        std::size_t offset;
        std::size_t size;

        switch (entry.type())
        {
        case File::Type::Filesystem:
            writeFilesystemFile(file, m_pathToPackageRoot / entry.path(), offset, size);
            break;
        case File::Type::Package:
            offset = entry.compressedOffset();
            size   = entry.compressedSize();
            writePackageFile(file, basePackageFile, offset, size);
            break;
        }

        auto currPosition = file.tellp();

        auto iter = entries.find(entry.path());

        file.seekp(iter->second.offsetToOffsetField);

        // Rewriting offset/size data
        buffer.clear();
        buffer.push_back<std::uint64_t>(offset, endianness::big);
        buffer.push_back<std::uint64_t>(size, endianness::big);

        file.write(reinterpret_cast<const char*>(buffer.container().data()), buffer.size());

        file.seekp(currPosition);
    }

    basePackageFile.close();

    file.close();

    basePackageFile.open(path.string(), std::ios::binary);

    if (!basePackageFile.is_open())
    {
        throw std::runtime_error("Can't open create package to calculate crc32: " + std::string(strerror(errno)));
    }

    basePackageFile.seekg(8, std::ios::beg);

    auto crc = calculateStreamCRC(basePackageFile);

    basePackageFile.close();

    file.open(path.string(), std::ios::binary | std::ios::in | std::ios::out | std::ios::ate);

    auto fileEnd = file.tellp();

    if (!file.is_open())
    {
        // todo: add special cross platform error getter
        throw std::invalid_argument(std::string("Can't open file for writing CRC32: ") + strerror(errno));
    }

    buffer.clear();
    buffer.push_back<std::uint32_t>(crc, endianness::big);

    file.seekp(4);
    file.write(reinterpret_cast<const char*>(buffer.container().data()), buffer.size());
    file.seekp(fileEnd);
}

void PackageProcessor::unpack(std::filesystem::path path)
{
    if (m_pathToOpenedPackage.empty())
    {
        throw std::runtime_error("Package is not opened");
    }

    std::ifstream file(m_pathToOpenedPackage.string(), std::ios::binary);

    if (!file.is_open())
    {
        throw std::runtime_error("Can't open package file for inflating");
    }

    for (const auto& fileEntry : m_entries)
    {
        internalUnpack(file, path / fileEntry.path(), fileEntry);
    }
}

void PackageProcessor::unpack(std::filesystem::path path, const PackageProcessor::File& file)
{
    if (m_pathToOpenedPackage.empty())
    {
        throw std::runtime_error("Package is not opened");
    }

    std::ifstream inputFile(m_pathToOpenedPackage.string(), std::ios::binary);

    if (!inputFile.is_open())
    {
        throw std::runtime_error("Can't open package file for infalting");
    }

    internalUnpack(inputFile, std::move(path), file);
}

void PackageProcessor::internalUnpack(std::ifstream& stream,
                                      std::filesystem::path destination,
                                      const PackageProcessor::File& file)
{
    auto filename = destination.filename();
    auto path     = destination.parent_path();

    std::error_code ec;

    std::filesystem::create_directories(path, ec);

    std::ofstream output(destination.string(), std::ios::binary);

    if (!output.is_open())
    {
        throw std::invalid_argument("Can't open file for inflating \"" + destination.string() + "\"");
    }

    stream.seekg(file.compressedOffset());

    HG::Utils::ZLib::InflateStreamToStream(stream, output, INFLATE_CHUNK_SIZE);
}

void PackageProcessor::validateCRC(std::ifstream& file, std::uint32_t crc)
{
    // Validating
    if (crc != calculateStreamCRC(file))
    {
        throw std::invalid_argument("Invalid control sum.");
    }
}

std::uint32_t PackageProcessor::calculateStreamCRC(std::ifstream& file)
{
    // Read whole file with huge chunks. Then return carriage
    auto calculatedCRC = crc32(0L, nullptr, 0);

    char buffer[CRC_BUFFER_SIZE];

    auto lastCarriage = file.tellg();
    file.seekg(CRC_BEGIN_COUNT_POS);

    long read = 0;
    while ((read = file.readsome(buffer, CRC_BUFFER_SIZE)))
    {
        calculatedCRC = crc32(calculatedCRC, (Bytef*)buffer, static_cast<uInt>(read));
    }

    file.seekg(lastCarriage);

    return static_cast<std::uint32_t>(calculatedCRC);
}

void PackageProcessor::writeFilesystemFile(std::ofstream& to,
                                           const std::filesystem::path& path,
                                           std::size_t& offset,
                                           std::size_t& size)
{
    offset = static_cast<std::size_t>(to.tellp());

    std::ifstream file(path.string());

    if (!file.is_open())
    {
        throw std::runtime_error("Can't open \"" + path.string() + "\": " + strerror(errno));
    }

    size = HG::Utils::ZLib::DeflateStreamToStream(file, to, DEFLATE_CHUNK_SIZE);
}

void PackageProcessor::writePackageFile(std::ofstream& to,
                                        std::ifstream& basePackageFile,
                                        std::size_t& offset,
                                        std::size_t& size)
{
    if (!basePackageFile.is_open())
    {
        basePackageFile.open(m_pathToOpenedPackage.string(), std::ios::binary);

        if (!basePackageFile.is_open())
        {
            throw std::runtime_error("Can't open package to copy data");
        }
    }

    basePackageFile.seekg(offset, std::ios::beg);
    offset = static_cast<std::size_t>(to.tellp());

    char buffer[COPY_CHUNK_SIZE];

    std::size_t written = 0;

    while (written >= size)
    {
        auto requiredRead = std::min(COPY_CHUNK_SIZE, size - written);

        auto read = basePackageFile.readsome(buffer, requiredRead);

        if (read != requiredRead)
        {
            throw std::runtime_error("Can't read expected amount of data");
        }

        to.write(buffer, read);
        written += read;
    }
}

void PackageProcessor::setPackageRoot(std::filesystem::path path)
{
    m_pathToPackageRoot = std::move(path);
}

void PackageProcessor::addFile(std::filesystem::path path)
{
    if (m_pathToPackageRoot.empty())
    {
        throw std::runtime_error("`setPackageRoot` has to called before `addFile`");
    }

    auto iter = std::search(path.begin(), path.end(), m_pathToPackageRoot.begin(), m_pathToPackageRoot.end());

    if (iter != path.begin())
    {
        throw std::runtime_error("Package root has to be begin of file");
    }

    m_entries.emplace_back(std::filesystem::relative(path, m_pathToPackageRoot), File::Type::Filesystem);
}

PackageProcessor::File::File(std::filesystem::path path,
                             PackageProcessor::File::Type type,
                             size_t offset,
                             size_t size) :
    m_path(std::move(path)),
    m_type(type),
    m_compressedOffset(offset),
    m_compressedSize(size)
{
}

PackageProcessor::File::Type PackageProcessor::File::type() const
{
    return m_type;
}

std::filesystem::path PackageProcessor::File::path() const
{
    return m_path;
}

std::size_t PackageProcessor::File::compressedOffset() const
{
    return m_compressedOffset;
}

std::size_t PackageProcessor::File::compressedSize() const
{
    return m_compressedSize;
}

const std::vector<PackageProcessor::File>& PackageProcessor::files() const
{
    return m_entries;
}
} // namespace HG::Tools
