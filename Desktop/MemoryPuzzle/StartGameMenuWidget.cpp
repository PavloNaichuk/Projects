#include "StartGameMenuWidget.h"
#include <QHBoxLayout>
#include <QPushButton>

StartGameMenuWidget::StartGameMenuWidget(QDialog* parent)
    : QDialog(parent)
{
    setWindowTitle("Start Menu");
    setWindowFlags(Qt::Window | Qt::WindowTitleHint | Qt::CustomizeWindowHint);
    setModal(true);

    mStartGameButton = new QPushButton("&Start Game");
    mShowHelpButton = new QPushButton("&Show Help");
    mExitGameButton = new QPushButton("&Exit Game");

    QVBoxLayout* layout = new QVBoxLayout;
    layout->addWidget(mStartGameButton);
    layout->addWidget(mShowHelpButton);
    layout->addWidget(mExitGameButton);
    setLayout(layout);

    resize(250, 100);
    connect(mStartGameButton, SIGNAL(clicked()), this, SLOT(startGameClicked()));
    connect(mShowHelpButton, SIGNAL(clicked()), this, SLOT(showHelpClicked()));
    connect(mExitGameButton, SIGNAL(clicked()), this, SLOT(exitGameClicked()));
}

StartGameMenuWidget::~StartGameMenuWidget()
{
    delete mStartGameButton;
    delete mShowHelpButton;
    delete mExitGameButton;
}

void StartGameMenuWidget::startGameClicked()
{
    emit startGame();
    close();
}

void StartGameMenuWidget::exitGameClicked()
{
    emit exitGame();
    close();
}

void StartGameMenuWidget::showHelpClicked()
{
    emit showHelp();
    show();
}
