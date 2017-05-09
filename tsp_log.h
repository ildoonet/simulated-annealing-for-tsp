#ifndef SA_TSP_TSP_LOG_H
#define SA_TSP_TSP_LOG_H

#include <string>

const int LogLevelSize = 4;
enum LogLevel { debug, info, warning, error };
const std::string LogLevelString[LogLevelSize] = {
        "DEBUG", "INFO", "WARN", "ERROR"
};

class TspLogger {
    bool printStatus = true;
public:
    LogLevel level;

    TspLogger &operator<<(const std::string &msg);
    TspLogger &operator<<(const int &msg);
    TspLogger &operator<<(const double &msg);
    TspLogger &operator<<(const float &msg);

    static const std::string endl;
};

void setLogLevel(LogLevel l);
TspLogger &tsplog(LogLevel n);

#endif //SA_TSP_TSP_LOG_H
