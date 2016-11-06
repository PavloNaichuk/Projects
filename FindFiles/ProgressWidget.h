#ifndef PROGRESSWIDGET_H
#define PROGRESSWIDGET_H

#include "FindWordInFilesThread.h"
#include <QtWidgets>

class ProgressWidget : public QDialog
{
    Q_OBJECT

public:
    enum ActionState
    {
        STOPPED,
        PAUSED,
        RUNNING,
    };

    ProgressWidget(qint64 directorySizeInBytes, QDialog* parent = 0);
    ~ProgressWidget();

    void restart(qint64 directorySizeInBytes);

signals:
    void finished();
    void continued();
    void paused();
    void canceled();

public slots:
    void progressChanged(qint64 bytesProcessed);
    void pauseClicked();
    void cancelClicked();
    void closeEvent(QCloseEvent* event);

private:
    QProgressBar* mProgressBar;
    QPushButton* mPauseOrContinueButton;
    QPushButton* mCancelButton;
    ActionState mActionState;
    qint64 mDirectorySizeInBytes;
    qint64 mBytesProcessed;
};

#endif // PROGRESSWIDGET_H
