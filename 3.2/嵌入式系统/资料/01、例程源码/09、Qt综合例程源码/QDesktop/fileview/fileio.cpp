/******************************************************************
Copyright © Deng Zhimao Co., Ltd. 1990-2030. All rights reserved.
* @projectName   fileview
* @brief         fileio.cpp
* @author        Deng Zhimao
* @email         1252699831@qq.com
* @date          2020-09-10
* @note          Open files must be in UTF-8 format
*******************************************************************/
#include "fileio.h"
#include "QTextCodec"
#include <QFile>
 
FileIO::FileIO(QObject *parent) :
    QObject(parent)
{
 
}
 
QString FileIO::read()
{
    if (mSource.isEmpty()){
        emit error("source is empty");
        return QString();
    }
 
    QFile file(mSource);
    QString fileContent;
    if ( file.open(QIODevice::ReadOnly | QIODevice::Text) ) {
        QString line;
        QTextCodec *code = QTextCodec::codecForName("UTF-8");
        QTextStream t( &file );
        t.setCodec(code);
        do {
            line = t.readLine();
            fileContent += line + "\n";
         } while (!line.isNull());
 
        file.close();
    } else {
        emit error("Unable to open the file");
        return QString();
    }
    return fileContent;
}
 
bool FileIO::write(const QString& data)
{
    if (mSource.isEmpty())
        return false;
 
    QFile file(mSource);
    if (!file.open(QFile::WriteOnly | QFile::Truncate))
        return false;
 
    QTextStream out(&file);
    out << data;
 
    file.close();
 
    return true;
}
