#ifndef LOGGER_H
#define LOGGER_H


#include <QFile>
#include <QDate>
#include <QTime>
#include <QApplication>
#include <QDebug>
#include <QDir>
#include <QDateTime>

enum MessageType {INFO_T = 0, WARNING_T, ERROR_T, FATAL_T};

class Logger: public QObject
{
    Q_OBJECT
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
    QString buffer;
    QDate* date;
    QTime* time;
    bool writeInfo;
    bool writeError;
    bool writeWarning;
    bool writeFatal;
public:
    bool startLogging();
    bool stopLogging();
    void writeLog(QString logMessage, MessageType type = INFO_T);
    void writeLog(QString logMessage, QString description, MessageType type = INFO_T);
    QTextStream& stream();
    QTextStream& infoLog();
    QTextStream& errorLog();
    QTextStream& fatalLog();
    QTextStream& warningLog();

public slots:
    void setWriteInfoStatus(bool status);
    void setWriteWarningStatus(bool status);
    void setWriteErrorStatus(bool status);
    void setWiteFatalStatus(bool status);
};

#endif // LOGGER_H
