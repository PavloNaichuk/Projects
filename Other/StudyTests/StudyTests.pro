TEMPLATE = subdirs
CONFIG += ordered

SUBDIRS += \
    Application \
    DataModel \
    UnitTests

Application.depends = DataModel
UnitTests.depends = DataModel
