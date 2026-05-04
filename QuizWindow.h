#pragma once
#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QTextEdit>
#include <QStackedWidget>
#include <QVector>
#include <QString>
#include <QStringList>
#include "Question.h"

class QuizWindow : public QWidget {
    Q_OBJECT
public:
    explicit QuizWindow(QWidget* parent = nullptr);
    void startQuiz(const QString& lessonId);

signals:
    void quizCompleted(const QString& lessonId, int xpEarned);
    void goBack();
    void openCodeHelper();
    void openChatBot();

private slots:
    void submitAnswer();
    void nextQuestion();

private:
    void setupUI();
    void showCurrentQuestion();
    void showResultPage();

    // Quiz page widgets
    QLabel*         m_progressLabel = nullptr;
    QLabel*         m_topicLabel    = nullptr;
    QLabel*         m_questionLabel = nullptr;
    QLabel*         m_xpValLabel    = nullptr;
    QFrame*         m_robotPanel    = nullptr;
    QLabel*         m_robotFace     = nullptr;
    QLabel*         m_robotName     = nullptr;
    QLabel*         m_robotMsg      = nullptr;
    QWidget*        m_mcqWidget     = nullptr;
    QWidget*        m_writingWidget = nullptr;
    QTextEdit*      m_answerEdit    = nullptr;
    QPushButton*    m_submitBtn     = nullptr;
    QPushButton*    m_nextBtn       = nullptr;
    QPushButton*    m_backBtn       = nullptr;
    QStackedWidget* m_inputStack    = nullptr;
    QStackedWidget* m_pageStack     = nullptr;

    // Result page widgets
    QLabel*      m_resultRobot  = nullptr;
    QLabel*      m_resultTitle  = nullptr;
    QLabel*      m_resultScore  = nullptr;
    QLabel*      m_resultXP     = nullptr;
    QLabel*      m_resultAdvice = nullptr;
    QPushButton* m_retryBtn     = nullptr;
    QPushButton* m_continueBtn  = nullptr;

    QVector<QPushButton*> m_choiceBtns;
    QVector<Question*>    m_questions;
    QStringList           m_wrongTopics;

    int     m_currentIdx   = 0;
    int     m_xpEarned     = 0;
    int     m_correctCount = 0;
    QString m_selectedMCQ;
    QString m_lessonId;
};
