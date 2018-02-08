#include <Application.hpp>

int main(int argc, char** argv)
{
    HG::Core::Application application(argc, argv);

    return application.exec();
}