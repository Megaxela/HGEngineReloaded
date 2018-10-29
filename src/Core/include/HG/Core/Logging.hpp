#pragma once

// ALogger
#include <Loggers/AbstractLogger.hpp>
#include <SystemTools.h>

#define HGDebug()     Loggers::Stream(HG::Core::Logging::userLogger(), AbstractLogger::ErrorClass::Debug,   __FILENAME__, __LINE__, std::this_thread::get_id(), SystemTools::getTypeName(*this), __FUNCTION__)
#define HGInfo()      Loggers::Stream(HG::Core::Logging::userLogger(), AbstractLogger::ErrorClass::Info,    __FILENAME__, __LINE__, std::this_thread::get_id(), SystemTools::getTypeName(*this), __FUNCTION__)
#define HGWarning()   Loggers::Stream(HG::Core::Logging::userLogger(), AbstractLogger::ErrorClass::Warning, __FILENAME__, __LINE__, std::this_thread::get_id(), SystemTools::getTypeName(*this), __FUNCTION__)
#define HGError()     Loggers::Stream(HG::Core::Logging::userLogger(), AbstractLogger::ErrorClass::Error,   __FILENAME__, __LINE__, std::this_thread::get_id(), SystemTools::getTypeName(*this), __FUNCTION__)

#define HGDebugF()    Loggers::Stream(HG::Core::Logging::userLogger(), AbstractLogger::ErrorClass::Debug,   __FILENAME__, __LINE__, std::this_thread::get_id(), std::string(), __FUNCTION__)
#define HGInfoF()     Loggers::Stream(HG::Core::Logging::userLogger(), AbstractLogger::ErrorClass::Info,    __FILENAME__, __LINE__, std::this_thread::get_id(), std::string(), __FUNCTION__)
#define HGWarningF()  Loggers::Stream(HG::Core::Logging::userLogger(), AbstractLogger::ErrorClass::Warning, __FILENAME__, __LINE__, std::this_thread::get_id(), std::string(), __FUNCTION__)
#define HGErrorF()    Loggers::Stream(HG::Core::Logging::userLogger(), AbstractLogger::ErrorClass::Error,   __FILENAME__, __LINE__, std::this_thread::get_id(), std::string(), __FUNCTION__)

#define HGDebugEx(CLASSNAME)    Loggers::Stream(HG::Core::Logging::userLogger(), AbstractLogger::ErrorClass::Debug,   __FILENAME__, __LINE__, std::this_thread::get_id(), CLASSNAME, __FUNCTION__)
#define HGInfoEx(CLASSNAME)     Loggers::Stream(HG::Core::Logging::userLogger(), AbstractLogger::ErrorClass::Info,    __FILENAME__, __LINE__, std::this_thread::get_id(), CLASSNAME, __FUNCTION__)
#define HGWarningEx(CLASSNAME)  Loggers::Stream(HG::Core::Logging::userLogger(), AbstractLogger::ErrorClass::Warning, __FILENAME__, __LINE__, std::this_thread::get_id(), CLASSNAME, __FUNCTION__)
#define HGErrorEx(CLASSNAME)    Loggers::Stream(HG::Core::Logging::userLogger(), AbstractLogger::ErrorClass::Error,   __FILENAME__, __LINE__, std::this_thread::get_id(), CLASSNAME, __FUNCTION__)

namespace HG::Core
{
    /**
     * @brief Class, that describes user logging system.
     * This system uses ALogger library as backend.
     */
    class Logging
    {
    public:

        /**
         * @brief Method for getting user logger.
         * @return Pointer to user logger.
         */
        static LoggerPtr userLogger();

    };
}


