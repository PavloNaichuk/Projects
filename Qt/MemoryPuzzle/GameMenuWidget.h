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

public slots:
    void resumeClicked();
    void exitGameClicked();

private:
    QPushButton* mResumeButton;
    QPushButton* mExitGameButton;
};

#endif // GAMEMENUWIDGET_H
