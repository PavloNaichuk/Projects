#include "FindWordInFilesThread.h"

#include <QFile>
#include <QTextStream>
#include <QDebug>

Range::Range(size_t start, size_t length)
    : mStart(start)
    , mLength(length)
{}

std::vector<Range> GenerateRanges(size_t numRanges, size_t numFiles)
{
    std::vector<Range> ranges;
    ranges.reserve(numRanges);

    const size_t minLength = numFiles / numRanges;
    const size_t modulo = numFiles % numRanges;

    for (size_t index = 0; index < modulo; ++index)
        ranges.push_back(Range(0, minLength + 1));

    for (size_t index = modulo; index < numRanges; ++index)
        ranges.push_back(Range(0, minLength));

    for (size_t index = 1; index < numRanges; ++index)
        ranges[index].mStart = ranges[index - 1].mStart + ranges[index - 1].mLength;

    return ranges;
}

FindWordInFilesThread::FindWordInFilesThread(const std::vector<QString>& filePath, const QString& word, size_t startIndex, size_t numElements)
    : mFilePath(filePath)
    , mWord(word)
    , mStartIndex(startIndex)
    , mNumElements(numElements)
    , mIsCancelled(false)
    , mIsPaused(false)
{}

void FindWordInFilesThread::run()
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

void FindWordInFilesThread::cancel()
{
    mIsCancelled = true;
}

void FindWordInFilesThread::pause()
{
    mIsPaused = true;
}

void FindWordInFilesThread::resume()
{
    mIsPaused = false;
}
