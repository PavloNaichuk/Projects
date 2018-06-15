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

bool Task::equals(const Component* comp) const
{
    const Task* task1 = this;
    const Task* task2 = dynamic_cast<const Task*>(comp);
    if (task2 != nullptr)
    {
        return ((task1->question() == task2->question()) &&
                (task1->answer() == task2->answer()) &&
                (task1->numPoints() == task2->numPoints()));
    }
    return false;
}
