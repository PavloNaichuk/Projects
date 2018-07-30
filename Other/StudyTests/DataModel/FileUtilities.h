#pragma once

#include "Category.h"
#include <memory>

std::unique_ptr<Category> LoadComponentFromFile(const QString& filePath);
void SaveComponentToFile(const QString& filePath, const Category& category);
