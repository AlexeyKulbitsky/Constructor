#include "logger.h"

Logger* Logger::logger = NULL;

Logger::Logger()
{
    date = new QDate();
    time = new QTime();
    file = new QFile();
}

Logger::~Logger()
{
    if (file)
        delete file;
    file = NULL;
    if (time)
        delete time;
    time = NULL;
    if (date)
        delete date;
    date = NULL;

}

Logger *Logger::getLogger()
{
    if (!logger)
    {
        logger = new Logger();
    }
    return logger;
}

bool Logger::deleteLogger()
{
    if (logger)
    {
        delete logger;
        logger = NULL;
        return true;
    }
    return false;
}







void Logger::writeLog(QString logMessage)
{
    *outStream << time->currentTime().toString("hh:mm:ss") << " " << logMessage << "\n";
}

bool Logger::startLogging()
{
    if (!QDir(QApplication::applicationDirPath() + "/logs/").exists())
    {
        QDir dir(QApplication::applicationDirPath());
        dir.mkdir("logs/");
    }
    QString fname = QApplication::applicationDirPath() + "/logs/log_" + QDateTime::currentDateTime().toString("yyyy-MM-dd_hh-mm-ss") + ".txt";
    file->setFileName(fname);
    if (file->open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append))
    {
        outStream = new QTextStream(file);
        *outStream << "Started logging\n";
        return true;
    }
    else
    {
        throw (QString("Cannot open(create) LOG file by path: " + fname));
        return false;
    }

}

bool Logger::stopLogging()
{
    if (file->isOpen())
    {
        file->close();
        return true;
    }
    return true;
}

