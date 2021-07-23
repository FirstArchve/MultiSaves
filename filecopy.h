#ifndef FILECOPY_H
#define FILECOPY_H

#include <QString>

class FileCopy
{
public:
    bool CopyFile(QString sourceDir ,QString toDir, bool coverFileIfExist);

    bool CopyDir(const QString &fromDir, const QString &toDir, bool coverFileIfExist);
};

#endif // FILECOPY_H
