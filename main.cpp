#include <QApplication>
#include <QMainWindow>
#include <QStackedWidget>
#include <QIcon>
#include <QPixmap>
#include "StyleSheet.h"
#include "LoginWindow.h"
#include "SignUpWindow.h"
#include "DashboardWindow.h"
#include "LessonListWindow.h"
#include "LessonViewWindow.h"
#include "QuizWindow.h"
#include "ProfileWindow.h"
#include "AuthController.h"


int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    app.setApplicationName("Codex Academy");
    app.setOrganizationName("Codex");
    app.setStyleSheet(Style::appStyle());

    // Set app icon from logo
    QPixmap iconPix(":/logo.png");
    if (!iconPix.isNull())
        app.setWindowIcon(QIcon(iconPix));



    QMainWindow mainWin;
    mainWin.setWindowTitle("Codex Academy");
    mainWin.resize(1024, 768);
    if (!iconPix.isNull())
        mainWin.setWindowIcon(QIcon(iconPix));

    auto* stack = new QStackedWidget();
    mainWin.setCentralWidget(stack);

    auto* loginW      = new LoginWindow();
    auto* signupW     = new SignUpWindow();
    auto* dashW       = new DashboardWindow();
    auto* lessonListW = new LessonListWindow();
    auto* lessonViewW = new LessonViewWindow();
    auto* quizW       = new QuizWindow();
    auto* profileW    = new ProfileWindow();


    stack->addWidget(loginW);      // 0
    stack->addWidget(signupW);     // 1
    stack->addWidget(dashW);       // 2
    stack->addWidget(lessonListW); // 3
    stack->addWidget(lessonViewW); // 4
    stack->addWidget(quizW);       // 5
    stack->addWidget(profileW);    // 6


    // ════════════════════════════════════════════════════════
    //  AI CONNECTIONS — single source of truth
    // ════════════════════════════════════════════════════════










    // ════════════════════════════════════════════════════════
    //  NAVIGATION
    // ════════════════════════════════════════════════════════
    QObject::connect(loginW,  &LoginWindow::goToSignUp,
                     [&]{ stack->setCurrentIndex(1); });
    QObject::connect(signupW, &SignUpWindow::goToLogin,
                     [&]{ stack->setCurrentIndex(0); });
    QObject::connect(loginW,  &LoginWindow::loginSuccess, [&]{
        dashW->refreshUser(); stack->setCurrentIndex(2);
    });
    QObject::connect(signupW, &SignUpWindow::signUpSuccess,
                     [&]{ stack->setCurrentIndex(0); });

    QObject::connect(dashW, &DashboardWindow::openLearnSection, [&]{
        lessonListW->refreshLessons(); stack->setCurrentIndex(3);
    });
    QObject::connect(dashW, &DashboardWindow::openProfile, [&]{
        profileW->refreshProfile(); stack->setCurrentIndex(6);
    });
    QObject::connect(dashW, &DashboardWindow::logoutRequested, [&]{
        AuthController::instance().logout(); stack->setCurrentIndex(0);
    });
    QObject::connect(dashW, &DashboardWindow::openChatBot,
                     [&]{ stack->setCurrentIndex(8); });
    QObject::connect(dashW, &DashboardWindow::openCodeHelper,
                     [&]{ stack->setCurrentIndex(7); });

    QObject::connect(lessonListW, &LessonListWindow::lessonSelected,
                     [&](const QString& id){
        lessonViewW->loadLesson(id); stack->setCurrentIndex(4);
    });
    QObject::connect(lessonListW, &LessonListWindow::goBack,
                     [&]{ stack->setCurrentIndex(2); });

    QObject::connect(lessonViewW, &LessonViewWindow::lessonDone,
                     [&](const QString& id){
        quizW->startQuiz(id); stack->setCurrentIndex(5);
    });
    QObject::connect(lessonViewW, &LessonViewWindow::goBack,
                     [&]{ stack->setCurrentIndex(3); });

    QObject::connect(quizW, &QuizWindow::quizCompleted,
                     [&](const QString&, int){
        lessonListW->refreshLessons();
        dashW->refreshUser();
        stack->setCurrentIndex(3);
    });
    QObject::connect(quizW, &QuizWindow::goBack,
                     [&]{ stack->setCurrentIndex(4); });
    QObject::connect(quizW, &QuizWindow::openChatBot,
                     [&]{ stack->setCurrentIndex(8); });
    QObject::connect(quizW, &QuizWindow::openCodeHelper,
                     [&]{ stack->setCurrentIndex(7); });

    QObject::connect(profileW, &ProfileWindow::goBack,
                     [&]{ stack->setCurrentIndex(2); });
    QObject::connect(profileW, &ProfileWindow::openSmartPath,
                     [&]{ stack->setCurrentIndex(6); });




    stack->setCurrentIndex(0);
    mainWin.show();
    return app.exec();
}
