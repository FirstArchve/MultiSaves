#include "filecopy.h"
#include <QFile>
#include <QDir>

//文件拷贝函数
bool FileCopy::CopyFile(QString sourceDir ,QString toDir, bool coverFileIfExist)
{
    toDir.replace("\\","/");
    if (sourceDir == toDir)
    {
        return true;
    }
    if (!QFile::exists(sourceDir))
    {
        return false;
    }
    QDir *createfile = new QDir;
    bool exist = createfile->exists(toDir);
    if (exist)
    {
        if(coverFileIfExist)
        {
            createfile->remove(toDir);
        }
    }

    if(!QFile::copy(sourceDir, toDir))
    {
        return false;
    }
    return true;
}

//文件夹拷贝函数
bool FileCopy::CopyDir(const QString &fromDir, const QString &toDir, bool coverFileIfExist)
{
    QDir sourceDir(fromDir);
    QDir targetDir(toDir);
    //< 如果目标目录不存在，则进行创建
    if(!targetDir.exists())
    {
        if(!targetDir.mkdir(targetDir.absolutePath()))
        {
            return false;
        }
    }

    QFileInfoList fileInfoList = sourceDir.entryInfoList();
    foreach(QFileInfo fileInfo, fileInfoList)
    {
        if(fileInfo.fileName() == "." || fileInfo.fileName() == "..")
        {
            continue;
        }
        //当为目录时，递归的进行copy
        if(fileInfo.isDir())
        {
            if(!CopyFile(fileInfo.filePath(), targetDir.filePath(fileInfo.fileName()),coverFileIfExist))
            {
                return false;
            }
        }
        //当允许覆盖操作时，将旧文件进行删除操作
        else
        {
            if(coverFileIfExist && targetDir.exists(fileInfo.fileName()))
            {
                targetDir.remove(fileInfo.fileName());
            }

            //进行文件copy
            if(!QFile::copy(fileInfo.filePath(), targetDir.filePath(fileInfo.fileName()))){
                    return false;
            }
        }
    }
    return true;
}
