#include "MainWidget.h"
#include "CardBoard.h"
#include "CardBoardWidget.h"
#include "LevelGenerator.h"

#include <QHBoxLayout>
#include <QVBoxLayout>

MainWidget::MainWidget(QWidget* parent)
    : QWidget(parent)
    , mTime(new QTime(0,0))
    , mTimer(new QTimer())
    , mCardBoardWidget(new CardBoardWidget())

{
    mLevelLabel = new QLabel("Level: 1");
    mScoreLabel = new QLabel("Score:");

    mTimeLabel = new QLabel();
    mTimeLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    mTimeLabel->setText(QString("Time: ") + mTime->toString("hh:mm:ss"));

    mTime->start();
    mTimer->start(1000);
    connect(mTimer, SIGNAL(timeout()), this, SLOT(timeUpdate()));

    QHBoxLayout* hLayout = new QHBoxLayout;
    hLayout->addWidget(mLevelLabel);
    hLayout->addWidget(mScoreLabel);
    hLayout->addWidget(mTimeLabel);

    mLevelLabel->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
    mScoreLabel->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
    mTimeLabel->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
    mCardBoardWidget->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);

    QVBoxLayout* vLayout = new QVBoxLayout;
    vLayout->addLayout(hLayout);
    vLayout->addWidget(mCardBoardWidget);

    resize(800, 600);
    setLayout(vLayout);
}

MainWidget::~MainWidget()
{
    delete mTimer;
    delete mTime;
    delete mCardBoardWidget;
    delete mLevelLabel;
    delete mScoreLabel;
    delete mTimeLabel;
}

void MainWidget::setCardBoard(CardBoard* cardBoard)
{
    mCardBoardWidget->setCardBoard(cardBoard);
}

const CardBoard* MainWidget::getCardBoard() const
{
    return mCardBoardWidget->getCardBoard();
}

void MainWidget::timeUpdate()
{
    int elapsedTimeMs = mTime->elapsed();
    int elapsedTimeSec = elapsedTimeMs / 1000;
    mTimeLabel->setText(QString("Time: ") + QTime(0, 0, elapsedTimeSec).toString("hh:mm:ss"));
}

