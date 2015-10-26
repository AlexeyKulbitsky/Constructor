#ifndef LOGGER_H
#define LOGGER_H


#include <QFile>
#include <QDate>
#include <QTime>
#include <QApplication>
#include <QDebug>
#include <QDir>
#include <QDateTime>

class Logger
{

private:
    static Logger* logger;
public:
    Logger();
    Logger(const Logger& logger);
    virtual ~Logger();

    static Logger* getLogger();

    static bool deleteLogger();




private:

    QFile* file;
    QTextStream* outStream;
    QDate* date;
    QTime* time;

public:
    bool startLogging();
    bool stopLogging();
    void writeLog(QString logMessage);
};

#endif // LOGGER_H
