#include "FindFilesWidget.h"
#include "FindWordInFileThread.h"

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
{
    setWindowTitle("Search Files");

    QVBoxLayout* vLayout = new QVBoxLayout(this);
    vLayout->setSpacing(6);
    vLayout->setContentsMargins(11, 11, 11, 11);
    vLayout->addWidget(createSelectDirectoryGroup());
    vLayout->addWidget(createSearchWordGroup());
    vLayout->addWidget(createFoundFilesGroup());
    setLayout(vLayout);

    resize(600, 600);
}

void FindFilesWidget::startClicked()
{
    const int idealThreadCount = QThread::idealThreadCount();

    std::vector<QThread*> threads;
    threads.reserve(idealThreadCount);

    QDirIterator dirIt(mSearchDirectoryLine->text(), QDirIterator::Subdirectories);
    std::vector<QString> paths;
    while (dirIt.hasNext())
    {
        dirIt.next();
        if (QFileInfo(dirIt.filePath()).isFile())
        {
            if (QFileInfo(dirIt.filePath()).suffix() == "txt")
                paths.push_back(dirIt.filePath());
        }
    }
    const std::vector<Range> ranges = GenerateRanges(idealThreadCount, paths.size());
    std::vector<QString> foundFiles;

    for (size_t index = 0; index < idealThreadCount; ++index)
        threads.push_back(new FindWordInFileThread(paths, mSearchWordLine->text(), ranges[index].mStart, ranges[index].mLength, foundFiles));
    for (size_t index = 0; index < idealThreadCount; ++index)
        threads[index]->start();
    for (size_t index = 0; index < idealThreadCount; ++index)
        threads[index]->wait();

    std::sort(foundFiles.begin(), foundFiles.end());

    for (size_t index = 0; index < idealThreadCount; ++index)
        delete threads[index];

    mFoundFilesList->clear();
    for (size_t index = 0; index < foundFiles.size(); ++index)
        mFoundFilesList->addItem(foundFiles[index]);
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
    QPushButton* startButton = new QPushButton("&Start");

    QHBoxLayout* hLayout = new QHBoxLayout();
    hLayout->setSpacing(6);
    hLayout->setContentsMargins(11, 11, 11, 11);
    hLayout->addWidget(mSearchWordLine);
    hLayout->addWidget(startButton);

    connect(startButton, SIGNAL(clicked()), SLOT(startClicked()));

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
