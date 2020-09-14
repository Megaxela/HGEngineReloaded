#pragma once

// C++ STL
#include <list>
#include <string>
#include <unordered_map>
#include <variant>
#include <vector>

namespace HG::ToolsCore
{
/**
 * @brief Class, that describes command line
 * arguments definition and parser.
 */
class CommandLineArguments
{
public:
    using ArgumentType = std::variant<int, bool, std::string>;

    using ArgumentsMap = std::unordered_map<std::string, ArgumentType>;

    /**
     * @brief Argument actions.
     */
    enum class Action
    {
        Store,
        StoreConst,
        StoreTrue,
        StoreFalse,
        Help,
        Version
    };

    enum class Type
    {
        Integer,
        String,
        Boolean
    };

private:
    /**
     * @brief Argument, required in CommandLineArgument.
     */
    struct Argument
    {
        std::vector<std::string> keys;
        Action action                 = Action::Store;
        std::size_t numberOfArguments = 0;
        ArgumentType constantValue;
        ArgumentType defaultVaue;
        Type type = Type::String;
        std::vector<ArgumentType> choices;
        bool required = false;
        std::string meta;
        std::string destination;
        std::string help;
        std::string version;
    };

public:
    /**
     * @brief Builder class, that's used by CommandLineArguments::addArgument
     */
    class ArgumentBuilder
    {
    public:
        /**
         * @brief Constructor.
         * @param arg Reference to object to setup.
         */
        explicit ArgumentBuilder(Argument& arg);

        /**
         * @brief Method for setting argument action.
         * Store by-default.
         * @param action Action.
         * @return Reference to builder.
         */
        ArgumentBuilder& action(Action action);

        /**
         * @brief The number of command-line arguments that should be consumed.
         * after this one.
         * 0 by-default.
         * @param number
         * @return Reference to builder.
         */
        ArgumentBuilder& numberOfArguments(std::size_t number);

        /**
         * @brief A constant value required by some actions and number of arguments
         * selections.
         * Empty value by-default.
         * @param value Value.
         * @return Reference to builder.
         */
        ArgumentBuilder& constantValue(ArgumentType argument);

        /**
         * @brief The value produced if the argument is absent from the command line.
         * Empty value by-default.
         * @param value Value.
         * @return Reference to builder.
         */
        ArgumentBuilder& defaultValue(ArgumentType value);

        /**
         * @brief The type to which the command-line argument should be converted.
         * String by-default.
         * @param type Type.
         * @return Reference to builder.
         */
        ArgumentBuilder& type(Type type);

        /**
         * @brief A container of the allowable values for the argument.
         * Empty by-default.
         * @param choices Available choices.
         * @return Reference to builder.
         */
        ArgumentBuilder& choices(std::vector<ArgumentType> choices);

        /**
         * @brief Whether or not the command-line option may be omitted (optionals only).
         * Not required by-default.
         * @param required Is required.
         * @return Reference to builder.
         */
        ArgumentBuilder& required(bool required);

        /**
         * @brief A name for the argument in usage messages.
         * Empty by-default.
         * @param meta Meta value.
         * @return Reference to builder.
         */
        ArgumentBuilder& metavar(std::string meta);

        /**
         * @brief The name of the attribute to be added to the object returned
         * by CommandLineArguments::parse.
         * @param name Field name.
         * @return Reference to builder.
         */
        ArgumentBuilder& destination(std::string name);

        /**
         * @brief  A brief description of what the argument does.
         * @param help String.
         * @return Reference to builder.
         */
        ArgumentBuilder& help(std::string help);

        /**
         * @brief Version information.
         * @param version String version information.
         * @return Reference to builder.
         */
        ArgumentBuilder& version(std::string version);

    private:
        Argument& m_ref;
    };

    /**
     * @brief Arguments constructor.
     * @param name Descriptions.
     */
    explicit CommandLineArguments(std::string name);

    /**
     * @brief Method for adding argument.
     * @param namesOrFlags Name or flags. Example: {"--name", "-n", "-N"}
     * @return Builder for argument setup.
     */
    ArgumentBuilder addArgument(std::vector<std::string> namesOrFlags);

    /**
     * @brief Method for parsing argc/argv.
     * @param argc Count of arguments.
     * @param argv Pointer to arguments.
     * @return Parsing result.
     */
    ArgumentsMap parse(int argc, char** argv);

    /**
     * @brief Method, that will show version text to stdout.
     */
    void showVersion();

    /**
     * @brief Method, that will show help text to stdout.
     */
    void showHelp();

    /**
     * @brief Method, that dispays help usage line.
     */
    void showUsageLine();

private:
    std::string getValueReplacer(const Argument* arg) const;

    ArgumentsMap internalParsing(int numberOfArguments, char** arguments);

    void showArguments(const std::vector<const Argument*>& arguments);

    std::string nameFromKey(const std::string& s) const;

    ArgumentType parseValue(const char* value, Type t);

    std::string m_name;

    std::unordered_map<std::string, Argument*> m_argumentsMap;

    std::list<Argument> m_argumentsList;
};
} // namespace HG::ToolsCore
