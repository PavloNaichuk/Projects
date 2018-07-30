#include "FileUtilities.h"
#include "Category.h"
#include "Archiver.h"
#include "JSONVisitor.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>

std::unique_ptr<Category> LoadComponentFromFile(const QString& filePath)
{
    QFile file(filePath);
    if (file.open(QIODevice::ReadOnly))
    {
        QByteArray fileData = file.readAll();
        QJsonDocument loadDoc;
        loadDoc.fromBinaryData(fileData);

        auto jsonData = Archiver::deserialise(loadDoc.object());
        return std::unique_ptr<Category>(dynamic_cast<Category*>(jsonData.release()));
    }
    return std::make_unique<Category>();
}

void SaveComponentToFile(const QString& filePath, const Category& category)
{
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly))
    {
        qWarning("Couldn't open save file.");
        return;
    }
    auto fileData = Archiver::serialise(&category);
    QJsonDocument saveDoc(*fileData);
    file.write(saveDoc.toBinaryData());
}
