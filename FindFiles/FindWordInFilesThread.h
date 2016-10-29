#ifndef FIND_WORD_IN_FILES_THREAD_H
#define FIND_WORD_IN_FILES_THREAD_H

#include <QThread>
#include <QString>


class FindWordInFilesThread : public QThread
{
    Q_OBJECT

public:
    FindWordInFilesThread(const std::vector<QString>& filePath, const QString& word, size_t startIndex, size_t numElements);
    virtual void run();
    void cancel();
    void pause();
    void resume();

signals:
    void fileFound(const QString&);
    void progressChanged(qint64);

private:
    const std::vector<QString>& mFilePath;
    const QString mWord;
    size_t mStartIndex;
    size_t mNumElements;
    bool mIsCancelled;
    bool mIsPaused;
};

#endif // FIND_WORD_IN_FILES_THREAD_H
