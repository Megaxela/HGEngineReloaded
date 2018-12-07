#pragma once

#include <filesystem>

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
     * | Type|    Reserved     |           ID          |       Parent ID       |       Name length     |
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
    public:

        struct Metadata
        {
            std::string name;
            std::string author;
            struct
            {
                uint32_t major;
                uint32_t minor;
            } version;
        };

        /**
         * @brief Constructor.
         */
        PackageProcessor();

        /**
         * @brief Method for loading package.
         * `std::invalid_argument` exception will be thrown
         * if there is some error with loading package.
         * @param path Path to package.
         */
        void load(std::filesystem::path path);

        /**
         * @brief Method for writing new package 
         * @param path
         */
        void write(std::filesystem::path path);

    private:

        Metadata m_metadata;

    };
}