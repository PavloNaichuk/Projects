#include "FindFilesWidget.h"

struct Range
{
    Range(size_t start, size_t length);

    size_t mStart;
    size_t mLength;
};

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

FindFilesWidget::FindFilesWidget(QWidget *parent)
    : QWidget(parent)
    , mActionState(ActionState::STOPPED)
    , mDirectorySizeInBytes(0)
    , mBytesProcessed(0)
{
    setWindowTitle("Find Files");

    QVBoxLayout* vLayout = new QVBoxLayout(this);
    vLayout->setSpacing(6);
    vLayout->setContentsMargins(11, 11, 11, 11);
    vLayout->addWidget(createSelectDirectoryGroup());
    vLayout->addWidget(createSearchWordGroup());
    vLayout->addWidget(createProgressBarGroup());
    vLayout->addWidget(createFoundFilesGroup());
    setLayout(vLayout);
    resize(600, 600);
}

void FindFilesWidget::startClicked()
{
    if (mActionState == ActionState::STOPPED)
    {
        mFoundFilesList->clear();
        mProgressBar->setValue(0);
        mBytesProcessed = 0;
        mActionState = ActionState::RUNNING;
        mStartButton->setText("&Cancel");

        const int idealThreadCount = QThread::idealThreadCount();
        for (size_t index = 0; index < mThreads.size(); ++index)
            delete mThreads[index];
        mThreads.clear();
        mThreads.reserve(idealThreadCount);

        mDirectorySizeInBytes = 0;
        mPaths.clear();
        QDirIterator dirIt(mSearchDirectoryLine->text(), QDirIterator::Subdirectories);
        while (dirIt.hasNext())
        {
            dirIt.next();
            QFileInfo fileInfo(dirIt.filePath());
            if (fileInfo.isFile())
            {
                if (fileInfo.suffix() == "txt")
                {
                    mPaths.push_back(dirIt.filePath());
                    mDirectorySizeInBytes += fileInfo.size();
                }
            }
        }

        const std::vector<Range> ranges = GenerateRanges(idealThreadCount, mPaths.size());
        for (size_t index = 0; index < idealThreadCount; ++index)
            mThreads.push_back(new FindWordInFileThread(mPaths, mSearchWordLine->text(), ranges[index].mStart, ranges[index].mLength));

        for (size_t index = 0; index < idealThreadCount; ++index)
        {
            QObject::connect(mThreads[index], SIGNAL(progressChanged(qint64)), this, SLOT(progressChanged(qint64)));
            QObject::connect(mThreads[index], SIGNAL(fileFound(const QString&)), this, SLOT(fileFound(const QString&)));
        }

        for (size_t index = 0; index < idealThreadCount; ++index)
            mThreads[index]->start();
    }
    else
    {
        mActionState = ActionState::STOPPED;
        mStartButton->setText("&Start");
        for (size_t index = 0; index < mThreads.size(); ++index)
            mThreads[index]->cancel();
    }
}

void FindFilesWidget::progressChanged(qint64 bytesProcessed)
{
    mBytesProcessed += bytesProcessed;
    int progressValue = 100 * ((float)mBytesProcessed / (float)mDirectorySizeInBytes);
    mProgressBar->setValue(progressValue);
    if (mBytesProcessed == mDirectorySizeInBytes)
    {
        mActionState = ActionState::STOPPED;
        mStartButton->setText("&Start");
    }
}

void FindFilesWidget::fileFound(const QString& fileName)
{
    mFoundFilesList->addItem(fileName);
}

void FindFilesWidget::selectDirectoryClicked()
{
    QString selectedDirectory = QFileDialog::getExistingDirectory(this, tr("Open Directory"), mSearchDirectoryLine->text(), QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    mSearchDirectoryLine->setText(selectedDirectory);
}

QGroupBox* FindFilesWidget::createSelectDirectoryGroup()
{
    QGroupBox* selectDirectoryGroup = new QGroupBox("&Select directory:");
    QPushButton* selectDirectoryButton = new QPushButton("Select", selectDirectoryGroup);
    mSearchDirectoryLine = new QLineEdit(tr("D:/"), selectDirectoryGroup);

    QHBoxLayout* hLayout = new QHBoxLayout();
    hLayout->setSpacing(6);
    hLayout->addWidget(mSearchDirectoryLine);
    hLayout->addWidget(selectDirectoryButton);

    connect(selectDirectoryButton, SIGNAL(clicked()), SLOT(selectDirectoryClicked()));

    selectDirectoryGroup->setLayout(hLayout);
    return selectDirectoryGroup;
}

QGroupBox* FindFilesWidget::createSearchWordGroup()
{
    QGroupBox* searchWordGroup = new QGroupBox("&Search word:");
    mSearchWordLine = new QLineEdit(searchWordGroup);
    mStartButton = new QPushButton("&Start");

    QHBoxLayout* hLayout = new QHBoxLayout();
    hLayout->setSpacing(6);
    hLayout->setContentsMargins(11, 11, 11, 11);
    hLayout->addWidget(mSearchWordLine);
    hLayout->addWidget(mStartButton);

    connect(mStartButton, SIGNAL(clicked()), SLOT(startClicked()));

    searchWordGroup->setLayout(hLayout);
    return searchWordGroup;
}

QGroupBox* FindFilesWidget::createProgressBarGroup()
{
    QGroupBox* progressBarGroup = new QGroupBox("&Progress:");
    mProgressBar = new QProgressBar(progressBarGroup);

    QHBoxLayout* hLayout = new QHBoxLayout();
    hLayout->setSpacing(6);
    hLayout->setContentsMargins(11, 11, 11, 11);
    hLayout->addWidget(mProgressBar);
    mProgressBar->setMaximum(100);
    mProgressBar->setMinimumWidth(500);
    mProgressBar->setAlignment(Qt::AlignCenter);

    progressBarGroup->setLayout(hLayout);
    return progressBarGroup;
}

QGroupBox* FindFilesWidget::createFoundFilesGroup()
{
    QGroupBox* foundFilesGroup = new QGroupBox("&Found files:");
    mFoundFilesList = new QListWidget(foundFilesGroup);

    QHBoxLayout* hLayout = new QHBoxLayout();
    hLayout->setSpacing(6);
    hLayout->setContentsMargins(11, 11, 11, 11);
    hLayout->addWidget(mFoundFilesList);

    foundFilesGroup->setLayout(hLayout);
    return foundFilesGroup;
}

void FindFilesWidget::closeEvent(QCloseEvent* event)
{
    for (size_t index = 0; index < mThreads.size(); ++index)
        mThreads[index]->pause();

    QMessageBox::StandardButton message = QMessageBox::question(this, "Find Files", tr("Are you sure?\n"), QMessageBox::Cancel | QMessageBox::No | QMessageBox::Yes, QMessageBox::Yes);
    if (message == QMessageBox::Yes)
    {
        for (size_t index = 0; index < mThreads.size(); ++index)
            mThreads[index]->cancel();

        event->accept();
    }
    else
    {
        for (size_t index = 0; index < mThreads.size(); ++index)
            mThreads[index]->resume();

        event->ignore();
    }
}

FindFilesWidget::~FindFilesWidget()
{
    for (size_t index = 0; index < mThreads.size(); ++index)
        delete mThreads[index];
}
