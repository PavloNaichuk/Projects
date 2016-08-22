#include "MainWidget.h"
#include "CardBoard.h"
#include "CardBoardWidget.h"
#include "GameLogic.h"
#include <QKeyEvent>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QTime>

MainWidget::MainWidget(GameLogic* gameLogic, QWidget* parent)
    : QWidget(parent)
    , mGameLogic(gameLogic)
    , mCardBoardWidget(new CardBoardWidget(gameLogic))
{
    setWindowTitle("Memory Puzzle");
    setWindowModality(Qt::WindowModal);

    QFont textFont("Times", 12, QFont::Bold);

    mLevelLabel = new QLabel(QString("Level: %1").arg(mGameLogic->getCurrentLevel()));
    mLevelLabel->setAlignment(Qt::AlignLeft);
    mLevelLabel->setFont(textFont);

    mScoreLabel = new QLabel(QString("Score: %1").arg(mGameLogic->getTotalScore()));
    mScoreLabel->setAlignment(Qt::AlignCenter);
    mScoreLabel->setFont(textFont);

    mTimeLabel = new QLabel(QString("Time: %1").arg(QTime(0, 0, 0).toString("hh:mm:ss")));
    mTimeLabel->setAlignment(Qt::AlignRight);
    mTimeLabel->setFont(textFont);

    QHBoxLayout* hLayout = new QHBoxLayout;
    hLayout->addWidget(mLevelLabel);
    hLayout->addWidget(mScoreLabel);
    hLayout->addWidget(mTimeLabel);

    mLevelLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    mScoreLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    mTimeLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    mCardBoardWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    QVBoxLayout* vLayout = new QVBoxLayout;
    vLayout->addLayout(hLayout);
    vLayout->addWidget(mCardBoardWidget);

    resize(800, 600);
    setLayout(vLayout);
    connect(mGameLogic, SIGNAL(scoreChanged(int)), this, SLOT(scoreUpdated(int)));
    connect(mGameLogic, SIGNAL(timeChanged(int)), this, SLOT(timeUpdated(int)));
}

MainWidget::~MainWidget()
{
    delete mCardBoardWidget;
    delete mLevelLabel;
    delete mScoreLabel;
    delete mTimeLabel;
}

void MainWidget::setCardBoard(CardBoard* cardBoard)
{
    mCardBoardWidget->setCardBoard(cardBoard);
    levelUpdated();
}

const CardBoard* MainWidget::getCardBoard() const
{
    return mCardBoardWidget->getCardBoard();
}

void MainWidget::levelUpdated()
{
    mLevelLabel->setText(QString("Level: %1").arg(mGameLogic->getCurrentLevel()));
}

void MainWidget::scoreUpdated(int score)
{
    mScoreLabel->setText(QString("Score: %1").arg(score));
}

void MainWidget::timeUpdated(int elapsedMs)
{
    QTime time = QTime::fromMSecsSinceStartOfDay(elapsedMs);
    QString timeString = QString("Time: %1").arg(time.toString("hh:mm:ss"));
    mTimeLabel->setText(timeString);
}

void MainWidget::keyPressEvent(QKeyEvent* event)
{
     if (event->key() == Qt::Key_Escape)
     {
        emit gameMenuRequested();
     }
}
