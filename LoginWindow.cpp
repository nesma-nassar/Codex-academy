#include "LoginWindow.h"
#include "AuthController.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFrame>
#include <QLabel>
#include <QPixmap>
#include <QGraphicsDropShadowEffect>

LoginWindow::LoginWindow(QWidget* parent) : QWidget(parent) {
    setWindowTitle("Codex Academy");
    setFixedSize(460, 640);
    setupUI();
    setupConnections();
}

void LoginWindow::setupUI() {
    setStyleSheet("background:#0f1117;");
    auto* root = new QVBoxLayout(this);
    root->setContentsMargins(0,0,0,0);
    root->addStretch(1);

    auto* frame = new QFrame();
    frame->setStyleSheet(
        "QFrame{background:#1a1f2e;border:2px solid #2a3040;border-radius:20px;}");
    auto* lay = new QVBoxLayout(frame);
    lay->setContentsMargins(44,36,44,40);
    lay->setSpacing(14);

    // ── Logo image ─────────────────────────────────────────────
    auto* logoRow = new QHBoxLayout();
    logoRow->addStretch();

    auto* logoLabel = new QLabel();
    QPixmap pix(":/logo.png");
    if (!pix.isNull()) {
        logoLabel->setPixmap(pix.scaled(80, 80, Qt::KeepAspectRatio,
                                        Qt::SmoothTransformation));
    } else {
        // Fallback text logo
        logoLabel->setText("✦");
        logoLabel->setStyleSheet(
            "color:#1cb0f6;font-size:48px;font-weight:900;");
    }
    logoLabel->setAlignment(Qt::AlignCenter);
    logoRow->addWidget(logoLabel);
    logoRow->addStretch();
    lay->addLayout(logoRow);

    // App name
    auto* appName = new QLabel("CODEX ACADEMY");
    appName->setAlignment(Qt::AlignCenter);
    appName->setStyleSheet(
        "color:#e8eaf0;font-size:20px;font-weight:900;"
        "letter-spacing:3px;margin-top:-4px;");
    lay->addWidget(appName);

    auto* tagline = new QLabel("Learn to code. Level up your skills.");
    tagline->setAlignment(Qt::AlignCenter);
    tagline->setStyleSheet(
        "color:#6b7280;font-size:12px;margin-bottom:8px;");
    lay->addWidget(tagline);

    // Divider
    auto* div = new QFrame();
    div->setFrameShape(QFrame::HLine);
    div->setStyleSheet("color:#2a3040;");
    lay->addWidget(div);

    // Fields
    auto* uLabel = new QLabel("USERNAME");
    uLabel->setStyleSheet(
        "color:#6b7280;font-size:11px;font-weight:800;letter-spacing:1.5px;");
    lay->addWidget(uLabel);
    m_usernameEdit = new QLineEdit();
    m_usernameEdit->setPlaceholderText("Enter your username");
    m_usernameEdit->setFixedHeight(50);
    lay->addWidget(m_usernameEdit);

    auto* pLabel = new QLabel("PASSWORD");
    pLabel->setStyleSheet(
        "color:#6b7280;font-size:11px;font-weight:800;letter-spacing:1.5px;");
    lay->addWidget(pLabel);
    m_passwordEdit = new QLineEdit();
    m_passwordEdit->setPlaceholderText("Enter your password");
    m_passwordEdit->setEchoMode(QLineEdit::Password);
    m_passwordEdit->setFixedHeight(50);
    lay->addWidget(m_passwordEdit);

    m_errorLabel = new QLabel("");
    m_errorLabel->setObjectName("errorLabel");
    m_errorLabel->setAlignment(Qt::AlignCenter);
    m_errorLabel->setWordWrap(true);
    m_errorLabel->setFixedHeight(20);
    lay->addWidget(m_errorLabel);

    m_loginBtn = new QPushButton("LOG IN");
    m_loginBtn->setFixedHeight(54);
    m_loginBtn->setStyleSheet(R"(
        QPushButton{background:#1cb0f6;color:white;border:none;
            border-bottom:4px solid #1899d6;border-radius:12px;
            font-size:15px;font-weight:800;letter-spacing:1px;}
        QPushButton:hover{background:#3dbff8;}
        QPushButton:pressed{background:#1899d6;border-bottom:2px solid #1899d6;margin-top:2px;}
        QPushButton:disabled{background:#2a3040;border-bottom:4px solid #1e2433;color:#3d4455;}
    )");
    lay->addWidget(m_loginBtn);

    // OR divider
    auto* divRow = new QHBoxLayout();
    auto* l1 = new QFrame(); l1->setFrameShape(QFrame::HLine);
    l1->setStyleSheet("color:#2a3040;");
    auto* orLbl = new QLabel("OR");
    orLbl->setStyleSheet(
        "color:#6b7280;padding:0 10px;font-size:12px;font-weight:700;");
    auto* l2 = new QFrame(); l2->setFrameShape(QFrame::HLine);
    l2->setStyleSheet("color:#2a3040;");
    divRow->addWidget(l1); divRow->addWidget(orLbl); divRow->addWidget(l2);
    lay->addLayout(divRow);

    m_signUpBtn = new QPushButton("CREATE ACCOUNT");
    m_signUpBtn->setFixedHeight(50);
    m_signUpBtn->setStyleSheet(R"(
        QPushButton{background:#1a1f2e;color:#1cb0f6;
            border:2px solid #1cb0f6;border-bottom:4px solid #1899d6;
            border-radius:12px;font-size:14px;font-weight:800;letter-spacing:1px;}
        QPushButton:hover{background:rgba(28,176,246,0.1);}
        QPushButton:pressed{border-bottom:2px solid #1899d6;margin-top:2px;}
    )");
    lay->addWidget(m_signUpBtn);

    root->addWidget(frame);
    root->addStretch(1);

    auto* shadow = new QGraphicsDropShadowEffect();
    shadow->setBlurRadius(50);
    shadow->setColor(QColor(28,176,246,30));
    shadow->setOffset(0,10);
    frame->setGraphicsEffect(shadow);
}

void LoginWindow::setupConnections() {
    connect(m_loginBtn,  &QPushButton::clicked,
            this, &LoginWindow::onLoginClicked);
    connect(m_signUpBtn, &QPushButton::clicked,
            this, &LoginWindow::goToSignUp);
    connect(m_passwordEdit, &QLineEdit::returnPressed,
            this, &LoginWindow::onLoginClicked);
}

void LoginWindow::onLoginClicked() {
    m_errorLabel->clear();
    m_loginBtn->setEnabled(false);
    m_loginBtn->setText("LOGGING IN...");
    QString err;
    if (AuthController::instance().loginUser(
            m_usernameEdit->text().trimmed(),
            m_passwordEdit->text(), err)) {
        emit loginSuccess();
    } else {
        m_errorLabel->setText(err);
        m_loginBtn->setEnabled(true);
        m_loginBtn->setText("LOG IN");
    }
}
