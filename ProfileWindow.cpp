#include "ProfileWindow.h"
#include "AuthController.h"
#include "Badge.h"
#include "Lesson.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFrame>
#include <QScrollArea>
#include <QPushButton>
#include <QLabel>
#include <QPixmap>
#include <QProgressBar>
#include <QLayoutItem>
#include <QMessageBox>

ProfileWindow::ProfileWindow(QWidget* parent) : QWidget(parent) {
    setWindowTitle("Profile — Codex Academy");
    setMinimumSize(800, 720);
    setupUI();

    // AI connections wired in main.cpp
}

void ProfileWindow::clearLayout(QLayout* lay) {
    if (!lay) return;
    while (lay->count() > 0) {
        QLayoutItem* item = lay->takeAt(0);
        if (item->widget()) delete item->widget();
        else if (item->layout()) clearLayout(item->layout());
        delete item;
    }
}

void ProfileWindow::setupUI() {
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

    auto* back = new QPushButton("← Back");
    back->setStyleSheet(
        "QPushButton{background:transparent;color:#6b7280;border:none;"
        "font-size:14px;font-weight:700;}"
        "QPushButton:hover{color:#1cb0f6;}");
    connect(back, &QPushButton::clicked, this, &ProfileWindow::goBack);
    nl->addWidget(back);
    nl->addStretch();

    auto* tLbl = new QLabel("My Profile");
    tLbl->setStyleSheet("font-size:16px;font-weight:900;color:#e8eaf0;");
    nl->addWidget(tLbl);
    nl->addStretch();
    nl->addSpacing(80);
    root->addWidget(nav);

    // ── Scroll ─────────────────────────────────────────────────
    auto* scroll = new QScrollArea();
    scroll->setWidgetResizable(true);
    scroll->setStyleSheet("QScrollArea{border:none;background:#0f1117;}");

    auto* content = new QWidget();
    content->setStyleSheet("background:#0f1117;");
    auto* cLay = new QVBoxLayout(content);
    cLay->setContentsMargins(36,28,36,36);
    cLay->setSpacing(20);

    // ── Profile card ───────────────────────────────────────────
    auto* card = new QFrame();
    card->setStyleSheet(
        "QFrame{background:#1a1f2e;border:2px solid #2a3040;border-radius:18px;}");
    auto* cardLay = new QVBoxLayout(card);
    cardLay->setContentsMargins(28,24,28,24);
    cardLay->setSpacing(14);

    // Avatar + Info
    auto* topRow = new QHBoxLayout();
    auto* avatarLabel = new QLabel("A");
    avatarLabel->setObjectName("avatarLabel");
    avatarLabel->setFixedSize(68,68);
    avatarLabel->setAlignment(Qt::AlignCenter);
    avatarLabel->setStyleSheet(
        "background:qlineargradient(x1:0,y1:0,x2:1,y2:1,"
        "stop:0 #1cb0f6,stop:1 #5ecfff);"
        "border-radius:34px;font-size:30px;font-weight:900;color:white;");
    topRow->addWidget(avatarLabel);
    topRow->addSpacing(16);

    auto* infoLay = new QVBoxLayout();
    m_nameLabel = new QLabel("Username");
    m_nameLabel->setStyleSheet(
        "font-size:22px;font-weight:900;color:#e8eaf0;");
    auto* lvlXpRow = new QHBoxLayout();
    m_levelLabel = new QLabel("⚡ Level 1");
    m_levelLabel->setStyleSheet(
        "color:#ffd900;font-size:13px;font-weight:800;");
    m_xpLabel = new QLabel("🏆 0 XP");
    m_xpLabel->setStyleSheet(
        "color:#1cb0f6;font-size:13px;font-weight:800;");
    lvlXpRow->addWidget(m_levelLabel);
    lvlXpRow->addSpacing(14);
    lvlXpRow->addWidget(m_xpLabel);
    lvlXpRow->addStretch();
    infoLay->addWidget(m_nameLabel);
    infoLay->addLayout(lvlXpRow);
    topRow->addLayout(infoLay);
    topRow->addStretch();
    cardLay->addLayout(topRow);

    // XP Bar
    auto* xpBarLbl = new QLabel("XP Progress to next level");
    xpBarLbl->setStyleSheet("color:#6b7280;font-size:12px;font-weight:700;");
    cardLay->addWidget(xpBarLbl);

    m_xpBar = new QProgressBar();
    m_xpBar->setFixedHeight(14);
    m_xpBar->setTextVisible(false);
    m_xpBar->setRange(0,200);
    m_xpBar->setValue(0);
    cardLay->addWidget(m_xpBar);

    // ── Smart Path AI ──────────────────────────────────────────
    auto* spFrame = new QFrame();
    spFrame->setStyleSheet(
        "QFrame{background:#0f1117;border:2px solid #2a3040;"
        "border-radius:12px;padding:4px;}");
    auto* spLay = new QVBoxLayout(spFrame);
    spLay->setContentsMargins(16,14,16,14);
    spLay->setSpacing(10);

    auto* spTitle = new QLabel("🗺️  Smart Learning Path");
    spTitle->setStyleSheet(
        "font-size:15px;font-weight:900;color:#e8eaf0;");
    spLay->addWidget(spTitle);

    auto* spDesc = new QLabel(
        "Our AI analyses your performance and recommends what to study next.");
    spDesc->setStyleSheet("color:#6b7280;font-size:12px;");
    spDesc->setWordWrap(true);
    spLay->addWidget(spDesc);

    m_smartPathBtn = new QPushButton("🗺️  GET SMART RECOMMENDATION");
    m_smartPathBtn->setFixedHeight(44);
    m_smartPathBtn->setStyleSheet(R"(
        QPushButton{background:#1cb0f6;color:white;border:none;
            border-bottom:4px solid #1899d6;border-radius:12px;
            font-size:12px;font-weight:800;letter-spacing:1px;}
        QPushButton:hover{background:#3dbff8;}
        QPushButton:pressed{border-bottom:2px solid #1899d6;margin-top:2px;}
        QPushButton:disabled{background:#2a3040;border-bottom:4px solid #1e2433;color:#3d4455;}
    )");
    connect(m_smartPathBtn, &QPushButton::clicked, this, [this]{
        m_smartPathBtn->setEnabled(false);
        m_smartPathBtn->setText("⏳  Analysing your performance...");
        if (m_smartPathResult) {
            m_smartPathResult->setText("Getting AI recommendation...");
            m_smartPathResult->show();
        }

    });
    spLay->addWidget(m_smartPathBtn);

    m_smartPathResult = new QLabel();
    m_smartPathResult->setWordWrap(true);
    m_smartPathResult->setStyleSheet(
        "color:#e8eaf0;font-size:13px;line-height:1.7;"
        "background:#1a1f2e;border:2px solid #1cb0f6;"
        "border-radius:10px;padding:12px 14px;");
    m_smartPathResult->hide();
    spLay->addWidget(m_smartPathResult);

    cardLay->addWidget(spFrame);
    cLay->addWidget(card);

    // ── Badges ─────────────────────────────────────────────────
    auto* badgeTitle = new QLabel("🏅  Badges & Achievements");
    badgeTitle->setStyleSheet(
        "font-size:18px;font-weight:900;color:#e8eaf0;");
    cLay->addWidget(badgeTitle);

    auto* badgesContainer = new QWidget();
    badgesContainer->setStyleSheet("background:transparent;");
    m_badgesLay = new QVBoxLayout(badgesContainer);
    m_badgesLay->setContentsMargins(0,0,0,0);
    cLay->addWidget(badgesContainer);

    // ── Topic Performance ──────────────────────────────────────
    auto* topicTitle = new QLabel("📊  Topic Performance");
    topicTitle->setStyleSheet(
        "font-size:18px;font-weight:900;color:#e8eaf0;margin-top:6px;");
    cLay->addWidget(topicTitle);

    auto* topicsContainer = new QWidget();
    topicsContainer->setStyleSheet("background:transparent;");
    m_topicsLay = new QVBoxLayout(topicsContainer);
    m_topicsLay->setContentsMargins(0,0,0,0);
    cLay->addWidget(topicsContainer);

    // ── Completed Lessons ──────────────────────────────────────
    auto* lessTitle = new QLabel("✅  Completed Lessons");
    lessTitle->setStyleSheet(
        "font-size:18px;font-weight:900;color:#e8eaf0;margin-top:6px;");
    cLay->addWidget(lessTitle);

    auto* lessonsContainer = new QWidget();
    lessonsContainer->setStyleSheet("background:transparent;");
    m_lessonsLay = new QVBoxLayout(lessonsContainer);
    m_lessonsLay->setContentsMargins(0,0,0,0);
    cLay->addWidget(lessonsContainer);

    cLay->addStretch();
    scroll->setWidget(content);
    root->addWidget(scroll);

    refreshProfile();
}

void ProfileWindow::refreshProfile() {
    if (!AuthController::instance().isLoggedIn()) return;
    const User& u = AuthController::instance().currentUser();

    m_nameLabel->setText(u.getUsername());
    m_levelLabel->setText(QString("⚡ Level %1").arg(u.getLevel()));
    m_xpLabel->setText(QString("🏆 %1 XP").arg(u.getXP()));

    int needed = qMax(1, u.getXP() + u.getXPToNextLevel());
    m_xpBar->setRange(0, needed);
    m_xpBar->setValue(u.getXP());

    auto* av = findChild<QLabel*>("avatarLabel");
    if (av) av->setText(u.getUsername().left(1).toUpper());

    // ── Badges ────────────────────────────────────────────────
    clearLayout(m_badgesLay);
    if (u.getBadges().isEmpty()) {
        auto* none = new QLabel("No badges yet — complete lessons to earn some! 🎯");
        none->setStyleSheet(
            "color:#6b7280;font-size:13px;"
            "background:#1a1f2e;border:2px solid #2a3040;"
            "border-radius:10px;padding:12px;");
        m_badgesLay->addWidget(none);
    } else {
        auto* bRow = new QHBoxLayout();
        for (const QString& bid : u.getBadges()) {
            Badge b = Badge::find(bid);
            auto* bc = new QFrame();
            bc->setStyleSheet(QString(
                "QFrame{background:#1a1f2e;border:2px solid %1;"
                "border-radius:12px;}").arg(b.color));
            auto* bl = new QVBoxLayout(bc);
            bl->setContentsMargins(14,12,14,12);
            bl->setSpacing(4);
            auto* be = new QLabel(b.emoji);
            be->setStyleSheet("font-size:22px;");
            be->setAlignment(Qt::AlignCenter);
            auto* bn = new QLabel(b.title);
            bn->setStyleSheet(QString(
                "color:%1;font-size:11px;font-weight:800;").arg(b.color));
            bn->setAlignment(Qt::AlignCenter);
            bl->addWidget(be); bl->addWidget(bn);
            bRow->addWidget(bc);
        }
        bRow->addStretch();
        m_badgesLay->addLayout(bRow);
    }

    // ── Topics ────────────────────────────────────────────────
    clearLayout(m_topicsLay);
    auto stats = u.getTopicStats();
    if (stats.isEmpty()) {
        auto* none = new QLabel("Complete quizzes to see your performance here 📈");
        none->setStyleSheet(
            "color:#6b7280;font-size:13px;"
            "background:#1a1f2e;border:2px solid #2a3040;"
            "border-radius:10px;padding:12px;");
        m_topicsLay->addWidget(none);
    } else {
        auto* topicCard = new QFrame();
        topicCard->setStyleSheet(
            "QFrame{background:#1a1f2e;border:2px solid #2a3040;"
            "border-radius:14px;}");
        auto* tcLay = new QVBoxLayout(topicCard);
        tcLay->setContentsMargins(20,16,20,16);
        tcLay->setSpacing(14);

        for (auto it = stats.cbegin(); it != stats.cend(); ++it) {
            int correct = it.value().first;
            int total   = it.value().second;
            int pct     = (total > 0) ? (correct * 100 / total) : 0;

            auto* rowW = new QWidget();
            auto* rowL = new QVBoxLayout(rowW);
            rowL->setContentsMargins(0,0,0,0);
            rowL->setSpacing(6);

            auto* topRow2 = new QHBoxLayout();
            auto* lbl = new QLabel(it.key());
            lbl->setStyleSheet("color:#e8eaf0;font-size:13px;font-weight:700;");
            auto* pLbl = new QLabel(QString("%1%  (%2/%3 correct)")
                                        .arg(pct).arg(correct).arg(total));
            pLbl->setStyleSheet(
                pct >= 60
                ? "color:#1cb0f6;font-size:12px;font-weight:700;"
                : "color:#ff4b4b;font-size:12px;font-weight:700;");
            topRow2->addWidget(lbl);
            topRow2->addStretch();
            topRow2->addWidget(pLbl);
            rowL->addLayout(topRow2);

            auto* bar = new QProgressBar();
            bar->setRange(0,100);
            bar->setValue(pct);
            bar->setFixedHeight(10);
            bar->setTextVisible(false);
            bar->setStyleSheet(pct >= 60
                ? "QProgressBar{background:#0f1117;border:none;border-radius:999px;}"
                  "QProgressBar::chunk{background:#1cb0f6;border-radius:999px;}"
                : "QProgressBar{background:#0f1117;border:none;border-radius:999px;}"
                  "QProgressBar::chunk{background:#ff4b4b;border-radius:999px;}");
            rowL->addWidget(bar);
            tcLay->addWidget(rowW);
        }
        m_topicsLay->addWidget(topicCard);
    }

    // ── Lessons ───────────────────────────────────────────────
    clearLayout(m_lessonsLay);
    if (u.completedLessons().isEmpty()) {
        auto* none = new QLabel("No lessons completed yet. Start your quest! 🚀");
        none->setStyleSheet(
            "color:#6b7280;font-size:13px;"
            "background:#1a1f2e;border:2px solid #2a3040;"
            "border-radius:10px;padding:12px;");
        m_lessonsLay->addWidget(none);
    } else {
        auto allLessons = Lesson::defaultCppLessons();
        for (const QString& lid : u.completedLessons()) {
            for (const Lesson& l : allLessons) {
                if (l.getId() == lid) {
                    auto* lw = new QLabel(
                        QString("  ✅  %1  •  +%2 XP")
                            .arg(l.getTitle()).arg(l.getXPReward()));
                    lw->setStyleSheet(
                        "color:#1cb0f6;font-size:14px;font-weight:700;"
                        "padding:8px 0;");
                    m_lessonsLay->addWidget(lw);
                }
            }
        }
    }
}

void ProfileWindow::onSmartPathReady(const QString& text) {
    if (m_smartPathResult) {
        m_smartPathResult->setText(text);
        m_smartPathResult->show();
    }
    if (m_smartPathBtn) {
        m_smartPathBtn->setEnabled(true);
        m_smartPathBtn->setText("🗺️  GET SMART RECOMMENDATION");
    }
}

void ProfileWindow::onSmartPathError(const QString& msg) {
    if (m_smartPathResult) {
        m_smartPathResult->setText("❌ Error: " + msg);
        m_smartPathResult->show();
    }
    if (m_smartPathBtn) {
        m_smartPathBtn->setEnabled(true);
        m_smartPathBtn->setText("🗺️  GET SMART RECOMMENDATION");
    }
}
