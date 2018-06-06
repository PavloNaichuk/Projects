#include "Task.h"

const QString& Task::question() const
{
    return mQuestion;
}

void Task::setQuestion(QString question)
{
    mQuestion = std::move(question);
}

const QString& Task::answer() const
{
    return mAnswer;
}

void Task::setAnswer(QString answer)
{
    mAnswer = std::move(answer);
}

int Task::numPoints() const
{
    return mNumPoints;
}

void Task::setNumPoints(unsigned numPoints)
{
    mNumPoints = numPoints;
}
