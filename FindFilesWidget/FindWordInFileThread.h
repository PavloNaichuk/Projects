#ifndef FIND_WORD_IN_FILE_THREAD_H
#define FIND_WORD_IN_FILE_THREAD_H

#include <QThread>
#include <QString>
#include <QtDebug>

class FindWordInFileThread : public QThread
{
public:
    FindWordInFileThread(const std::vector<QString>& filePath, const QString& word, size_t startIndex, size_t numElements, std::vector<QString>& foundFiles);
    virtual void run();

private:
    const std::vector<QString>& mFilePath;
    const QString mWord;
    size_t mStartIndex;
    size_t mNumElements;
    std::vector<QString>& mFoundFiles;
};

#endif // FIND_WORD_IN_FILE_THREAD_H
