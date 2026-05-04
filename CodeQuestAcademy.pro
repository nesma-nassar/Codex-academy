QT += core gui widgets multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

TARGET   = CodeQuestAcademy
TEMPLATE = app

INCLUDEPATH += include

SOURCES += \
    src/main.cpp \
    src/models/User.cpp \
    src/models/Lesson.cpp \
    src/models/Question.cpp \
    src/models/Badge.cpp \
    src/views/LoginWindow.cpp \
    src/views/SignUpWindow.cpp \
    src/views/DashboardWindow.cpp \
    src/views/LessonListWindow.cpp \
    src/views/LessonViewWindow.cpp \
    src/views/QuizWindow.cpp \
    src/views/ProfileWindow.cpp \
    src/controllers/AuthController.cpp \
    src/controllers/LessonController.cpp \
    src/controllers/QuizController.cpp \
    src/controllers/ProfileController.cpp

HEADERS += \
    include/User.h \
    include/Lesson.h \
    include/Question.h \
    include/Badge.h \
    include/LoginWindow.h \
    include/SignUpWindow.h \
    include/DashboardWindow.h \
    include/LessonListWindow.h \
    include/LessonViewWindow.h \
    include/QuizWindow.h \
    include/ProfileWindow.h \
    include/AuthController.h \
    include/LessonController.h \
    include/QuizController.h \
    include/ProfileController.h \
    include/StyleSheet.h

RESOURCES += resources/resources.qrc

qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
