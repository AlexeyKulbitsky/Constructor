#include "filemanager.h"

QString FileManager::autoSaveFileName = "auto_save";
int FileManager::autoSaveTime = 5;
bool FileManager::autoSaveOn = true;
QTimer FileManager::timer;

void FileManager::addMap(QWebView *map)
{
    if (map)
        maps.push_back(map);
}

void FileManager::setAutoSaveFileName(QString fileName)
{
    autoSaveFileName = fileName;
}

void FileManager::setAutoSaveTime(int time)
{
    if (!autoSaveOn)
        return;
    autoSaveTime = time;
    timer.stop();
    timer.setInterval(1000 * 60 * time);
    timer.start();

}

void FileManager::setAutoSaveOn(bool status)
{
    autoSaveOn = status;
    if (autoSaveOn)
        timer.start();
}
