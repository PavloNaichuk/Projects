#ifndef FINDFILESWIDGET_H
#define FINDFILESWIDGET_H

#include <QtWidgets>

class FindFilesWidget : public QWidget
{
    Q_OBJECT

public:
    FindFilesWidget(QWidget* parent = 0);

public slots:
    void startClicked();
    void selectDirectoryClicked();

private:
    QGroupBox* createSelectDirectoryGroup();
    QGroupBox* createSearchWordGroup();
    QGroupBox* createFoundFilesGroup();

    QLineEdit* mSearchDirectoryLine;
    QListWidget* mFoundFilesList;
    QLineEdit* mSearchWordLine;
};

#endif // FINDFILESWIDGET_H
