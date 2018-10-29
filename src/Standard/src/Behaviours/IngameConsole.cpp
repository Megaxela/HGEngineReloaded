// C++ STL
#include <algorithm>

// HG::Core
#include <HG/Core/Scene.hpp>
#include <HG/Core/Application.hpp>

// HG::Standard
#include <HG/Standard/Behaviours/IngameConsole.hpp>
#include <HG/Standard/Behaviours/DebugControllerOverlay.hpp>

// HG::Utils
#include <HG/Utils/StringTools.hpp>

// ALogger
#include <CurrentLogger.hpp>

// ImGui
#include <imgui.h>


HG::Standard::Behaviours::IngameConsole::IngameConsole() :
    m_logsListener(),
    m_commands(),
    m_linesBuffer()
{
    connectLoggingWatcher();

    Info() << "Creating ingame console";

    addCommand(Command(
        "help",
        "Displays this text.",
        [this](Command::Arguments) -> int
        {
            for (auto&& [name, command] : m_commands)
            {
                logText(HG::Utils::Color::Gray, command.command);

                auto splittedDescription = HG::Utils::StringTools::split(command.description, '\n');

                for (auto&& line : splittedDescription)
                {
                    logText(HG::Utils::Color::White, "    " + line);
                }
            }

            return 0;
        }
    ));

    addCommand(Command(
        "clear",
        "Clears console buffer.",
        [this](Command::Arguments) -> int
        {
            m_linesBuffer.clear();

            return 0;
        }
    ));

    addCommand(Command(
        "quit",
        "Closes application.",
        [this](Command::Arguments) -> int
        {
            Info() << "Closing app from console.";
            scene()->application()->stop();

            return 0;
        }
    ));
    
    addCommand(Command(
        "cl_enableDebug",
        "Enables or disables debug controller system.\n"
        "Expected arguments: [0, 1]",
        [this](Command::Arguments a) { return commandClEnableDebug(a); }
    ));
}

HG::Standard::Behaviours::IngameConsole::~IngameConsole()
{
    if (m_logsListener && CurrentLogger::i())
    {
        CurrentLogger::i()->removeLogsListener(m_logsListener);
    }
}

void HG::Standard::Behaviours::IngameConsole::addCommand(HG::Standard::Behaviours::IngameConsole::Command c)
{
    m_commands[HG::Utils::StringTools::toLower(c.command)] = std::move(c);
}

void HG::Standard::Behaviours::IngameConsole::displayConsole()
{
    bool shown = true;

    if (ImGui::Begin("Console", &shown))
    {
        if (!shown)
        {
            setEnabled(false);
        }

        {
            const float footer_height_to_reserve = ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing(); // 1 separator, 1 input text
            ImGui::BeginChild(
                "LogsRegion",
                ImVec2(0, -footer_height_to_reserve),
                false,
                ImGuiWindowFlags_HorizontalScrollbar
            );

            {
                ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 1));

                for (auto&& [color, message] : m_linesBuffer)
                {
                    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(color.redF(), color.greenF(), color.blueF(), 1.0f));

                    ImGui::TextUnformatted(message.c_str());

                    ImGui::PopStyleColor(1);
                }

                ImGui::PopStyleVar(1);
            }

            ImGui::EndChild();
        }

        ImGui::Separator();

        static char buffer[1024];

        ImGui::PushItemWidth(-1);

        if (ImGui::InputText(
            "##label",
            buffer, 1024,
            ImGuiInputTextFlags_EnterReturnsTrue
//            ImGuiInputTextFlags_
//        ImGuiInputTextFlags_CallbackCompletion |
//        ImGuiInputTextFlags_CallbackHistory
        ) && buffer[0])
        {
            executeCommand(buffer);

            buffer[0] = '\0';

            ImGui::SetItemDefaultFocus();
            ImGui::SetKeyboardFocusHere(-1);
        }

        ImGui::PopItemWidth();

        ImGui::End();
    }
}

void HG::Standard::Behaviours::IngameConsole::onStart()
{
    setEnabled(false);
}

void HG::Standard::Behaviours::IngameConsole::onUpdate()
{
    proceedLogs();

    displayConsole();
}

void HG::Standard::Behaviours::IngameConsole::proceedLogs()
{
    AbstractLogger::Message msg;
    while (m_logsListener &&
           m_logsListener->hasMessages())
    {
        msg = std::move(m_logsListener->popMessage());

        proceedMessage(std::move(msg));
    }
}

void HG::Standard::Behaviours::IngameConsole::logText(const HG::Utils::Color& color, std::string text)
{
    m_linesBuffer.emplace_back(color, text);
}

void HG::Standard::Behaviours::IngameConsole::executeCommand(std::string command)
{
    logText(HG::Utils::Color::fromRGB(250, 250, 250), ">>> " + command);

    // Splitting command by spaces
    if (command.length() > 0)
    {
        auto splittedCommand = HG::Utils::StringTools::smartSplit(command, ' ');

//        addCommandToHistory(std::move(command));

        std::vector<std::string> arguments(splittedCommand.begin() + 1, splittedCommand.end());

        auto commandName = HG::Utils::StringTools::toLower(splittedCommand[0]);

        auto commandIter = std::find_if(
            m_commands.begin(),
            m_commands.end(),
            [&commandName](const std::pair<std::string, Command>& pair)
            { return pair.first == commandName; }
        );

        if (commandIter != m_commands.end())
        {
            auto result = commandIter->second.function(arguments);

            // Displaying command description
            if (result == -1)
            {
                auto errorColor = getLogColor(AbstractLogger::ErrorClass::Error);

                logText(errorColor, "Wrong usage of command \"" + splittedCommand[0] + "\"");

                auto splittedDescription = HG::Utils::StringTools::split(commandIter->second.description, '\n');

                for (auto&& line : splittedDescription)
                {
                    logText(errorColor, line);
                }
            }
        }
        else
        {
            auto errorColor = getLogColor(AbstractLogger::ErrorClass::Error);
            logText(errorColor, "There is no command \"" + splittedCommand[0] + "\"");
        }
    }
}

void HG::Standard::Behaviours::IngameConsole::proceedMessage(AbstractLogger::Message message)
{
    logText(getLogColor(message.errorClass), formatMessage(message));
}

std::string HG::Standard::Behaviours::IngameConsole::formatMessage(AbstractLogger::Message message)
{
    return message.message;
}

HG::Utils::Color HG::Standard::Behaviours::IngameConsole::getLogColor(AbstractLogger::ErrorClass errClass)
{
    switch (errClass)
    {
    case AbstractLogger::ErrorClass::Debug:
        return HG::Utils::Color::fromRGB(180, 180, 180);
    case AbstractLogger::ErrorClass::Warning:
        return HG::Utils::Color::fromRGB(180, 50, 50);
    case AbstractLogger::ErrorClass::Error:
        return HG::Utils::Color::fromRGB(255, 50, 50);
    case AbstractLogger::ErrorClass::None:
    case AbstractLogger::ErrorClass::Info:
    case AbstractLogger::ErrorClass::Unknown:
        return HG::Utils::Color::White;
    }

    return HG::Utils::Color::White;
}

void HG::Standard::Behaviours::IngameConsole::connectLoggingWatcher()
{
    m_logsListener = std::make_shared<LoggingWatcher>(this);
    
    if (CurrentLogger::i())
    {
        CurrentLogger::i()->addLogsListener(m_logsListener);
    }
    else
    {
        std::cerr << "Can't connect logging watcher, because of no logger." << std::endl;
    }
}

HG::Standard::Behaviours::IngameConsole::LoggingWatcher::LoggingWatcher(HG::Standard::Behaviours::IngameConsole* ingameConsole) :
    m_console(ingameConsole),
    m_messages()
{

}

AbstractLogger::Message HG::Standard::Behaviours::IngameConsole::LoggingWatcher::popMessage()
{
    auto element = std::move(m_messages.front());
    
    m_messages.pop_front();
    
    return element;
}

bool HG::Standard::Behaviours::IngameConsole::LoggingWatcher::hasMessages() const
{
    return !m_messages.empty();
}

void HG::Standard::Behaviours::IngameConsole::LoggingWatcher::newMessage(const AbstractLogger::Message& m)
{
    m_messages.push_back(m);
}

int HG::Standard::Behaviours::IngameConsole::commandClEnableDebug(
    HG::Standard::Behaviours::IngameConsole::Command::Arguments arguments)
{
    return !toggleBehaviour<DebugControllerOverlay>(arguments);
}
