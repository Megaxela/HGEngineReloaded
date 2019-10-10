#pragma once

// C++ STL
#include <filesystem>

// ByteArray
#include <bytearray.hpp>
#include <bytearray_view.hpp>

namespace HG::Tools
{
/**
 * @brief Class, that describes object, that can proceed
 * assets package.
     *
 * Package format:
 * File format:
 * #-----#-----#-----#-----#-----#-----#-----#-----#-----#-----#-----#-----#-----#-----#-----#-----#
 * | 0xDE| 0xC0| 0xDE| 0x00|        CRC32          |                   File size                   |
 * #-----#-----#-----#-----#-----#-----#-----#-----#-----#-----#-----#-----#-----#-----#-----#-----#
 * |        Version        | Metadata fields count | File struct fields cnt|       Reserved        |
 * #-----#-----#-----#-----#-----#-----#-----#-----#-----#-----#-----#-----#-----#-----#-----#-----#
 * |                                         Metadata fields...                                    |
 * #-----#-----#-----#-----#-----#-----#-----#-----#-----#-----#-----#-----#-----#-----#-----#-----#
 * |                                          File structure...                                    |
 * #-----#-----#-----#-----#-----#-----#-----#-----#-----#-----#-----#-----#-----#-----#-----#-----#
 * |                                         Compressed data...                                    |
 * #-----#-----#-----#-----#-----#-----#-----#-----#-----#-----#-----#-----#-----#-----#-----#-----#
     *
 * Description:
 * [ 0 -  3] - magic bytex 0xdec0de00
 * [ 4 -  7] - package crc32 (counts from 8th byte)
 * [ 8 - 15] - total package size (uint32_t)
 * [16 - 19] - package packer version (uint32_t)
 * [20 - 23] - amount of metadata fields (uint32_t)
 * [24 - 27] - amount of file structure fields (uint32_t)
 * [28 - 31] - reserved for future use
 * [.. - ..] - metadata fields. aligned by 16 bytes. unknown fields has to be ignored.
 * [.. - ..] - file structure. aligned by 16 bytes. parent structures must be before derived.
 * [.. - ..] - compressed data. zlib compressed data.
     *
 * Metadata field format (each field aligned by 16 bytes):
 * Value type is defined by field type.
 * #-----#-----#-----#-----#-----#-----#-----#-----#-----#-----#-----#-----#-----#-----#-----#-----#
 * |         Type          |     Value length      |                  Value...                     |
 * #-----#-----#-----#-----#-----#-----#-----#-----#-----#-----#-----#-----#-----#-----#-----#-----#
     *
 * Description:
 * [ 0 -  3] - field type (see below for available field types)
 * [ 4 -  7] - length (amount of bytes) of following value (uint32_t)
 * [ 8 - ..] - actual value. if value is not aligned by 16 bytes - zeroes just added to end.
     *
 * Available field types:
 * |   Value   |   Type   | Is Required |                    Description                        |
 * |-----------|----------|-------------|-------------------------------------------------------|
 * | 0x0000001 | string   |  Required   | Package name.                                         |
 * | 0x0000002 | string   |  Required   | Package author.                                       |
 * | 0x0000003 | uint32_t |  Required   | Package major version.                                |
 * | 0x0000004 | uint32_t |  Required   | Package minor version.                                |
 * 
 * File structure field format (each field aligned by 16 bytes):
 * #-----#-----#-----#-----#-----#-----#-----#-----#-----#-----#-----#-----#-----#-----#-----#-----#
 * | Type|    Reserved     |           ID          |       Parent ID       |      Name length      |
 * #-----#-----#-----#-----#-----#-----#-----#-----#-----#-----#-----#-----#-----#-----#-----#-----#
 * |               Compressed data offset          |             Compressed data length            |
 * #-----#-----#-----#-----#-----#-----#-----#-----#-----#-----#-----#-----#-----#-----#-----#-----#
 * |                                             Name...                                           |
 * #-----#-----#-----#-----#-----#-----#-----#-----#-----#-----#-----#-----#-----#-----#-----#-----#
     *
 * Description:
 * [ 0 -  1] - file type (see below for available file types)
 * [ 1 -  3] - reserved for future use
 * [ 4 -  7] - entry id (uint32_t, must be > 1)
 * [ 8 - 11] - parent entry id (uint32_t, if 0 - entry has no parent)
 * [12 - 15] - length (number of bytes) of name (uint32_t) 
 * [16 - 23] - global offset to compressed data. (uint64_t, can be 0 only if it's directory (see type))
 * [24 - 31] - length of compressed data. (uint32_t 64bit, can be 0 only if it's directory (see type))
 * [32 - ..] - entry name (string, utf-8)
 * 
 * Available file types:
 * |   Value  |  Description  |
 * ----------------------------
 * |   0x01   | Directory     |
 * |   0x02   | File          |
     *
 * Available data types:
 * |   Name   |            Description            |
 * ------------------------------------------------
 * | uint32_t | unsigned int, little endian       |
 * | uint64_t | unsigned int 64bit, little endian |
 * | string   | utf-8                             |
 */
class PackageProcessor
{
private:
public:
    /**
     * @brief Structure, that describes
     * package metadata.
     */
    struct Metadata
    {
        std::string name;
        std::string author;
        struct
        {
            uint32_t major = 0;
            uint32_t minor = 0;
        } version;
    };

    class File
    {
        friend class PackageProcessor;

    public:
        /**
         * @brief Enum of possible file
         * types.
         */
        enum class Type
        {
            Filesystem,
            Package
        };

        /**
         * @brief Constructor, that's used by PackageProcessor
         * at file loaded from package.
         */
        File(std::filesystem::path path, Type type, std::size_t offset = 0, std::size_t size = 0);

        /**
         * @brief Method for getting type of file.
         * Filesystem or package located.
         * @return
         */
        Type type() const;

        /**
         * @brief Method for getting file path.
         * @return Path to file.
         */
        std::filesystem::path path() const;

        /**
         * @brief Method for getting size of
         * compressed data. If this method
         * called on `Type::Filesystem` file
         * `std::runtime_error` exception
         * will be thrown.
         * @return Global offset in bytes.
         */
        std::size_t compressedOffset() const;

        /**
         * @brief Method for getting compressed
         * size in bytes. If this method called
         * on `Type::Filesystem` file
         * `std::runtime_error` exception
         * will be thrown.
         * @return Size in bytes.
         */
        std::size_t compressedSize() const;

    private:
        Type m_type;
        std::filesystem::path m_path;
        std::size_t m_compressedOffset;
        std::size_t m_compressedSize;
    };

    /**
     * @brief Constructor.
     */
    PackageProcessor();

    /**
     * @brief Method for clearing package
     * processor from any data.
     */
    void clear();

    /**
     * @brief Method for getting metadata for
     * editing.
     * @return Reference to internal metadata.
     */
    Metadata& metadata();

    /**
     * @brief Method for loading package.
     * `std::invalid_argument` exception will be thrown
     * if there is some error with loading package.
     * This method performs package validating,
     * reading metadata fields and file entries.
     * @param path Path to package.
     */
    void load(std::filesystem::path path);

    /**
     * @brief Method for writing new package 
     * @param path Path to package.
     */
    void write(std::filesystem::path path);

    /**
     * @brief Method for unpacking loaded
     * package to filesystem. If no package was
     * loaded - `std::runtime_error` exception
     * will be thrown.
     * @param path
     */
    void unpack(std::filesystem::path path);

    /**
     * @brief Method for unpacking one file.
     * @param file Reference to internal file.
     */
    void unpack(std::filesystem::path path, const File& file);

    /**
     * @brief Method for setting package root for
     * adding new files. This method call is required
     * before adding new files to package.
     * @param path Full path to package root.
     */
    void setPackageRoot(std::filesystem::path path);

    /**
     * @brief Method for adding new file from
     * filesystem to package. This method
     * can be called only after
     * `PackageProcessor::setPackageRoot` call.
     * Otherwise `std::runtime_error` exception
     * will be thrown. Also it can throw
     * `std::filesystem::filesystem_error` if,
     * any filesystem error is acquired.
     * @param path Full path to file.
     */
    void addFile(std::filesystem::path path);

    /**
     * @brief Method for getting container with
     * files.
     * @return Constant reference to vector with files.
     */
    const std::vector<File>& files() const;

private:
    void internalUnpack(std::ifstream& stream, std::filesystem::path destination, const File& file);

    void
    writeFilesystemFile(std::ofstream& to, const std::filesystem::path& path, std::size_t& offset, std::size_t& size);

    void writePackageFile(std::ofstream& to, std::ifstream& basePackageFile, std::size_t& offset, std::size_t& size);

    uint32_t calculateStreamCRC(std::ifstream& fl);

    void validateCRC(std::ifstream& file, uint32_t crc);

    std::filesystem::path m_pathToOpenedPackage;

    std::filesystem::path m_pathToPackageRoot;

    Metadata m_metadata;

    std::vector<File> m_entries;
};
} // namespace HG::Tools
