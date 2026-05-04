#include "LessonListWindow.h"
#include "AuthController.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFrame>
#include <QListWidgetItem>
#include <QScrollArea>
#include <QLabel>
#include <QPixmap>
#include <QProgressBar>

LessonListWindow::LessonListWindow(QWidget* parent) : QWidget(parent) {
    setWindowTitle("C++ Course — Codex Academy");
    setMinimumSize(720, 620);
    m_lessons = Lesson::defaultCppLessons();
    setupUI();
    refreshLessons();
}

void LessonListWindow::setupUI() {
    auto* root = new QVBoxLayout(this);
    root->setContentsMargins(0,0,0,0);
    root->setSpacing(0);

    // ── Nav ────────────────────────────────────────────────────
    auto* nav = new QFrame();
    nav->setFixedHeight(58);
    nav->setStyleSheet(
        "QFrame{background:#1a1f2e;border-bottom:2px solid #2a3040;}");
    auto* nl = new QHBoxLayout(nav);
    nl->setContentsMargins(20,0,20,0);

    auto* back = new QPushButton("<- Back");
    back->setStyleSheet(
        "QPushButton{background:transparent;color:#6b7280;border:none;"
        "font-size:14px;font-weight:700;}"
        "QPushButton:hover{color:#1cb0f6;}");
    connect(back, &QPushButton::clicked, this, &LessonListWindow::goBack);
    nl->addWidget(back);
    nl->addStretch();

    auto* brand = new QLabel("Codex Academy");
    brand->setStyleSheet("color:#e8eaf0;font-size:15px;font-weight:800;");
    nl->addWidget(brand);
    nl->addStretch();
    nl->addSpacing(70);
    root->addWidget(nav);

    // ── Scroll ─────────────────────────────────────────────────
    auto* scroll = new QScrollArea();
    scroll->setWidgetResizable(true);
    scroll->setStyleSheet("QScrollArea{border:none;background:#0f1117;}");

    auto* content = new QWidget();
    content->setStyleSheet("background:#0f1117;");
    auto* cLay = new QVBoxLayout(content);
    cLay->setContentsMargins(36,28,36,36);
    cLay->setSpacing(14);

    // Header
    auto* hRow = new QHBoxLayout();
    auto* icon = new QLabel();
    icon->setFixedSize(44,44);
    icon->setAlignment(Qt::AlignCenter);
    icon->setStyleSheet(
        "background:#00d4ff;border-radius:12px;"
        "font-family:'Consolas';font-size:14px;font-weight:900;color:#0f1117;");
    icon->setText("</>");

    auto* titLay = new QVBoxLayout();
    auto* title = new QLabel("C++");
    title->setStyleSheet("font-size:24px;font-weight:900;color:#e8eaf0;");
    auto* completed = new QLabel("0 / 5 lessons completed");
    completed->setObjectName("completedLbl");
    completed->setStyleSheet("color:#6b7280;font-size:13px;");
    titLay->addWidget(title);
    titLay->addWidget(completed);

    hRow->addWidget(icon);
    hRow->addSpacing(12);
    hRow->addLayout(titLay);
    hRow->addStretch();
    cLay->addLayout(hRow);

    // Overall progress bar
    auto* overallBar = new QProgressBar();
    overallBar->setObjectName("overallBar");
    overallBar->setRange(0, 5);
    overallBar->setValue(0);
    overallBar->setFixedHeight(10);
    overallBar->setTextVisible(false);
    cLay->addWidget(overallBar);
    cLay->addSpacing(6);

    // Lesson list
    m_list = new QListWidget();
    m_list->setStyleSheet(R"(
        QListWidget { background:#0f1117; border:none; outline:none; }
        QListWidget::item {
            background:#1a1f2e; border:2px solid #2a3040;
            border-radius:14px; padding:16px 18px;
            margin:4px 0; color:#e8eaf0;
        }
        QListWidget::item:hover    { border-color:#1cb0f6; }
        QListWidget::item:selected {
            background:rgba(28,176,246,0.12);
            border-color:#1cb0f6; color:white;
        }
    )");
    m_list->setSpacing(2);
    connect(m_list, &QListWidget::itemDoubleClicked,
            this, &LessonListWindow::onItemDoubleClicked);
    cLay->addWidget(m_list, 1);

    scroll->setWidget(content);
    root->addWidget(scroll);
}

void LessonListWindow::refreshLessons() {
    m_list->clear();
    const User& user = AuthController::instance().currentUser();
    int doneCount = 0;

    for (int i = 0; i < m_lessons.size(); i++) {
        const auto& l = m_lessons[i];
        bool done     = user.hasCompletedLesson(l.getId());
        bool unlocked = l.getPrerequisiteId().isEmpty()
                        || user.hasCompletedLesson(l.getPrerequisiteId());
        if (done) doneCount++;

        QString statusIcon = done ? "✅" : (unlocked ? "▶" : "🔒");
        QString status     = done ? "Completed"
                           : (unlocked
                              ? QString("Ready  •  +%1 XP").arg(l.getXPReward())
                              : "Complete previous lesson first");

        QString display = QString("%1  Lesson %2: %3\n      %4")
            .arg(statusIcon).arg(i+1).arg(l.getTitle()).arg(status);

        auto* item = new QListWidgetItem(display);
        item->setData(Qt::UserRole,   l.getId());
        item->setData(Qt::UserRole+1, unlocked);

        if (done)
            item->setForeground(QColor("#1cb0f6"));
        else if (!unlocked)
            item->setForeground(QColor("#3d4455"));

        m_list->addItem(item);
    }

    // Update header stats
    auto* completedLbl = findChild<QLabel*>("completedLbl");
    if (completedLbl)
        completedLbl->setText(
            QString("%1 / %2 lessons completed").arg(doneCount).arg(m_lessons.size()));

    auto* overallBar = findChild<QProgressBar*>("overallBar");
    if (overallBar) overallBar->setValue(doneCount);
}

void LessonListWindow::onItemDoubleClicked(QListWidgetItem* item) {
    bool unlocked = item->data(Qt::UserRole+1).toBool();
    if (!unlocked) return;
    emit lessonSelected(item->data(Qt::UserRole).toString());
}
