#include "ProgressWidget.h"
#include <cassert>

ProgressWidget::ProgressWidget(qint64 directorySizeInBytes, QDialog *parent)
    : QDialog(parent)
    , mActionState(RUNNING)
    , mDirectorySizeInBytes(directorySizeInBytes)
    , mBytesProcessed(0)
{
    setWindowTitle("Search Progress");
    setModal(true);

    mProgressBar = new QProgressBar();
    mProgressBar->setMaximum(100);
    mProgressBar->setMinimumWidth(200);
    mProgressBar->setAlignment(Qt::AlignCenter);
    mProgressBar->setValue(0);

    mPauseOrContinueButton = new QPushButton("&Pause");
    mCancelButton = new QPushButton("&Cancel");

    QHBoxLayout* hLayout = new QHBoxLayout();
    hLayout->addWidget(mPauseOrContinueButton);
    hLayout->addWidget(mCancelButton);
    hLayout->setSpacing(6);
    hLayout->setContentsMargins(11, 11, 11, 11);

    QVBoxLayout* vLayout = new QVBoxLayout();
    vLayout->addWidget(mProgressBar);
    vLayout->addLayout(hLayout);

    connect(mPauseOrContinueButton, SIGNAL(clicked()), SLOT(pauseClicked()));
    connect(mCancelButton, SIGNAL(clicked()), SLOT(cancelClicked()));

    setLayout(vLayout);
    resize(300, 100);
}

ProgressWidget::~ProgressWidget()
{
    delete mPauseOrContinueButton;
    delete mCancelButton;
}

void ProgressWidget::pauseClicked()
{
    if ((mActionState == STOPPED) || (mActionState == PAUSED))
    {
        mActionState = RUNNING;
        mPauseOrContinueButton->setText("&Pause");
        emit continued();
    }
    else if (mActionState == RUNNING)
    {
        mActionState = PAUSED;
        mPauseOrContinueButton->setText("&Continue");
        emit paused();
    }
    else
    {
        assert(false);
    }
}

void ProgressWidget::cancelClicked()
{
    emit canceled();
    close();
}

void ProgressWidget::restart(qint64 directorySizeInBytes)
{
    mProgressBar->setValue(0);
    mBytesProcessed = 0;
    mDirectorySizeInBytes = directorySizeInBytes;
}

void ProgressWidget::progressChanged(qint64 bytesProcessed)
{
    mBytesProcessed += bytesProcessed;

    int progressValue = 100 * ((float)mBytesProcessed / (float)mDirectorySizeInBytes);
    mProgressBar->setValue(progressValue);

    if (mBytesProcessed == mDirectorySizeInBytes)
    {
        close();
        emit finished();
    }
}

void ProgressWidget::closeEvent(QCloseEvent* event)
{
    if (mBytesProcessed != mDirectorySizeInBytes)
    {
        emit paused();

        QMessageBox::StandardButton message = QMessageBox::question(this, "Search Progress", tr("Are you sure you want to exit?"), QMessageBox::Cancel | QMessageBox::No | QMessageBox::Yes, QMessageBox::Yes);
        if (message == QMessageBox::Yes)
        {
            event->accept();
            emit canceled();
        }
        else
        {
            event->ignore();
            emit continued();
        }
    }
    else
    {
        close();
        emit finished();
    }
}
