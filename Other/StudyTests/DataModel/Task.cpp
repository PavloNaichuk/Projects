#include "Task.h"

Task::Task()
    : mNumPoints(0)
{}

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

void Task::setNumPoints(int numPoints)
{
    mNumPoints = numPoints;
}

bool Task::equals(const Component* comp) const
{
    const Task* task = dynamic_cast<const Task*>(comp);
    if (task == nullptr)
        return false;

    return ((question() == task->question()) &&
            (answer() == task->answer()) &&
            (numPoints() == task->numPoints()));
}

void Task::visit(Visitor* visitor) const
{
    visitor->visit(this);
}
