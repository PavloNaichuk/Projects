#include "FindFilesWidget.h"

FindFilesWidget::FindFilesWidget(QWidget *parent)
    : QWidget(parent)
    , mProgressWidget(nullptr)
{
    setWindowTitle("Find Files");

    QVBoxLayout* vLayout = new QVBoxLayout(this);
    vLayout->setSpacing(6);
    vLayout->setContentsMargins(11, 11, 11, 11);
    vLayout->addWidget(createSelectDirectoryGroup());
    vLayout->addWidget(createSearchWordGroup());
    vLayout->addWidget(createFoundFilesGroup());
    setLayout(vLayout);
    resize(600, 600);
}

FindFilesWidget::~FindFilesWidget()
{
    for (size_t index = 0; index < mThreads.size(); ++index)
        delete mThreads[index];
    delete mProgressWidget;
}

void FindFilesWidget::startClicked()
{
    mFoundFilesList->clear();
    const int idealThreadCount = QThread::idealThreadCount();
    for (size_t index = 0; index < mThreads.size(); ++index)
        delete mThreads[index];
    mThreads.clear();
    mThreads.reserve(idealThreadCount);

    mPaths.clear();
    QDirIterator dirIt(mSearchDirectoryLine->text(), QDirIterator::Subdirectories);
    qint64 directorySizeInBytes = 0;
    while (dirIt.hasNext())
    {
        dirIt.next();
        QFileInfo fileInfo(dirIt.filePath());
        if (fileInfo.isFile())
        {
            if (fileInfo.suffix() == "txt")
            {
                mPaths.push_back(dirIt.filePath());
                directorySizeInBytes += fileInfo.size();
            }
        }
    }

    const std::vector<Range> ranges = GenerateRanges(idealThreadCount, mPaths.size());
    for (size_t index = 0; index < idealThreadCount; ++index)
        mThreads.push_back(new FindWordInFilesThread(mPaths, mSearchWordLine->text(), ranges[index].mStart, ranges[index].mLength));

    if (mProgressWidget == nullptr)
    {
         mProgressWidget = new ProgressWidget(directorySizeInBytes);

         QObject::connect(mProgressWidget, SIGNAL(finished()), this, SLOT(searchFinished()));
         QObject::connect(mProgressWidget, SIGNAL(paused()), this, SLOT(searchPaused()));
         QObject::connect(mProgressWidget, SIGNAL(continued()), this, SLOT(searchContinued()));
         QObject::connect(mProgressWidget, SIGNAL(canceled()), this, SLOT(searchCancelled()));
    }

    mProgressWidget->restart(directorySizeInBytes);
    mProgressWidget->show();

    for (size_t index = 0; index < idealThreadCount; ++index)
    {
        QObject::connect(mThreads[index], SIGNAL(progressChanged(qint64)), mProgressWidget, SLOT(progressChanged(qint64)));
        QObject::connect(mThreads[index], SIGNAL(fileFound(const QString&)), this, SLOT(fileFound(const QString&)));
    }

    for (size_t index = 0; index < idealThreadCount; ++index)
        mThreads[index]->start();

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

void FindFilesWidget::searchFinished()
{
}

void FindFilesWidget::searchContinued()
{
    for (size_t index = 0; index < mThreads.size(); ++index)
        mThreads[index]->resume();
}

void FindFilesWidget::searchPaused()
{
    for (size_t index = 0; index < mThreads.size(); ++index)
        mThreads[index]->pause();
}

void FindFilesWidget::searchCancelled()
{
    for (size_t index = 0; index < mThreads.size(); ++index)
        mThreads[index]->cancel();
}

void FindFilesWidget::closeEvent(QCloseEvent* event)
{
    QMessageBox::StandardButton message = QMessageBox::question(this, "Find Files", tr("Are you sure you want to exit?"), QMessageBox::Cancel | QMessageBox::No | QMessageBox::Yes, QMessageBox::Yes);
    if (message == QMessageBox::Yes)
        event->accept();
    else
        event->ignore();
}
