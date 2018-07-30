#include "CategoryWidget.h"
#include <QHBoxLayout>
#include <QPushButton>
#include <QSplitter>
#include <QTextEdit>
#include <QScrollBar>
#include <QGroupBox>


CategoryWidget::CategoryWidget(QWidget* parent)
    : QWidget(parent)
{
    setWindowFlags(Qt::Window);

    setFixedWidth(810);
    setFixedHeight(700);


    QGroupBox* groupBoxCategory = new QGroupBox("Назва категорії");
    QTextEdit* textEditCategory = new QTextEdit;
    textEditCategory->setPlainText("");
    textEditCategory->setFixedSize(709, 25);
    QVBoxLayout* vLayoutCategory = new QVBoxLayout;
    vLayoutCategory->addWidget(textEditCategory);
    groupBoxCategory->setLayout(vLayoutCategory);


    QGroupBox* groupBoxTest = new QGroupBox("Назва тесту");
    QTextEdit* textEditTest = new QTextEdit;
    textEditTest->setPlainText("");
    textEditTest->setFixedSize(709, 25);
    QVBoxLayout* vLayoutTest = new QVBoxLayout;
    vLayoutTest->addWidget(textEditTest);
    groupBoxTest->setLayout(vLayoutTest);


    QGroupBox* groupBoxTask = new QGroupBox("Назва запитання");
    QTextEdit* textEditTask = new QTextEdit;
    textEditTask->setPlainText("");
    textEditTask->setFixedSize(709, 25);
    QVBoxLayout* vLayoutTask = new QVBoxLayout;
    vLayoutTask->addWidget(textEditTask);
    groupBoxTask->setLayout(vLayoutTask);



    QGroupBox* groupBoxQuestion = new QGroupBox("Запитання");
    QTextEdit* textEditQuestion = new QTextEdit;
    textEditQuestion->setPlainText("");
    textEditQuestion->setFixedSize(709, 25);
    QVBoxLayout* vLayoutQuestion = new QVBoxLayout;
    vLayoutQuestion->addWidget(textEditQuestion);
    groupBoxQuestion->setLayout(vLayoutQuestion);



    QGroupBox* groupBoxAnswer = new QGroupBox("Відповідь");
    QTextEdit* textEditAnswer = new QTextEdit;
    textEditAnswer->setPlainText("");
    textEditAnswer->setFixedSize(709, 25);
    QVBoxLayout* vLayoutAnswer = new QVBoxLayout;
    vLayoutAnswer->addWidget(textEditAnswer);
    groupBoxAnswer->setLayout(vLayoutAnswer);

    QGroupBox* groupBoxNumPoints = new QGroupBox("Кількість  балів");
    QTextEdit* textEditNumPoints = new QTextEdit;
    textEditNumPoints->setPlainText("");
    textEditNumPoints->setFixedSize(709, 25);
    QVBoxLayout* vLayoutNumPoints = new QVBoxLayout;
    vLayoutNumPoints->addWidget(textEditNumPoints);
    groupBoxNumPoints->setLayout(vLayoutNumPoints);


    mSaveButton = new QPushButton("Зберегти");
    mSaveButton->setFixedSize(75, 25);


    QVBoxLayout* vLayout = new QVBoxLayout;
    vLayout->addWidget(groupBoxCategory);
    vLayout->addWidget(groupBoxTest);
    vLayout->addWidget(groupBoxTask);
    vLayout->addWidget(groupBoxQuestion);
    vLayout->addWidget(groupBoxAnswer);
    vLayout->addWidget(groupBoxNumPoints);
    vLayout->addWidget(mSaveButton);
    setLayout(vLayout);

    connect(mSaveButton, SIGNAL(clicked()), this, SLOT(saveClicked()));
}

CategoryWidget::~CategoryWidget()
{
    delete mSaveButton;
}

void CategoryWidget::saveClicked()
{
    close();
}
