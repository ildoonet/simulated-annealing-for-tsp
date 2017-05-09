#include <iostream>
#include <ctime>
#include <sstream>
#include "tsp_log.h"

const std::string TspLogger::endl = "\n";
LogLevel currentLogLv = info;
TspLogger l[LogLevelSize];

TspLogger& TspLogger::operator<<(const std::string &msg) {
    time_t rawtime;
    struct tm * timeinfo;
    char dt_buffer[80];

    time (&rawtime);
    timeinfo = localtime(&rawtime);

    strftime(dt_buffer,sizeof(dt_buffer),"%d-%m-%Y %I:%M:%S",timeinfo);

    if (this->level >= currentLogLv) {
        if (printStatus) {
            std::cout << "[" << dt_buffer << "]" << "[" << LogLevelString[this->level] << "] ";
            printStatus = false;
        } else if (msg == endl) {
            printStatus = true;
        }
        std::cout << msg;
    }
    return *this;
}

template <typename T>
std::string to_string(T val)
{
    std::stringstream stream;
    stream << val;
    return stream.str();
}

TspLogger& TspLogger::operator<<(const int &msg) {
    return this->operator<<(to_string<int>(msg));
}

TspLogger& TspLogger::operator<<(const double &msg) {
    return this->operator<<(to_string<double>(msg));
}

TspLogger& TspLogger::operator<<(const float &msg) {
    return this->operator<<(to_string<float>(msg));
}

TspLogger& tsplog(LogLevel n) {
    TspLogger& logger = l[n];
    logger.level = n;
    return logger;
}

void setLogLevel(LogLevel l) {
    currentLogLv = l;
}
