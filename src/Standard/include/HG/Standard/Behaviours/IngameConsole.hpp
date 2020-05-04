#pragma once

// C++ STL
#include <map>

// HG::Core
#include <HG/Core/Behaviour.hpp>
#include <HG/Core/GameObject.hpp> // Because of template method

// HG::Utils
#include <HG/Utils/Color.hpp>

// spdlog
#include <spdlog/sinks/base_sink.h>

// RingBuffer
#include <ringbuffer.hpp>

namespace HG::Standard::Behaviours
{
/**
 * @brief Standard behaviour, that implements
 * ingame console.
 */
class IngameConsole : public HG::Core::Behaviour
{
public:
    struct Command
    {
        using Arguments = std::vector<std::string>;

        using Function = std::function<int(Arguments)>;

        Command() : command(), description(), function()
        {
        }

        Command(std::string command, std::string desc, Function func) :
            command(std::move(command)),
            description(std::move(desc)),
            function(std::move(func))
        {
        }

        std::string command;

        std::string description;

        Function function;
    };

    /**
     * @brief Constructor.
     */
    IngameConsole();

    /**
     * @brief Destructor.
     */
    ~IngameConsole() override;

    /**
     * @brief Method for adding command to ingame console.
     * @param c Command.
     */
    void addCommand(Command c);

protected:
    void onUpdate() override;

    void onStart() override;

private:
    struct Message
    {
        Message() : message(), level(spdlog::level::level_enum::off)
        {
        }

        Message(std::string msg, spdlog::level::level_enum lvl) : message(std::move(msg)), level(lvl)
        {
        }

        std::string message;
        spdlog::level::level_enum level;
    };

    template <typename MutexT>
    class LoggingWatcher : public spdlog::sinks::base_sink<MutexT>
    {
    public:
        explicit LoggingWatcher(IngameConsole* ingameConsole) : m_console(ingameConsole), m_messages()
        {
        }

        Message popMessage()
        {
            auto element = std::move(m_messages.front());
            m_messages.pop_front();

            return element;
        }

        [[nodiscard]] bool hasMessages() const
        {
            return !m_messages.empty();
        }

    protected:
        void flush_() override
        {
        }

        void sink_it_(const spdlog::details::log_msg& msg) override
        {
            spdlog::memory_buf_t formatted;
            spdlog::sinks::base_sink<MutexT>::formatter_->format(msg, formatted);

            m_messages.emplace_back(std::string(msg.payload.begin(), msg.payload.end()), msg.level);
        }

    private:
        IngameConsole* m_console;

        ringbuffer<Message, 1024> m_messages;
    };

    void displayConsole();

    void connectLoggingWatcher();

    void proceedLogs();

    void proceedMessage(const Message& message);

    void logText(const HG::Utils::Color& color, std::string text);

    std::string formatMessage(const Message& message);

    HG::Utils::Color getLogColor(spdlog::level::level_enum errClass);

    void executeCommand(std::string command);

    template <typename T>
    bool toggleBehaviour(const Command::Arguments& arguments)
    {
        if (arguments.size() < 1 || (arguments[0] != "0" && arguments[0] != "1"))
        {
            return false;
        }

        auto behaviour = gameObject()->findBehaviour<T>();

        if (behaviour && arguments[0] == "0")
        {
            if (behaviour->isEnabled())
            {
                behaviour->setEnabled(false);
            }
            else
            {
                logText(getLogColor(spdlog::level::level_enum::err), "Already disabled");
            }
        }
        else if (behaviour && arguments[0] == "1")
        {
            if (behaviour->isEnabled())
            {
                logText(getLogColor(spdlog::level::level_enum::err), "Already enabled");
            }
            else
            {
                behaviour->setEnabled(true);
            }
        }
        else if (!behaviour && arguments[0] == "0")
        {
            logText(getLogColor(spdlog::level::level_enum::err), "Already disabled");
        }
        else if (!behaviour && arguments[0] == "1")
        {
            gameObject()->addBehaviour(new T);
        }

        return true;
    }

    // Commands
    int commandClEnableDebug(Command::Arguments arguments);

    std::shared_ptr<LoggingWatcher<std::mutex>> m_logsListener;

    std::unordered_map<std::string, Command> m_commands;

    ringbuffer<std::pair<HG::Utils::Color, std::string>, 1024> m_linesBuffer;
};
} // namespace HG::Standard::Behaviours
