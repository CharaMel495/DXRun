#pragma once

namespace CaramelEngine 
{

    /// <summary>
    /// エラーレベルを定義
    /// </summary>
    enum class LogLevel 
    {
        INFO,
        WARNING,
        ERR,
        DEBUG
    };

#ifndef NDEBUG  // デバッグビルド時のみ有効
    class Logger 
    {
    public:
        static Logger& getInstance() 
        {
            static Logger instance;
            return instance;
        }

        void setLogLevel(LogLevel level) 
        {
            _logLevel = level;
        }

        void enableFileOutput(const CString& filename) 
        {
            _logFile.open(filename, std::ios::out | std::ios::app);
            if (!_logFile.is_open())
                std::cerr << "Failed to open log file: " << filename << std::endl;
        }

        void log(LogLevel level, const CString& message, const char* file, int line, const char* function) 
        {
            if (level >= _logLevel) 
            {
                CString logLevelStr = getLogLevelString(level);
                CString timestamp = getCurrentTime();
                CString logMessage = "[" + timestamp + "] [" + logLevelStr + "] (" + file + ":" + std::to_string(line) + " in " + function + ") " + message;

                // Console output
                std::cout << logMessage << std::endl;

                // File output if enabled
                if (_logFile.is_open()) 
                {
                    _logFile << logMessage << std::endl;
                }
            }
        }

    private:
        LogLevel _logLevel = LogLevel::DEBUG;
        std::ofstream _logFile;

        Logger() = default;
        ~Logger() 
        {
            if (_logFile.is_open())
                _logFile.close();
        }

        CString getLogLevelString(LogLevel level) 
        {
            switch (level) {
            case LogLevel::INFO: return "INFO";
            case LogLevel::WARNING: return "WARNING";
            case LogLevel::ERR: return "ERROR";
            case LogLevel::DEBUG: return "DEBUG";
            default: return "UNKNOWN";
            }
        }

        CString getCurrentTime()
        {
            auto now = std::chrono::system_clock::now();
            auto time = std::chrono::system_clock::to_time_t(now);

            std::tm lt = {};
            localtime_s(&lt, &time);

            uint64_t ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();

            std::stringstream ss;
            ss << lt.tm_year + 1900;
            ss << "/" << std::setw(2) << std::setfill('0') << lt.tm_mon + 1;
            ss << "/" << std::setw(2) << std::setfill('0') << lt.tm_mday;
            ss << " " << std::setw(2) << std::setfill('0') << lt.tm_hour;
            ss << ":" << std::setw(2) << std::setfill('0') << lt.tm_min;
            ss << ":" << std::setw(2) << std::setfill('0') << lt.tm_sec;
            ss << "." << std::setw(3) << std::setfill('0') << (ms % 1000);

            return CString(ss.str().c_str());
        }

    };

    // ログ出力インライン関数（NDEBUG時は無効化）
    inline void log(LogLevel level, const CString& message, const char* file, int line, const char* func) 
    {
        Logger::getInstance().log(level, message, file, line, func);
    }

    // ログ出力専用インライン関数
    inline void logInfo(const CString& message, const char* file, int line, const char* func) 
    {
        log(LogLevel::INFO, message, file, line, func);
    }

    inline void logWarning(const CString& message, const char* file, int line, const char* func) 
    {
        log(LogLevel::WARNING, message, file, line, func);
    }

    inline void logError(const CString& message, const char* file, int line, const char* func) 
    {
        log(LogLevel::ERR, message, file, line, func);
    }

    inline void logDebug(const CString& message, const char* file, int line, const char* func) 
    {
        log(LogLevel::DEBUG, message, file, line, func);
    }

#else  // リリースビルドでは空の関数として定義
    inline void log(LogLevel, const CString&, const char*, int, const char*) {}
    inline void logInfo(const CString&, const char*, int, const char*) {}
    inline void logWarning(const CString&, const char*, int, const char*) {}
    inline void logError(const CString&, const char*, int, const char*) {}
    inline void logDebug(const CString&, const char*, int, const char*) {}
#endif

} // namespace CaramelEngine
