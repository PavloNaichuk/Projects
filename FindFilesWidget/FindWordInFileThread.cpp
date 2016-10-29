#include "FindWordInFileThread.h"

#include <QFile>
#include <QTextStream>
#include <QDebug>

FindWordInFileThread::FindWordInFileThread(const std::vector<QString>& filePath, const QString& word, size_t startIndex, size_t numElements)
    : mFilePath(filePath)
    , mWord(word)
    , mStartIndex(startIndex)
    , mNumElements(numElements)
    , mIsCancelled(false)
    , mIsPaused(false)
{}

void FindWordInFileThread::run()
{
    mIsCancelled = false;
    mIsPaused = false;
    for (size_t index = mStartIndex; (index < mStartIndex + mNumElements) && !mIsCancelled; ++index)
    {
        while (mIsPaused)
        {
            sleep(1);
        }
        QFile file(mFilePath[index]);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
           return;

        QTextStream textStream(&file);
        while (!textStream.atEnd() && !mIsCancelled)
        {
            while (mIsPaused)
            {
                sleep(1);
            }
            QString line = textStream.readLine();
            if (line.contains(mWord, Qt::CaseInsensitive))
            {
                emit fileFound(file.fileName());
                break;
            }
        }
        emit progressChanged(file.size());
        file.close();
    }
}

void FindWordInFileThread::cancel()
{
    mIsCancelled = true;
}

void FindWordInFileThread::pause()
{
    mIsPaused = true;
}

void FindWordInFileThread::resume()
{
    mIsPaused = false;
}
