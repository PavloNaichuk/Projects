#ifndef STARTGAMEMENUWIDGET_H
#define STARTGAMEMENUWIDGET_H

#include <QDialog>

class QPushButton;

class StartGameMenuWidget : public QDialog
{
    Q_OBJECT

public:
    StartGameMenuWidget(QDialog* parent = 0);
    ~StartGameMenuWidget();

signals:
    void startGame();
    void exitGame();
    void showHelp();

public slots:
    void startGameClicked();
    void exitGameClicked();
    void showHelpClicked();

private:
    QPushButton* mStartGameButton;
    QPushButton* mExitGameButton;
    QPushButton* mShowHelpButton;
};

#endif // STARTGAMEMENUWIDGET_H
