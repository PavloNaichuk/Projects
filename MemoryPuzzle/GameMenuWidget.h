#ifndef GAMEMENUWIDGET_H
#define GAMEMENUWIDGET_H

#include <QDialog>

class QPushButton;

class GameMenuWidget : public QDialog
{
    Q_OBJECT

public:
    GameMenuWidget(QDialog* parent = 0);
    ~GameMenuWidget();

signals:
    void resume();
    void exitGame();
    void showHelp();

public slots:
    void resumeClicked();
    void exitGameClicked();
    void showHelpClicked();

private:
    QPushButton* mResumeButton;
    QPushButton* mExitGameButton;
    QPushButton* mShowHelpButton;
};

#endif // GAMEMENUWIDGET_H
