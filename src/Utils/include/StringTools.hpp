#pragma once
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wstrict-aliasing"

#include <string>
#include <sstream>
#include <vector>

namespace HG::Utils::StringTools
{
    template<typename Out>
    void split(const std::string &s, char delim, Out result)
    {
        std::stringstream ss;
        ss.str(s);
        std::string item;

        while (std::getline(ss, item, delim))
        {
            *(result++) = item;
        }
    }


    std::vector<std::string> split(const std::string &s, char delim);

    template<typename Out>
    void split(const std::wstring &s, wchar_t delim, Out result)
    {
        std::wstringstream ss;
        ss.str(s);
        std::wstring item;

        while (std::getline(ss, item, delim))
        {
            *(result++) = item;
        }
    }

    template<typename T>
    bool endsWith(const T& string, const T& end)
    {
        if (end.size() > string.size())
        {
            return false;
        }

        return std::equal(
            string.begin() + string.size() - end.size(),
            string.end(),
            end.begin()
        );
    }

    template<typename T>
    bool startsWith(const T& lhs, const T& begin)
    {
        if (begin.size() > lhs.size())
        {
            return false;
        }

        return std::equal(
            lhs.begin(),
            lhs.begin() + begin.size(),
            begin.begin()
        );
    }


    std::vector<std::wstring> split(const std::wstring &s, wchar_t delim);

    std::vector<std::wstring> smartSplit(const std::wstring &s, wchar_t delim);

    std::vector<std::string> smartSplit(const std::string& s, char delim);

    std::string toLower(const std::string& s);

    template<typename T>
    typename T::value_type join(T arr, typename T::value_type delim)
    {
        std::basic_stringstream<typename T::value_type::value_type> ss;

        size_t s = 0;

        if (arr.size() > 1)
        {
            for (auto&& item : arr)
            {
                ss << item << delim;

                if (s >= arr.size() - 2)
                {
                    break;
                }

                ++s;
            }
        }

        ss << *(arr.end() - 1);

        return ss.str();
    }
}

#pragma GCC diagnostic pop