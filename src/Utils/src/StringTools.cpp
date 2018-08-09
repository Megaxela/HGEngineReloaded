#include "StringTools.hpp"
#include <algorithm>

std::vector<std::string> HG::Utils::StringTools::split(const std::string &s, char delim)
{
    std::vector<std::string> elems;
    split(s, delim, std::back_inserter(elems));
    return elems;
}

std::vector<std::wstring> HG::Utils::StringTools::split(const std::wstring &s, wchar_t delim)
{
    std::vector<std::wstring> elems;
    split(s, delim, std::back_inserter(elems));
    return elems;
}

std::vector<std::wstring> HG::Utils::StringTools::smartSplit(const std::wstring& s, wchar_t delim)
{
    std::vector<std::wstring> elems;
    std::wstringstream ss;

    bool isQuoted = false;
    bool isScreening = false;
    for (auto&& character : s)
    {
        if (character == delim &&
            !ss.str().empty() &&
            !isQuoted)
        {
            elems.push_back(ss.str());
            ss.str(std::wstring());
            continue;
        }

        if (!isScreening)
        {
            switch (character)
            {
            case L'"':
                isQuoted = !isQuoted;
                continue;
            case L'\\':
                isScreening = true;
                continue;
            default:
                break;
            }
        }

        ss << character;

        if (isScreening)
        {
            isScreening = false;
        }
    }

    if (!ss.str().empty() &&
        !isQuoted)
    {
        elems.push_back(ss.str());
        ss.clear();
    }

    return elems;
}

std::vector<std::string> HG::Utils::StringTools::smartSplit(const std::string& s, char delim)
{
    std::vector<std::string> elems;
    std::stringstream ss;

    bool isQuoted = false;
    bool isScreening = false;
    for (auto&& character : s)
    {
        if (character == delim &&
            !ss.str().empty() &&
            !isQuoted)
        {
            elems.push_back(ss.str());
            ss.str(std::string());
            continue;
        }

        if (!isScreening)
        {
            switch (character)
            {
            case '"':
                isQuoted = !isQuoted;
                continue;
            case '\\':
                isScreening = true;
                continue;
            default:
                break;
            }
        }

        ss << character;

        if (isScreening)
        {
            isScreening = false;
        }
    }

    if (!ss.str().empty() &&
        !isQuoted)
    {
        elems.push_back(ss.str());
        ss.clear();
    }

    return elems;
}

std::string HG::Utils::StringTools::toLower(const std::string& s)
{
    std::string copy(s);

    std::transform(
        copy.begin(),
        copy.end(),
        copy.begin(),
        ::tolower
    );

    return copy;
}
