#pragma once

#include <cstdlib>
#include <type_traits>
#include <vector>
#include <zlib.h>
#include <bytearray.hpp>

namespace HG::Utils::ZLib
{
    namespace
    {
        constexpr std::size_t CHUNK_SIZE = 10 * 1024; // 10kB
    }

    enum CompressionLevel
    {
        Default         = Z_DEFAULT_COMPRESSION,
        NoCompression   = Z_NO_COMPRESSION,
        BestSpeed       = Z_BEST_SPEED,
        BestCompression = Z_BEST_COMPRESSION
    };

    /**
     * @brief Function for deflating some array of data
     * with zlib.
     * @tparam ByteIn Input byte type.
     * @tparam ByteOut Output byte type.
     * @param source Pointer to input byte array.
     * @param sourceSize Size of input byte array.
     * @param target Vector that will be filled with
     * compressed data.
     * @param compression Compression mode.
     */
    template<typename ByteIn, typename ByteOut>
    typename std::enable_if<
        sizeof(ByteIn) == 1 && sizeof(ByteOut) == 1
    >::type Deflate(const ByteIn* source,
                    std::size_t sourceSize,
                    std::vector<ByteOut>& target,
                    CompressionLevel compression = BestCompression)
    {
        uint8_t outBuffer[CHUNK_SIZE];

        z_stream stream = {nullptr};

        if (deflateInit(&stream, compression) != Z_OK)
        {
            throw std::runtime_error("Can't init ZLib deflating");
        }

        int flush;
        do
        {
            stream.avail_in = (uInt) std::min(sourceSize, CHUNK_SIZE);

            flush = sourceSize <= CHUNK_SIZE ? Z_FINISH : Z_NO_FLUSH;

            stream.next_in = (Bytef*) source;

            do
            {
                stream.avail_out = CHUNK_SIZE;
                stream.next_out = outBuffer;

                deflate(&stream, flush);

                auto numberOfBytes = CHUNK_SIZE - stream.avail_out;

                for (auto i = 0; i < numberOfBytes; ++i)
                {
                    target.push_back(static_cast<ByteOut>(outBuffer[i]));
                }
            }
            while (stream.avail_out == 0);

            source += CHUNK_SIZE;
            sourceSize -= CHUNK_SIZE;
        }
        while (flush != Z_FINISH);

        deflateEnd(&stream);
    }

    /**
     * @brief Function for inflating compressed data.
     * @tparam ByteIn Input byte type.
     * @tparam ByteOut Output byte type.
     * @param source Pointer to compressed data.
     * @param sourceSize Amount of compressed data (in bytes)
     * @param target Vector, that will be filled with uncompressed data.
     */
    template< typename ByteIn, typename ByteOut >
    typename std::enable_if<
        sizeof(ByteIn) == 1 && sizeof(ByteOut) == 1,
        bool
    >::type Inflate(const ByteIn* source,
                    std::size_t sourceSize,
                    std::vector<ByteOut>& target)
    {
        uint8_t outBuffer[CHUNK_SIZE];

        z_stream stream;

        memset(&stream, 0, sizeof(z_stream));

        int result;

        if ((result = inflateInit(&stream)) != Z_OK)
        {
            return false;
        }

        do
        {
            stream.avail_in = static_cast<uInt>(sourceSize);

            if (stream.avail_in == 0)
            {
                break;
            }

            stream.next_in = (Bytef*) source;

            do
            {
                stream.avail_out = CHUNK_SIZE;
                stream.next_out = outBuffer;

                result = inflate(&stream, Z_NO_FLUSH);

                if (result == Z_NEED_DICT ||
                    result == Z_DATA_ERROR ||
                    result == Z_MEM_ERROR)
                {
                    inflateEnd(&stream);
                    return false;
                }

                std::size_t numberOfBytes = CHUNK_SIZE - stream.avail_out;

                for (std::size_t i = 0; i < numberOfBytes; ++i)
                {
                    target.push_back((ByteOut) outBuffer[i]);
                }
            }
            while (stream.avail_out == 0);
        } while (result != Z_STREAM_END);

        inflateEnd(&stream);
        return result == Z_STREAM_END;
    }

    template<typename ByteOut, typename ByteIn>
    typename std::enable_if<
        sizeof(ByteOut) == 1 && sizeof(ByteIn) == 1,
        std::vector<ByteOut>
    >::type Inflate(const ByteIn* source,
                    std::size_t sourceSize,
                    bool* result=nullptr)
    {
        std::vector<ByteOut> ret;

        auto realResult = Inflate<ByteIn, ByteOut>(source, sourceSize, ret);

        if (result)
        {
            (*result) = realResult;
        }

        return ret;
    }

    template<typename ByteOut, typename ByteIn>
    typename std::enable_if<
        sizeof(ByteOut) == 1 && sizeof(ByteIn) == 1,
        std::vector<ByteOut>
    >::type Inflate(const std::vector<ByteIn>& data,
                    bool* result=nullptr)
    {
        return Inflate<ByteIn, ByteOut>(data.data(), data.size(), result);
    }
}