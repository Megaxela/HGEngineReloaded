// C++ STL
#include <iostream>
#include <algorithm>
#include <cstring>

// HG::ToolsCore
#include <HG/ToolsCore/CommandLineArguments.hpp>

HG::ToolsCore::CommandLineArguments::ArgumentBuilder::ArgumentBuilder(
        HG::ToolsCore::CommandLineArguments::Argument &arg) :
    m_ref(arg)
{

}

HG::ToolsCore::CommandLineArguments::ArgumentBuilder &
HG::ToolsCore::CommandLineArguments::ArgumentBuilder::action(HG::ToolsCore::CommandLineArguments::Action action)
{
    m_ref.action = action;

    return (*this);
}

HG::ToolsCore::CommandLineArguments::ArgumentBuilder &
HG::ToolsCore::CommandLineArguments::ArgumentBuilder::numberOfArguments(std::size_t number)
{
    m_ref.numberOfArguments = number;

    return (*this);
}

HG::ToolsCore::CommandLineArguments::ArgumentBuilder &
HG::ToolsCore::CommandLineArguments::ArgumentBuilder::constantValue(
        HG::ToolsCore::CommandLineArguments::ArgumentType argument)
{
    m_ref.constantValue = std::move(argument);

    return (*this);
}

HG::ToolsCore::CommandLineArguments::ArgumentBuilder &
HG::ToolsCore::CommandLineArguments::ArgumentBuilder::defaultValue(
        HG::ToolsCore::CommandLineArguments::ArgumentType value)
{
    m_ref.defaultVaue = std::move(value);

    return (*this);
}

HG::ToolsCore::CommandLineArguments::ArgumentBuilder &
HG::ToolsCore::CommandLineArguments::ArgumentBuilder::type(HG::ToolsCore::CommandLineArguments::Type type)
{
    m_ref.type = type;

    return (*this);
}

HG::ToolsCore::CommandLineArguments::ArgumentBuilder &HG::ToolsCore::CommandLineArguments::ArgumentBuilder::choices(
        std::vector<HG::ToolsCore::CommandLineArguments::ArgumentType> choices)
{
    m_ref.choices = std::move(choices);

    return (*this);
}

HG::ToolsCore::CommandLineArguments::ArgumentBuilder &
HG::ToolsCore::CommandLineArguments::ArgumentBuilder::required(bool required)
{
    m_ref.required = required;

    return (*this);
}

HG::ToolsCore::CommandLineArguments::ArgumentBuilder &
HG::ToolsCore::CommandLineArguments::ArgumentBuilder::metavar(std::string meta)
{
    m_ref.meta = std::move(meta);

    return (*this);
}

HG::ToolsCore::CommandLineArguments::ArgumentBuilder &
HG::ToolsCore::CommandLineArguments::ArgumentBuilder::destination(std::string name)
{
    m_ref.destination = std::move(name);

    return (*this);
}

HG::ToolsCore::CommandLineArguments::ArgumentBuilder &
HG::ToolsCore::CommandLineArguments::ArgumentBuilder::help(std::string help)
{
    m_ref.help = std::move(help);

    return (*this);
}

HG::ToolsCore::CommandLineArguments::ArgumentBuilder &
HG::ToolsCore::CommandLineArguments::ArgumentBuilder::version(std::string version)
{
    m_ref.version = std::move(version);

    return (*this);
}

HG::ToolsCore::CommandLineArguments::CommandLineArguments(std::string name) :
    m_name(std::move(name)),
    m_argumentsMap(),
    m_argumentsList()
{
    // Adding default help argument
    addArgument({"-h", "--help"})
        .help("show this message and exit")
        .action(Action::Help)
        .numberOfArguments(0);
}

HG::ToolsCore::CommandLineArguments::ArgumentBuilder
HG::ToolsCore::CommandLineArguments::addArgument(std::vector<std::string> namesOrFlags)
{
    if (namesOrFlags.empty())
    {
        throw std::invalid_argument("Can't create argument without flags or name");
    }

    // Creating argument
    {
        Argument argument;
        argument.keys = std::move(namesOrFlags);
        m_argumentsList.emplace_back(std::move(argument));
    }

    Argument& keptArgument = m_argumentsList.back();

    // Adding to map
    for (auto& key : keptArgument.keys)
    {
        m_argumentsMap.insert({key, &keptArgument});
    }

    return ArgumentBuilder(keptArgument);
}

HG::ToolsCore::CommandLineArguments::ArgumentsMap
HG::ToolsCore::CommandLineArguments::parse(int argc, char **argv)
{
    // Collecting required elements and counting minimal
    // number of arguments to proceed
    // todo c++20: use views to collect em
    std::unordered_map<std::string, const Argument*> requiredArguments;
    std::size_t minimalNumberOfArguments = 0;

    for (const auto& argument : m_argumentsList)
    {
        if (!argument.required)
        {
            continue;
        }

        // Adding all keys to required
        for (auto& key : argument.keys)
        {
            requiredArguments.insert({key, &argument});
        }

        minimalNumberOfArguments += argument.numberOfArguments + 1;
    }

    // Iterating through items
    if ((argc - 1) < minimalNumberOfArguments)
    {
        std::cout << "Wrong usage. Not enough required arguments. See help below." << std::endl;
        showHelp();
        exit(-1);
    }

    return internalParsing(argc - 1, argv + 1);
}

void HG::ToolsCore::CommandLineArguments::showHelp()
{
    showUsageLine();
    std::cout << std::endl;

    std::vector<const Argument*> requiredArguments;
    std::vector<const Argument*> optionalArguments;

    for (const auto& argument : m_argumentsList)
    {
        if (argument.required)
        {
            requiredArguments.push_back(&argument);
        }
        else
        {
            optionalArguments.push_back(&argument);
        }
    }

    if (!requiredArguments.empty())
    {
        std::cout << "required arguments:" << std::endl;
        showArguments(requiredArguments);

        std::cout << std::endl;
    }

    if (!optionalArguments.empty())
    {
        std::cout << "optional arguments:" << std::endl;
        showArguments(optionalArguments);
    }
}

void HG::ToolsCore::CommandLineArguments::showUsageLine()
{
    std::cout << "usage: " << m_name << ' ';

    for (const auto& argument : m_argumentsList)
    {
        if (!argument.required)
        {
            std::cout << '[';
        }

        const std::string* selected = nullptr;

        // Find command or leave first flags
        for (auto& val : argument.keys)
        {
            if (selected == nullptr)
            {
                selected = &val;
                continue;
            }

            // todo: Make this condition more convinient
            if (val.size() > 1 &&
                val[0] == '-' && val[1] != '-')
            {
                selected = &val;
            }
        }

        // Printing it
        std::cout << *selected;

        // todo c++20: make pythonlike for
        if (!argument.meta.empty())
        {
            for (std::size_t i = 0; i < argument.numberOfArguments; ++i)
            {
                std::cout << ' ' << argument.meta;
            }
        }

        if (!argument.required)
        {
            std::cout << "] ";
        }
    }

    std::cout << std::endl;
}

void HG::ToolsCore::CommandLineArguments::showArguments(
        const std::vector<const HG::ToolsCore::CommandLineArguments::Argument *> &arguments)
{
    // Getting required number of tabs
    std::size_t requiredNumberOfTags = (*std::max_element(
            arguments.begin(),
            arguments.end(),
            [](const Argument* l, const Argument* r)
            {
                return l->keys.size() < r->keys.size();
            }
    ))->keys.size();

    for (const auto* argument : arguments)
    {
        // Indentation
        std::cout << "  ";

        // todo c++20: filter '-' with views

        std::string valueReplacer = nameFromKey(argument->keys.back());

        std::transform(valueReplacer.begin(), valueReplacer.end(), valueReplacer.begin(), [](char c) {return std::toupper(c); });

        bool isFirst = true;
        for (const auto& key : argument->keys)
        {
            if (!isFirst)
            {
                std::cout << ", ";
            }

            std::cout << key;

            // todo c++20: use views for iterating for
            for (std::size_t i = 0; i < argument->numberOfArguments; ++i)
            {
                if (!argument->meta.empty())
                {
                    std::cout << ' ' << argument->meta;
                }
                else if (!argument->choices.empty())
                {
                    std::cout << " {";

                    bool firstChoice = true;
                    for (auto& choice : argument->choices)
                    {
                        if (!firstChoice)
                        {
                            std::cout << ",";
                        }

                        switch (argument->type)
                        {
                        case Type::Integer:
                            std::cout << std::get<int>(choice);
                            break;
                        case Type::String:
                            std::cout << std::get<std::string>(choice);
                            break;
                        default:
                            throw std::runtime_error("Unknown argument type.");
                        }

                        firstChoice = false;
                    }
                    std::cout << '}';
                }
                else
                {
                    std::cout << ' ' << valueReplacer;
                }
            }


            isFirst = false;
        }

        // Printing description
        if (!argument->help.empty())
        {
            // todo: c++20 use views
            for (std::size_t i = 0; i < requiredNumberOfTags; ++i)
            {
                std::cout << '\t';
            }

            std::cout << argument->help;
        }

        std::cout << std::endl;
    }

    std::cout << std::endl;
}

void HG::ToolsCore::CommandLineArguments::showVersion()
{

}

HG::ToolsCore::CommandLineArguments::ArgumentType
HG::ToolsCore::CommandLineArguments::parseValue(const char *value, HG::ToolsCore::CommandLineArguments::Type t)
{
    ArgumentType result;

    switch (t)
    {
    case Type::Integer:
    {
        try
        {
            result = std::stoi(value);
        }
        catch (...)
        {
            std::cout << "Wrong value for number type. See help above." << std::endl;
            showHelp();
            exit(1);
        }

        break;
    }
    case Type::String:
    {
        result = std::string(value);

        break;
    }
    case Type::Boolean:
    {
        if (std::strcmp(value, "true") == 0 ||
            std::strcmp(value, "TRUE") == 0 ||
            std::strcmp(value, "True") == 0)
        {
            result = true;
        }
        else if (std::strcmp(value, "false") == 0 ||
                 std::strcmp(value, "FALSE") == 0 ||
                 std::strcmp(value, "False") == 0)
        {
            result = false;
        }

        break;
    }
    }

    return result;
}

HG::ToolsCore::CommandLineArguments::ArgumentsMap
HG::ToolsCore::CommandLineArguments::internalParsing(int numberOfArguments, char **arguments)
{
    std::unordered_map<std::string, ArgumentType> result;

    Argument* arg = nullptr;
    std::size_t argumentsLeft = 0;
    for (int i = 0; i < numberOfArguments; ++i)
    {
        // If arguments ended - searching for command
        if (argumentsLeft == 0)
        {
            arg = nullptr;
        }

        // If there was no argument selected
        // select one
        if (arg == nullptr)
        {
            auto iter = m_argumentsMap.find(arguments[i]);

            if (iter == m_argumentsMap.end())
            {
                std::cout << "Unknown argument \"" << arguments[i] << "\". See help below.";
                showHelp();
                exit(1);
            }

            arg = iter->second;
            argumentsLeft = arg->numberOfArguments;

            if (argumentsLeft > 0)
            {
                continue;
            }
        }

        // Parse argument as value
        ArgumentType value;

        switch (arg->action)
        {
        case Action::Store:
            if (argumentsLeft == 0)
            {
                throw std::runtime_error("Can't store value at flag without arguments.");
            }

            value = parseValue(arguments[i], arg->type);
            break;
        case Action::StoreConst:
            value = arg->constantValue;
            break;
        case Action::StoreTrue:
            value = true;
            break;
        case Action::StoreFalse:
            value = false;
            break;
        case Action::Help:
            showHelp();
            exit(1);
        case Action::Version:
            showVersion();
            exit(1);
        default:
            throw std::runtime_error("Unknown argument action found.");
        }

        if (!arg->destination.empty())
        {
            result.insert({arg->destination, value});
        }
        else
        {
            result.insert({nameFromKey(arg->keys.back()), value});
        }

        --argumentsLeft;
    }

    return result;
}

std::string HG::ToolsCore::CommandLineArguments::nameFromKey(const std::string &s)
{
    return std::string(
            std::find_if(
                    s.begin(),
                    s.end(),
                    [](char c){ return c != '-'; }
            ),
            s.end()
    );
}
