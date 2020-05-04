// HG::Core
#include <HG/Core/Application.hpp>
#include <HG/Core/FilesystemResourceAccessor.hpp>
#include <HG/Core/ResourceManager.hpp>

// HG::Networking
#include <HG/Networking/Base/Server.hpp>

// HG::Utils
#include <HG/Utils/Logging.hpp>

// HG::ToolsCore
#include <HG/ToolsCore/CommandLineArguments.hpp>

// NetworkingDedicated Server
#include <ArgumentNames.hpp>

int main(int argc, char** argv)
{
    HG::ToolsCore::CommandLineArguments arguments(argv[0]);

    arguments.addArgument({"--version"})
        .help("prints server version")
        .action(HG::ToolsCore::CommandLineArguments::Action::Version)
        .version("1.0");

    arguments.addArgument({"-t", "--tcpPort"})
        .help("tcp port to bind")
        .numberOfArguments(1)
        .required(true)
        .type(HG::ToolsCore::CommandLineArguments::Type::Integer)
        .destination(ArgumentsNames::kTcpPort);

    arguments.addArgument({"-u", "--udpPort"})
        .help("udp port to bind")
        .numberOfArguments(1)
        .required(true)
        .type(HG::ToolsCore::CommandLineArguments::Type::Integer)
        .destination(ArgumentsNames::kUdpPort);

    auto args = arguments.parse(argc, argv);

    HG::Core::Application application("Networking Dedicated Server");
    application.resourceManager()->setResourceAccessor(new HG::Core::FilesystemResourceAccessor());

    HG::Networking::Base::Server server(&application);

    try
    {
        auto tcpPort = std::get<int>(args.find(ArgumentsNames::kTcpPort)->second);
        auto udpPort = std::get<int>(args.find(ArgumentsNames::kUdpPort)->second);

        server.start(udpPort, tcpPort, 2);
    }
    catch (const std::invalid_argument& e)
    {
        HGErrorF() << "Invalid argument: \"" << e.what() << "\". See help below.";
        arguments.showHelp();
        return 1;
    }
    catch (const std::runtime_error& e)
    {
        HGErrorF() << "Error: \"" << e.what() << "\"";
        return 2;
    }

    HGInfoF() << "Server is up and running.";
    std::string a;
    std::cin >> a;

    HGInfoF() << "Stopping server...";
    server.stop();

    return 0;
}