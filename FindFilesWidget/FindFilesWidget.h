#ifndef FINDFILESWIDGET_H
#define FINDFILESWIDGET_H

#include "FindWordInFileThread.h"
#include <QtWidgets>

class FindFilesWidget : public QWidget
{
    Q_OBJECT

public:
    FindFilesWidget(QWidget* parent = 0);
    ~FindFilesWidget();

public slots:
    void startClicked();
    void selectDirectoryClicked();
    void progressChanged(qint64 bytesProcessed);
    void fileFound(const QString& fileName);
    void closeEvent(QCloseEvent* event);

private:
    enum ActionState
    {
        RUNNING,
        STOPPED,
    };

    QGroupBox* createSelectDirectoryGroup();
    QGroupBox* createSearchWordGroup();
    QGroupBox* createProgressBarGroup();
    QGroupBox* createFoundFilesGroup();

    QLineEdit* mSearchDirectoryLine;
    QListWidget* mFoundFilesList;
    QLineEdit* mSearchWordLine;
    QPushButton* mStartButton;
    QProgressBar* mProgressBar;
    ActionState mActionState;
    std::vector<FindWordInFileThread*> mThreads;
    qint64 mDirectorySizeInBytes;
    qint64 mBytesProcessed;
    std::vector<QString> mPaths;
};

#endif // FINDFILESWIDGET_H
