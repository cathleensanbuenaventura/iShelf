QT += core gui sql

CONFIG += c++17

TARGET = HinLIBS
TEMPLATE = app

# Add the include directory to the project's include path
INCLUDEPATH += $$PWD/include

# Source files
SOURCES += \
    src/main.cpp \
    src/adminwindow.cpp \
    src/book.cpp \
    src/catalogue.cpp \
    src/catalogueitem.cpp \
    src/catalogueitemfactory.cpp \
    src/dbmanager.cpp \
    src/hinlibssystem.cpp \
    src/librarianwindow.cpp \
    src/loginwindow.cpp \
    src/magazine.cpp \
    src/mainwindow.cpp \
    src/movie.cpp \
    src/patronwindow.cpp \
    src/user.cpp \
    src/videogame.cpp

# Header files
HEADERS += \
    include/adminwindow.h \
    include/book.h \
    include/catalogue.h \
    include/catalogueitem.h \
    include/catalogueitemfactory.h \
    include/dbmanager.h \
    include/hinlibssystem.h \
    include/librarianwindow.h \
    include/loginwindow.h \
    include/magazine.h \
    include/mainwindow.h \
    include/movie.h \
    include/patronwindow.h \
    include/user.h \
    include/videogame.h

# UI files
FORMS += \
    mainwindow.ui

# Default rules for deployment
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
