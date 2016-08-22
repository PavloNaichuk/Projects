#pragma once

#include <QDialog>

class QPushButton;

class LevelCompletedWidget : public QDialog
{
    Q_OBJECT

public:
   LevelCompletedWidget(QDialog* parent = 0);
    ~LevelCompletedWidget();

signals:
    void nextLevel();
    void replayLevel();
    void exitGame();

public slots:
    void nextLevelClicked();
    void replayLevelClicked();
    void exitGameClicked();

private:
    QPushButton* mNextLevelButton;
    QPushButton* mReplayLevelButton;
    QPushButton* mExitGameButton;
};
