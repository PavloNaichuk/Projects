#ifndef FINDFILESWIDGET_H
#define FINDFILESWIDGET_H

#include "FindWordInFilesThread.h"
#include "ProgressWidget.h"
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
    void fileFound(const QString& fileName);
    void closeEvent(QCloseEvent* event);

    void searchFinished();
    void searchContinued();
    void searchPaused();
    void searchCancelled();

private:
    QGroupBox* createSelectDirectoryGroup();
    QGroupBox* createSearchWordGroup();
    QGroupBox* createFoundFilesGroup();

    QLineEdit* mSearchDirectoryLine;
    QListWidget* mFoundFilesList;
    QLineEdit* mSearchWordLine;
    QPushButton* mStartButton;
    ProgressWidget* mProgressWidget;
    std::vector<FindWordInFilesThread*> mThreads;
    std::vector<QString> mPaths;
};

#endif // FINDFILESWIDGET_H
