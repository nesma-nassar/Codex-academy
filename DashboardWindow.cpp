#include "DashboardWindow.h"
#include <QPixmap>
#include "AuthController.h"
#include "Lesson.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFrame>
#include <QScrollArea>
#include <QGridLayout>
#include <QPushButton>
#include <QLabel>
#include <QProgressBar>
#include <QStackedWidget>

DashboardWindow::DashboardWindow(QWidget* parent) : QWidget(parent) {
    setWindowTitle("Codex Academy");
    setMinimumSize(960, 700);
    setupUI();
    refreshUser();
}

static QWidget* makeLogoWidget() {
    auto* row = new QWidget();
    auto* rl = new QHBoxLayout(row);
    rl->setContentsMargins(0,0,0,0); rl->setSpacing(10);

    auto* logoLbl = new QLabel();
    QPixmap pix(":/logo.png");
    if (!pix.isNull())
        logoLbl->setPixmap(pix.scaled(36,36,Qt::KeepAspectRatio,Qt::SmoothTransformation));
    else { logoLbl->setText("✦"); logoLbl->setStyleSheet("color:#1cb0f6;font-size:22px;"); }

    auto* nm = new QLabel("CODEX ACADEMY");
    nm->setStyleSheet("font-size:15px;font-weight:900;color:#e8eaf0;letter-spacing:2px;");

    rl->addWidget(logoLbl); rl->addWidget(nm);
    return row;
}

void DashboardWindow::setupUI() {
    auto* root = new QVBoxLayout(this);
    root->setContentsMargins(0,0,0,0); root->setSpacing(0);

    // ── NavBar ─────────────────────────────────────────────────
    auto* nav = new QFrame();
    nav->setFixedHeight(62);
    nav->setStyleSheet("QFrame{background:#1a1f2e;border-bottom:2px solid #2a3040;}");
    auto* nl = new QHBoxLayout(nav); nl->setContentsMargins(24,0,24,0);
    nl->addWidget(makeLogoWidget()); nl->addStretch();

    auto* streakBadge = new QLabel("🔥 1");
    streakBadge->setStyleSheet(
        "color:#ff9600;font-size:15px;font-weight:800;"
        "background:rgba(255,150,0,0.12);border-radius:8px;padding:4px 12px;");
    nl->addWidget(streakBadge); nl->addSpacing(8);

    m_learnBtn = new QPushButton("LEARN");
    m_learnBtn->setFixedHeight(36);
    m_learnBtn->setStyleSheet(
        "QPushButton{background:#1cb0f6;color:white;border:none;"
        "border-bottom:3px solid #1899d6;border-radius:8px;"
        "padding:0 18px;font-size:12px;font-weight:800;letter-spacing:1px;}"
        "QPushButton:hover{background:#3dbff8;}");
    nl->addWidget(m_learnBtn);

    m_profileBtn = new QPushButton("PROFILE");
    m_profileBtn->setFixedHeight(36);
    m_profileBtn->setStyleSheet(
        "QPushButton{background:#1a1f2e;color:#e8eaf0;"
        "border:2px solid #2a3040;border-radius:8px;"
        "padding:0 18px;font-size:12px;font-weight:800;letter-spacing:1px;}"
        "QPushButton:hover{border-color:#1cb0f6;color:#1cb0f6;}");
    nl->addWidget(m_profileBtn);

    m_logoutBtn = new QPushButton("⏻");
    m_logoutBtn->setFixedSize(36,36);
    m_logoutBtn->setStyleSheet(
        "QPushButton{background:transparent;color:#6b7280;"
        "border:2px solid #2a3040;border-radius:8px;font-size:16px;}"
        "QPushButton:hover{color:#ff4b4b;border-color:#ff4b4b;}");
    nl->addWidget(m_logoutBtn);
    root->addWidget(nav);

    // ── Scroll ─────────────────────────────────────────────────
    auto* scroll = new QScrollArea();
    scroll->setWidgetResizable(true);
    scroll->setStyleSheet("QScrollArea{border:none;background:#0f1117;}");
    auto* content = new QWidget();
    content->setStyleSheet("background:#0f1117;");
    auto* cLay = new QVBoxLayout(content);
    cLay->setContentsMargins(40,36,40,40); cLay->setSpacing(0);

    // Welcome
    m_welcomeLabel = new QLabel("Welcome back!");
    m_welcomeLabel->setStyleSheet(
        "font-size:30px;font-weight:900;color:#e8eaf0;margin-bottom:4px;");
    cLay->addWidget(m_welcomeLabel);

    auto* xpRow = new QHBoxLayout();
    m_levelLabel = new QLabel("⚡ Level 1");
    m_levelLabel->setStyleSheet("color:#ffd900;font-size:14px;font-weight:800;");
    m_xpLabel = new QLabel("0 XP");
    m_xpLabel->setStyleSheet("color:#1cb0f6;font-size:14px;font-weight:800;");
    xpRow->addWidget(m_levelLabel); xpRow->addSpacing(16);
    xpRow->addWidget(m_xpLabel); xpRow->addStretch();
    cLay->addLayout(xpRow);
    cLay->addSpacing(24);

    // ── AI Features Banner ─────────────────────────────────────
    auto* aiBanner = new QFrame();
    aiBanner->setStyleSheet(
        "QFrame{background:qlineargradient(x1:0,y1:0,x2:1,y2:0,"
        "stop:0 #0f2030,stop:1 #1a1f2e);"
        "border:2px solid #1cb0f6;border-radius:16px;}");
    auto* abLay = new QVBoxLayout(aiBanner);
    abLay->setContentsMargins(24,18,24,18); abLay->setSpacing(14);

    auto* abTitle = new QLabel("🤖  AI-Powered Learning Features");
    abTitle->setStyleSheet("font-size:17px;font-weight:900;color:#e8eaf0;");
    abLay->addWidget(abTitle);

    auto* abDesc = new QLabel(
        "Get instant help from our AI tutor, check your code, "
        "and get a personalised learning path.");
    abDesc->setStyleSheet("color:#9ca3af;font-size:13px;");
    abDesc->setWordWrap(true);
    abLay->addWidget(abDesc);

    auto* btnRow = new QHBoxLayout(); btnRow->setSpacing(12);

    // Button 1: AI Chatbot
    m_chatBotBtn = new QPushButton("💬  Ask AI Assistant");
    m_chatBotBtn->setFixedHeight(44);
    m_chatBotBtn->setStyleSheet(R"(
        QPushButton{background:#1cb0f6;color:white;border:none;
            border-bottom:4px solid #1899d6;border-radius:10px;
            font-size:13px;font-weight:800;letter-spacing:0.5px;}
        QPushButton:hover{background:#3dbff8;}
        QPushButton:pressed{border-bottom:2px solid #1899d6;margin-top:2px;}
    )");
    connect(m_chatBotBtn, &QPushButton::clicked,
            this, &DashboardWindow::openChatBot);
    btnRow->addWidget(m_chatBotBtn);

    // Button 2: Code Helper
    m_codeHelperBtn = new QPushButton("🔍  AI Code Helper");
    m_codeHelperBtn->setFixedHeight(44);
    m_codeHelperBtn->setStyleSheet(R"(
        QPushButton{background:#1a1f2e;color:#1cb0f6;
            border:2px solid #1cb0f6;border-bottom:4px solid #1899d6;
            border-radius:10px;font-size:13px;font-weight:800;letter-spacing:0.5px;}
        QPushButton:hover{background:rgba(28,176,246,0.1);}
        QPushButton:pressed{border-bottom:2px solid #1899d6;margin-top:2px;}
    )");
    connect(m_codeHelperBtn, &QPushButton::clicked,
            this, &DashboardWindow::openCodeHelper);
    btnRow->addWidget(m_codeHelperBtn);

    // Button 3: Smart Path
    m_smartPathBtn = new QPushButton("🗺️  Smart Learning Path");
    m_smartPathBtn->setFixedHeight(44);
    m_smartPathBtn->setStyleSheet(R"(
        QPushButton{background:#1a1f2e;color:#ffd900;
            border:2px solid #ffd900;border-bottom:4px solid #cc9900;
            border-radius:10px;font-size:13px;font-weight:800;letter-spacing:0.5px;}
        QPushButton:hover{background:rgba(255,217,0,0.08);}
        QPushButton:pressed{border-bottom:2px solid #cc9900;margin-top:2px;}
    )");
    connect(m_smartPathBtn, &QPushButton::clicked,
            this, &DashboardWindow::openProfile);
    btnRow->addWidget(m_smartPathBtn);

    abLay->addLayout(btnRow);
    cLay->addWidget(aiBanner);
    cLay->addSpacing(28);

    // Choose Your Quest
    auto* qt = new QLabel("Choose Your Quest");
    qt->setStyleSheet("font-size:22px;font-weight:900;color:#e8eaf0;margin-bottom:4px;");
    cLay->addWidget(qt);
    auto* qs = new QLabel("Select a language to begin your learning journey");
    qs->setStyleSheet("color:#6b7280;font-size:14px;margin-bottom:20px;");
    cLay->addWidget(qs);

    // Language grid
    auto* grid = new QGridLayout(); grid->setSpacing(14);
    QStringList codes  = {"C++","Python","JavaScript","Java","C#","Rust"};
    QStringList icons  = {"</>","Py","JS","Jv","C#","Rs"};
    QStringList descs  = {
        "Systems programming & performance",
        "Versatile & beginner-friendly",
        "Web development essential",
        "Enterprise & Android development",
        "Games & .NET applications",
        "Safe systems programming"};
    QStringList colors = {"#00d4ff","#ffd43b","#f7df1e","#f89820","#9b59b6","#ce4a06"};
    QList<bool>  active= {true,false,false,false,false,false};

    for (int i = 0; i < codes.size(); i++) {
        bool on = active[i];
        auto* card = new QFrame();
        card->setFixedHeight(120);
        card->setStyleSheet(on
            ? "QFrame{background:#1a1f2e;border:2px solid #1cb0f6;border-radius:16px;}"
              "QFrame:hover{border-color:#5ecfff;background:#1e2438;}"
            : "QFrame{background:#13161e;border:2px solid #1e2433;border-radius:16px;}");
        auto* cl = new QVBoxLayout(card);
        cl->setContentsMargins(16,12,16,12); cl->setSpacing(6);

        auto* nr = new QHBoxLayout();
        auto* ico = new QLabel(icons[i]);
        ico->setFixedSize(34,34); ico->setAlignment(Qt::AlignCenter);
        ico->setStyleSheet(QString(
            "background:%1;border-radius:9px;"
            "font-family:'Consolas','Courier New',monospace;"
            "font-size:11px;font-weight:900;color:white;").arg(colors[i]));
        auto* nm = new QLabel(codes[i]);
        nm->setStyleSheet(on
            ? "font-size:16px;font-weight:900;color:#e8eaf0;"
            : "font-size:16px;font-weight:900;color:#3d4455;");
        nr->addWidget(ico); nr->addSpacing(8); nr->addWidget(nm); nr->addStretch();
        if (on) {
            auto* badge = new QLabel("ACTIVE");
            badge->setStyleSheet(
                "color:#1cb0f6;font-size:10px;font-weight:800;"
                "background:rgba(28,176,246,0.15);border-radius:6px;padding:2px 8px;");
            nr->addWidget(badge);
        }
        cl->addLayout(nr);

        auto* desc = new QLabel(on ? descs[i] : "Coming soon...");
        desc->setStyleSheet(on
            ? "color:#9ca3af;font-size:12px;"
            : "color:#3d4455;font-size:12px;font-style:italic;");
        cl->addWidget(desc);

        if (on) {
            auto* sb = new QPushButton("START QUEST  ->");
            sb->setFixedHeight(30);
            sb->setStyleSheet(
                "QPushButton{background:#1cb0f6;color:white;border:none;"
                "border-bottom:3px solid #1899d6;border-radius:8px;"
                "font-size:11px;font-weight:800;letter-spacing:1px;}"
                "QPushButton:hover{background:#3dbff8;}");
            connect(sb, &QPushButton::clicked,
                    this, &DashboardWindow::openLearnSection);
            cl->addWidget(sb);
        }
        grid->addWidget(card, i/2, i%2);
    }
    cLay->addLayout(grid);
    cLay->addStretch();

    scroll->setWidget(content);
    root->addWidget(scroll);

    connect(m_learnBtn,   &QPushButton::clicked, this, &DashboardWindow::openLearnSection);
    connect(m_profileBtn, &QPushButton::clicked, this, &DashboardWindow::openProfile);
    connect(m_logoutBtn,  &QPushButton::clicked, this, &DashboardWindow::logoutRequested);
}

void DashboardWindow::refreshUser() {
    if (!AuthController::instance().isLoggedIn()) return;
    const User& u = AuthController::instance().currentUser();
    m_welcomeLabel->setText(QString("Welcome back, %1! 👋").arg(u.getUsername()));
    m_levelLabel->setText(QString("⚡ Level %1").arg(u.getLevel()));
    m_xpLabel->setText(QString("🏆 %1 XP").arg(u.getXP()));
}
