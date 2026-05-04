#include "QuizWindow.h"
#include "AuthController.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFrame>
#include <QLabel>
#include <QPixmap>
#include <QTimer>
#include <QRandomGenerator>

// ── Random motivational messages ─────────────────────────────
static QStringList correctMessages() {
    return {
        "Excellent! You're on fire! 🔥",
        "Perfect! Keep going, superstar! ⭐",
        "That's right! You're crushing it! 💪",
        "Awesome work! Nothing can stop you! 🚀",
        "Brilliant! Your brain is a machine! 🧠",
        "Spot on! You're becoming a C++ pro! 👾"
    };
}
static QStringList wrongMessages() {
    return {
        "Not quite — but mistakes help us grow! 📚",
        "Almost! Review this topic and try again. 💡",
        "Don't worry! Even pros get this wrong at first. 🤝",
        "Good try! Read the explanation and you'll nail it next time. 📖",
        "Keep going! Every error is a lesson in disguise. 🌱"
    };
}
static QString randomFrom(const QStringList& list) {
    int idx = (int)(QRandomGenerator::global()->generate() % (quint32)list.size());
    return list[idx];
}

QuizWindow::QuizWindow(QWidget* parent) : QWidget(parent) {
    setWindowTitle("Quiz — Codex Academy");
    setMinimumSize(800, 680);
    setupUI();
}

void QuizWindow::setupUI() {
    auto* root = new QVBoxLayout(this);
    root->setContentsMargins(0,0,0,0);
    root->setSpacing(0);

    // ── Nav ────────────────────────────────────────────────────
    auto* nav = new QFrame();
    nav->setFixedHeight(58);
    nav->setStyleSheet("QFrame{background:#1a1f2e;border-bottom:2px solid #2a3040;}");
    auto* nl = new QHBoxLayout(nav);
    nl->setContentsMargins(20,0,20,0);

    m_backBtn = new QPushButton("<- Back");
    m_backBtn->setStyleSheet(
        "QPushButton{background:transparent;color:#6b7280;border:none;"
        "font-size:14px;font-weight:700;}"
        "QPushButton:hover{color:#1cb0f6;}");
    connect(m_backBtn, &QPushButton::clicked, this, &QuizWindow::goBack);
    nl->addWidget(m_backBtn);
    nl->addStretch();

    m_progressLabel = new QLabel("Question 1/4");
    m_progressLabel->setStyleSheet("color:#6b7280;font-size:13px;font-weight:700;");
    nl->addWidget(m_progressLabel);
    nl->addStretch();

    // AI help button
    auto* aiBtn = new QPushButton("💬 Ask AI");
    aiBtn->setFixedHeight(32);
    aiBtn->setStyleSheet(R"(
        QPushButton{background:#1a1f2e;color:#1cb0f6;
            border:2px solid #1cb0f6;border-radius:8px;
            padding:0 14px;font-size:12px;font-weight:800;}
        QPushButton:hover{background:rgba(28,176,246,0.1);}
    )");
    connect(aiBtn, &QPushButton::clicked, this, &QuizWindow::openChatBot);
    nl->addWidget(aiBtn);
    nl->addSpacing(8);

    auto* codeBtn = new QPushButton("🔍 Code Helper");
    codeBtn->setFixedHeight(32);
    codeBtn->setStyleSheet(aiBtn->styleSheet());
    connect(codeBtn, &QPushButton::clicked, this, &QuizWindow::openCodeHelper);
    nl->addWidget(codeBtn);
    root->addWidget(nav);

    // ── Stacked: Quiz page / Result page ─────────────────────
    m_pageStack = new QStackedWidget();

    // ════════════════════════════════════════════════════════════
    //  PAGE 0: Quiz page
    // ════════════════════════════════════════════════════════════
    auto* quizPage = new QWidget();
    quizPage->setStyleSheet("background:#0f1117;");
    auto* qpLay = new QVBoxLayout(quizPage);
    qpLay->setContentsMargins(40,20,40,20);
    qpLay->setSpacing(14);

    // XP row
    auto* xpRow = new QHBoxLayout();
    auto* xpTxt = new QLabel("⚡ XP Earned:");
    xpTxt->setStyleSheet("color:#6b7280;font-size:13px;font-weight:700;");
    m_xpValLabel = new QLabel("0");
    m_xpValLabel->setStyleSheet("color:#ffd900;font-size:14px;font-weight:900;");
    xpRow->addWidget(xpTxt); xpRow->addSpacing(6);
    xpRow->addWidget(m_xpValLabel); xpRow->addStretch();
    qpLay->addLayout(xpRow);

    // Topic badge
    m_topicLabel = new QLabel("Topic");
    m_topicLabel->setStyleSheet(
        "color:#1cb0f6;font-size:11px;font-weight:800;"
        "background:rgba(28,176,246,0.15);border:2px solid rgba(28,176,246,0.3);"
        "border-radius:999px;padding:3px 14px;");
    m_topicLabel->setFixedWidth(220);
    qpLay->addWidget(m_topicLabel);

    // Question text
    m_questionLabel = new QLabel();
    m_questionLabel->setStyleSheet(
        "font-size:18px;font-weight:800;color:#e8eaf0;line-height:1.5;");
    m_questionLabel->setWordWrap(true);
    qpLay->addWidget(m_questionLabel);

    // Input stack (MCQ / Writing)
    m_inputStack = new QStackedWidget();

    m_mcqWidget = new QWidget();
    m_mcqWidget->setStyleSheet("background:transparent;");
    auto* mcqLay = new QVBoxLayout(m_mcqWidget);
    mcqLay->setContentsMargins(0,0,0,0); mcqLay->setSpacing(10);
    for (int i = 0; i < 4; i++) {
        auto* btn = new QPushButton();
        btn->setCheckable(true);
        btn->setFixedHeight(50);
        btn->setStyleSheet(R"(
            QPushButton{background:#1a1f2e;border:2px solid #2a3040;
                border-radius:12px;padding:0 18px;text-align:left;
                color:#e8eaf0;font-size:14px;font-weight:600;}
            QPushButton:hover{border-color:#1cb0f6;}
            QPushButton:checked{background:rgba(28,176,246,0.15);
                border-color:#1cb0f6;color:#1cb0f6;font-weight:800;}
            QPushButton:disabled:checked{background:rgba(28,176,246,0.10);}
        )");
        int cap = i;
        connect(btn, &QPushButton::clicked, this, [this, cap]{
            for (auto* b : m_choiceBtns) b->setChecked(false);
            m_choiceBtns[cap]->setChecked(true);
            m_selectedMCQ = QString::number(cap);
        });
        m_choiceBtns.append(btn);
        mcqLay->addWidget(btn);
    }
    m_inputStack->addWidget(m_mcqWidget); // 0

    m_writingWidget = new QWidget();
    m_writingWidget->setStyleSheet("background:transparent;");
    auto* wLay = new QVBoxLayout(m_writingWidget);
    wLay->setContentsMargins(0,0,0,0); wLay->setSpacing(8);
    auto* wLbl = new QLabel("YOUR ANSWER:");
    wLbl->setStyleSheet("color:#6b7280;font-size:11px;font-weight:800;letter-spacing:1.5px;");
    m_answerEdit = new QTextEdit();
    m_answerEdit->setFixedHeight(90);
    m_answerEdit->setStyleSheet(
        "QTextEdit{background:#1a1f2e;border:2px solid #2a3040;"
        "border-radius:12px;padding:10px;color:#e8eaf0;font-size:14px;}"
        "QTextEdit:focus{border-color:#1cb0f6;}");
    wLay->addWidget(wLbl); wLay->addWidget(m_answerEdit);
    m_inputStack->addWidget(m_writingWidget); // 1

    qpLay->addWidget(m_inputStack, 1);

    // ── Robot mascot feedback panel ───────────────────────────
    m_robotPanel = new QFrame();
    m_robotPanel->setStyleSheet(
        "QFrame{background:#1a1f2e;border:2px solid #2a3040;border-radius:16px;}");
    auto* rpLay = new QHBoxLayout(m_robotPanel);
    rpLay->setContentsMargins(16,14,16,14); rpLay->setSpacing(14);

    m_robotFace = new QLabel("🤖");
    m_robotFace->setFixedSize(54,54);
    m_robotFace->setAlignment(Qt::AlignCenter);
    m_robotFace->setStyleSheet(
        "background:#0f2030;border-radius:27px;font-size:28px;"
        "border:2px solid #1cb0f6;");
    rpLay->addWidget(m_robotFace);

    auto* msgLay = new QVBoxLayout();
    m_robotName = new QLabel("CodeX Bot");
    m_robotName->setStyleSheet(
        "color:#1cb0f6;font-size:12px;font-weight:800;letter-spacing:1px;");
    m_robotMsg = new QLabel("Good luck on this question! I'm here if you need help 😊");
    m_robotMsg->setStyleSheet("color:#e8eaf0;font-size:14px;font-weight:600;");
    m_robotMsg->setWordWrap(true);
    msgLay->addWidget(m_robotName);
    msgLay->addWidget(m_robotMsg);
    rpLay->addLayout(msgLay, 1);
    qpLay->addWidget(m_robotPanel);

    // Buttons row
    auto* btnRow = new QHBoxLayout();
    m_submitBtn = new QPushButton("CHECK ANSWER");
    m_submitBtn->setFixedHeight(52);
    m_submitBtn->setStyleSheet(R"(
        QPushButton{background:#1cb0f6;color:white;border:none;
            border-bottom:4px solid #1899d6;border-radius:12px;
            font-size:14px;font-weight:800;letter-spacing:1px;}
        QPushButton:hover{background:#3dbff8;}
        QPushButton:pressed{background:#1899d6;border-bottom:2px solid #1899d6;margin-top:2px;}
        QPushButton:disabled{background:#2a3040;border-bottom:4px solid #1e2433;color:#3d4455;}
    )");
    connect(m_submitBtn, &QPushButton::clicked, this, &QuizWindow::submitAnswer);

    m_nextBtn = new QPushButton("CONTINUE  ->");
    m_nextBtn->setFixedHeight(52);
    m_nextBtn->hide();
    m_nextBtn->setStyleSheet(R"(
        QPushButton{background:#1a1f2e;color:#1cb0f6;
            border:2px solid #1cb0f6;border-bottom:4px solid #1899d6;
            border-radius:12px;font-size:14px;font-weight:800;letter-spacing:1px;}
        QPushButton:hover{background:rgba(28,176,246,0.1);}
    )");
    connect(m_nextBtn, &QPushButton::clicked, this, &QuizWindow::nextQuestion);
    btnRow->addWidget(m_submitBtn,2); btnRow->addSpacing(10); btnRow->addWidget(m_nextBtn,1);
    qpLay->addLayout(btnRow);

    m_pageStack->addWidget(quizPage); // page 0

    // ════════════════════════════════════════════════════════════
    //  PAGE 1: Result page
    // ════════════════════════════════════════════════════════════
    auto* resultPage = new QWidget();
    resultPage->setStyleSheet("background:#0f1117;");
    auto* rpageLay = new QVBoxLayout(resultPage);
    rpageLay->setContentsMargins(60,40,60,40);
    rpageLay->setSpacing(20);
    rpageLay->addStretch();

    // Big robot face
    m_resultRobot = new QLabel("🤖");
    m_resultRobot->setAlignment(Qt::AlignCenter);
    m_resultRobot->setStyleSheet("font-size:72px;");
    rpageLay->addWidget(m_resultRobot);

    m_resultTitle = new QLabel("Quiz Complete!");
    m_resultTitle->setAlignment(Qt::AlignCenter);
    m_resultTitle->setStyleSheet(
        "font-size:30px;font-weight:900;color:#e8eaf0;");
    rpageLay->addWidget(m_resultTitle);

    m_resultScore = new QLabel();
    m_resultScore->setAlignment(Qt::AlignCenter);
    m_resultScore->setStyleSheet(
        "font-size:18px;font-weight:700;color:#9ca3af;");
    rpageLay->addWidget(m_resultScore);

    m_resultXP = new QLabel();
    m_resultXP->setAlignment(Qt::AlignCenter);
    m_resultXP->setStyleSheet(
        "font-size:22px;font-weight:900;color:#ffd900;");
    rpageLay->addWidget(m_resultXP);

    // Weak topics
    m_resultAdvice = new QLabel();
    m_resultAdvice->setAlignment(Qt::AlignCenter);
    m_resultAdvice->setWordWrap(true);
    m_resultAdvice->setStyleSheet(
        "font-size:14px;color:#e8eaf0;padding:16px 20px;"
        "background:#1a1f2e;border-radius:14px;"
        "border:2px solid #2a3040;line-height:1.6;");
    rpageLay->addWidget(m_resultAdvice);

    // Buttons
    auto* resBtnRow = new QHBoxLayout();
    m_retryBtn = new QPushButton("🔁  RETRY QUIZ");
    m_retryBtn->setFixedHeight(50);
    m_retryBtn->setStyleSheet(R"(
        QPushButton{background:#1a1f2e;color:#e8eaf0;
            border:2px solid #2a3040;border-bottom:4px solid #141820;
            border-radius:12px;font-size:13px;font-weight:800;}
        QPushButton:hover{border-color:#1cb0f6;color:#1cb0f6;}
    )");
    connect(m_retryBtn, &QPushButton::clicked, this, [this]{
        m_pageStack->setCurrentIndex(0);
        startQuiz(m_lessonId);
    });

    m_continueBtn = new QPushButton("CONTINUE  ->");
    m_continueBtn->setFixedHeight(50);
    m_continueBtn->setStyleSheet(R"(
        QPushButton{background:#1cb0f6;color:white;border:none;
            border-bottom:4px solid #1899d6;border-radius:12px;
            font-size:13px;font-weight:800;letter-spacing:1px;}
        QPushButton:hover{background:#3dbff8;}
        QPushButton:pressed{background:#1899d6;border-bottom:2px solid #1899d6;margin-top:2px;}
        QPushButton:disabled{background:#2a3040;border-bottom:4px solid #1e2433;color:#3d4455;}
    )");
    connect(m_continueBtn, &QPushButton::clicked, this, [this]{
        emit quizCompleted(m_lessonId, m_xpEarned);
    });

    resBtnRow->addWidget(m_retryBtn);
    resBtnRow->addSpacing(12);
    resBtnRow->addWidget(m_continueBtn);
    rpageLay->addLayout(resBtnRow);
    rpageLay->addStretch();

    m_pageStack->addWidget(resultPage); // page 1
    root->addWidget(m_pageStack);
}

// ── Start quiz ────────────────────────────────────────────────
void QuizWindow::startQuiz(const QString& lessonId) {
    m_lessonId     = lessonId;
    m_currentIdx   = 0;
    m_xpEarned     = 0;
    m_correctCount = 0;
    m_wrongTopics.clear();
    m_selectedMCQ.clear();
    m_xpValLabel->setText("0");
    m_pageStack->setCurrentIndex(0);

    // Shuffle questions each time (simple: use QuestionFactory fresh)
    qDeleteAll(m_questions);
    m_questions = QuestionFactory::questionsForLesson(lessonId);

    m_robotFace->setText("🤖");
    m_robotFace->setStyleSheet(
        "background:#0f2030;border-radius:27px;font-size:28px;"
        "border:2px solid #1cb0f6;");
    m_robotMsg->setText("Good luck on this question! I'm here if you need help 😊");
    m_robotPanel->setStyleSheet(
        "QFrame{background:#1a1f2e;border:2px solid #2a3040;border-radius:16px;}");

    showCurrentQuestion();
}

// ── Show question ─────────────────────────────────────────────
void QuizWindow::showCurrentQuestion() {
    if (m_currentIdx >= m_questions.size()) {
        showResultPage();
        return;
    }
    Question* q = m_questions[m_currentIdx];
    m_progressLabel->setText(
        QString("Question %1 / %2").arg(m_currentIdx+1).arg(m_questions.size()));
    m_topicLabel->setText("  " + q->getTopic() + "  ");
    m_questionLabel->setText(q->getText());

    // Reset robot
    m_robotFace->setText("🤖");
    m_robotFace->setStyleSheet(
        "background:#0f2030;border-radius:27px;font-size:28px;"
        "border:2px solid #1cb0f6;");
    m_robotMsg->setText("Good luck on this question! I'm here if you need help 😊");
    m_robotPanel->setStyleSheet(
        "QFrame{background:#1a1f2e;border:2px solid #2a3040;border-radius:16px;}");

    m_submitBtn->show(); m_submitBtn->setEnabled(true);
    m_nextBtn->hide();
    m_selectedMCQ.clear();

    if (q->getType() == Question::MCQ) {
        m_inputStack->setCurrentIndex(0);
        auto* mcq = static_cast<MCQQuestion*>(q);
        for (int i = 0; i < m_choiceBtns.size(); i++) {
            if (i < mcq->getChoices().size()) {
                m_choiceBtns[i]->setText(
                    QString("  %1.  %2").arg(QChar('A'+i)).arg(mcq->getChoices()[i]));
                m_choiceBtns[i]->setChecked(false);
                m_choiceBtns[i]->setEnabled(true);
                m_choiceBtns[i]->show();
            } else { m_choiceBtns[i]->hide(); }
        }
    } else {
        m_inputStack->setCurrentIndex(1);
        m_answerEdit->clear();
        m_answerEdit->setReadOnly(false);
    }
}

// ── Submit ────────────────────────────────────────────────────
void QuizWindow::submitAnswer() {
    Question* q = m_questions[m_currentIdx];
    QString answer;

    if (q->getType() == Question::MCQ) {
        if (m_selectedMCQ.isEmpty()) return;
        answer = m_selectedMCQ;
        for (auto* b : m_choiceBtns) b->setEnabled(false);
    } else {
        answer = m_answerEdit->toPlainText().trimmed();
        m_answerEdit->setReadOnly(true);
    }

    bool correct = q->checkAnswer(answer);
    if (correct) {
        m_xpEarned += q->getXPReward();
        m_correctCount++;
        AuthController::instance().currentUser().recordAnswer(q->getTopic(), true);

        // Robot celebrates
        m_robotFace->setText("🤩");
        m_robotFace->setStyleSheet(
            "background:#0f2030;border-radius:27px;font-size:28px;"
            "border:2px solid #1cb0f6;");
        m_robotMsg->setText(randomFrom(correctMessages()));
        m_robotPanel->setStyleSheet(
            "QFrame{background:#0f2030;border:2px solid #1cb0f6;border-radius:16px;}");
    } else {
        if (!m_wrongTopics.contains(q->getTopic()))
            m_wrongTopics.append(q->getTopic());
        AuthController::instance().currentUser().recordAnswer(q->getTopic(), false);

        // Robot advises
        m_robotFace->setText("😔");
        m_robotFace->setStyleSheet(
            "background:#2e1a1a;border-radius:27px;font-size:28px;"
            "border:2px solid #ff4b4b;");
        m_robotMsg->setText(
            randomFrom(wrongMessages()) +
            "\n📌 Correct answer: " + q->getCorrectAnswer());
        m_robotPanel->setStyleSheet(
            "QFrame{background:#2e1a1a;border:2px solid #ff4b4b;border-radius:16px;}");
    }

    m_xpValLabel->setText(QString::number(m_xpEarned));
    m_submitBtn->hide();
    m_nextBtn->show();
}

// ── Next question ─────────────────────────────────────────────
void QuizWindow::nextQuestion() {
    m_currentIdx++;
    m_answerEdit->setReadOnly(false);
    showCurrentQuestion();
}

// ── Result page ───────────────────────────────────────────────
void QuizWindow::showResultPage() {
    int total   = m_questions.size();
    int correct = m_correctCount;

    double pct  = total > 0 ? (double)correct / total * 100.0 : 0.0;

    // Save progress
    AuthController::instance().currentUser()
        .markLessonComplete(m_lessonId, m_xpEarned);
    AuthController::instance().saveCurrentUser();

    // ── Passed or failed? ─────────────────────────────────────
    // Failed = more than half wrong → lesson stays unlocked to revisit
    bool passed = (correct >= (total + 1) / 2); // >= 50%

    if (passed) {
        m_resultRobot->setText("🎉");
        m_resultTitle->setText("Quiz Complete! Well Done!");
        m_resultTitle->setStyleSheet(
            "font-size:28px;font-weight:900;color:#1cb0f6;");
        m_resultScore->setText(
            QString("You got %1 / %2 correct  (%3%)")
                .arg(correct).arg(total).arg((int)pct));
        m_resultXP->setText(QString("+ %1 XP Earned! ⚡").arg(m_xpEarned));
        m_continueBtn->setEnabled(true);

        QString advice;
        if (m_wrongTopics.isEmpty()) {
            advice = "🏆 Perfect score! You mastered everything in this lesson!";
        } else {
            advice = "📌 You had some trouble with:\n";
            for (const QString& t : m_wrongTopics)
                advice += "  •  " + t + "\n";
            advice += "\nConsider reviewing these topics before moving on.";
        }
        m_resultAdvice->setText(advice);
        m_resultAdvice->setStyleSheet(
            "font-size:14px;color:#e8eaf0;padding:16px 20px;"
            "background:#0f2030;border-radius:14px;"
            "border:2px solid #1cb0f6;line-height:1.6;");
    } else {
        m_resultRobot->setText("😟");
        m_resultTitle->setText("You Need to Review!");
        m_resultTitle->setStyleSheet(
            "font-size:28px;font-weight:900;color:#ff4b4b;");
        m_resultScore->setText(
            QString("You got %1 / %2 correct (%3%)")
                .arg(correct).arg(total).arg((int)pct));
        m_resultXP->setText("Next lesson is locked until you pass this quiz.");
        m_resultXP->setStyleSheet("font-size:16px;font-weight:700;color:#ff4b4b;");
        m_continueBtn->setEnabled(false); // can't proceed

        QString advice = "📚 You need to re-watch the lesson!\n\n";
        advice += "You had difficulty with:\n";
        for (const QString& t : m_wrongTopics)
            advice += "  •  " + t + "\n";
        advice += "\nGo back, re-read the lesson content,\n"
                  "then retry the quiz. You've got this! 💪";
        m_resultAdvice->setText(advice);
        m_resultAdvice->setStyleSheet(
            "font-size:14px;color:#e8eaf0;padding:16px 20px;"
            "background:#2e1a1a;border-radius:14px;"
            "border:2px solid #ff4b4b;line-height:1.6;");
    }

    m_pageStack->setCurrentIndex(1);
}
