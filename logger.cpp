#include "logger.h"

Logger* Logger::logger = NULL;


Logger::Logger()
{
    date = new QDate();
    time = new QTime();
    file = new QFile();

    writeInfo = true;
    writeError = true;
    writeWarning = true;
    writeFatal = true;
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


void Logger::writeLog(QString logMessage, MessageType type)
{
    QString t;
    bool write = false;
    switch (type)
    {
    case INFO_T:
        write = writeInfo;
        t = "INFO";
        break;
    case WARNING_T:
        write = writeWarning;
        t = "WARNING";
        break;
    case ERROR_T:
        write = writeError;
        t = "ERROR";
        break;
    case FATAL_T:
        write = writeFatal;
        t = "FATAL";
        break;
    default:
        break;
    }

    if (write)
        *outStream << time->currentTime().toString("hh:mm:ss")
                   << " " << t
                   << " " << logMessage << "\n";

}

void Logger::writeLog(QString logMessage, QString description, MessageType type)
{
    QString t;
    bool write = false;
    switch (type)
    {
    case INFO_T:
        write = writeInfo;
        t = "INFO";
        break;
    case WARNING_T:
        write = writeWarning;
        t = "WARNING";
        break;
    case ERROR_T:
        write = writeError;
        t = "ERROR";
        break;
    case FATAL_T:
        write = writeFatal;
        t = "FATAL";
        break;
    default:
        break;
    }

    if (write)
        *outStream << time->currentTime().toString("hh:mm:ss")
                   << " " << t
                   << " " << logMessage
                   << " " << description
                   << "\n";

}


inline QTextStream& Logger::stream()
{
    return *outStream;
}

QTextStream& Logger::infoLog()
{
    return *outStream << time->currentTime().toString("hh:mm:ss") << " | INFO | ";
}

QTextStream& Logger::errorLog()
{
    return *outStream << time->currentTime().toString("hh:mm:ss") << " | ERROR | ";
}

QTextStream& Logger::fatalLog()
{
    return *outStream << time->currentTime().toString("hh:mm:ss") << " | FATAL| ";
}

QTextStream& Logger::warningLog()
{
    return *outStream << time->currentTime().toString("hh:mm:ss") << " | WARNING| ";
}



void Logger::setWriteInfoStatus(bool status)
{
    writeInfo = status;
}

void Logger::setWriteWarningStatus(bool status)
{
    writeWarning = status;
}

void Logger::setWriteErrorStatus(bool status)
{
    writeError = status;
}

void Logger::setWiteFatalStatus(bool status)
{
    writeFatal = status;
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

