#include "FindWordInFileThread.h"

#include <QFile>
#include <QTextStream>
#include <QFileInfo>

FindWordInFileThread::FindWordInFileThread(const std::vector<QString>& filePath, const QString& word, size_t startIndex, size_t numElements, std::vector<QString>& foundFiles)
    : mFilePath(filePath)
    , mWord(word)
    , mStartIndex(startIndex)
    , mNumElements(numElements)
    , mFoundFiles(foundFiles)
{}

void FindWordInFileThread::run()
{
    for (size_t index = mStartIndex; index < mStartIndex + mNumElements; ++index)
    {
        QFile file(mFilePath[index]);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
            return;

        QTextStream textStream(&file);
        while (!textStream.atEnd())
        {
            QString line = textStream.readLine();
            if (line.contains(mWord, Qt::CaseInsensitive))
            {
                mFoundFiles.push_back(QFileInfo(mFilePath[index]).filePath());
                break;
            }
        }
        file.close();
    }
}
