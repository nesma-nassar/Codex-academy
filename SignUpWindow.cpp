#include "SignUpWindow.h"
#include "AuthController.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFrame>
#include <QLabel>
#include <QPixmap>
#include <QGraphicsDropShadowEffect>

SignUpWindow::SignUpWindow(QWidget* parent) : QWidget(parent) {
    setWindowTitle("Codex Academy — Create Account");
    setFixedSize(460, 700);
    setupUI();
}

void SignUpWindow::setupUI() {
    setStyleSheet("background:#0f1117;");
    auto* root = new QVBoxLayout(this);
    root->setContentsMargins(0,0,0,0);
    root->addStretch(1);

    auto* frame = new QFrame();
    frame->setStyleSheet(
        "QFrame{background:#1a1f2e;border:2px solid #2a3040;border-radius:20px;}");
    auto* lay = new QVBoxLayout(frame);
    lay->setContentsMargins(44,32,44,36);
    lay->setSpacing(12);

    // Logo + name row
    auto* logoRow = new QHBoxLayout();
    logoRow->addStretch();
    auto* logoLbl = new QLabel();
    QPixmap pix(":/logo.png");
    if (!pix.isNull())
        logoLbl->setPixmap(pix.scaled(50,50,Qt::KeepAspectRatio,Qt::SmoothTransformation));
    else { logoLbl->setText("✦"); logoLbl->setStyleSheet("color:#1cb0f6;font-size:32px;"); }
    logoLbl->setAlignment(Qt::AlignCenter);
    auto* nameLabel = new QLabel("CODEX ACADEMY");
    nameLabel->setStyleSheet(
        "color:#e8eaf0;font-size:18px;font-weight:900;letter-spacing:3px;margin-left:10px;");
    logoRow->addWidget(logoLbl);
    logoRow->addWidget(nameLabel);
    logoRow->addStretch();
    lay->addLayout(logoRow);

    auto* sub = new QLabel("Create your account to start learning");
    sub->setAlignment(Qt::AlignCenter);
    sub->setStyleSheet("color:#6b7280;font-size:12px;margin-bottom:4px;");
    lay->addWidget(sub);

    auto* div = new QFrame();
    div->setFrameShape(QFrame::HLine);
    div->setStyleSheet("color:#2a3040;");
    lay->addWidget(div);

    auto makeLabel = [](const QString& t) {
        auto* l = new QLabel(t);
        l->setStyleSheet(
            "color:#6b7280;font-size:11px;font-weight:800;letter-spacing:1.5px;");
        return l;
    };

    lay->addWidget(makeLabel("USERNAME"));
    m_usernameEdit = new QLineEdit();
    m_usernameEdit->setPlaceholderText("Choose a username");
    m_usernameEdit->setFixedHeight(48);
    lay->addWidget(m_usernameEdit);

    lay->addWidget(makeLabel("EMAIL"));
    m_emailEdit = new QLineEdit();
    m_emailEdit->setPlaceholderText("your@email.com");
    m_emailEdit->setFixedHeight(48);
    lay->addWidget(m_emailEdit);

    lay->addWidget(makeLabel("PASSWORD"));
    m_passwordEdit = new QLineEdit();
    m_passwordEdit->setPlaceholderText("Min. 6 characters");
    m_passwordEdit->setEchoMode(QLineEdit::Password);
    m_passwordEdit->setFixedHeight(48);
    lay->addWidget(m_passwordEdit);

    lay->addWidget(makeLabel("CONFIRM PASSWORD"));
    m_confirmEdit = new QLineEdit();
    m_confirmEdit->setPlaceholderText("Repeat your password");
    m_confirmEdit->setEchoMode(QLineEdit::Password);
    m_confirmEdit->setFixedHeight(48);
    lay->addWidget(m_confirmEdit);

    m_errorLabel = new QLabel("");
    m_errorLabel->setObjectName("errorLabel");
    m_errorLabel->setAlignment(Qt::AlignCenter);
    m_errorLabel->setWordWrap(true);
    m_errorLabel->setFixedHeight(20);
    lay->addWidget(m_errorLabel);

    m_registerBtn = new QPushButton("CREATE ACCOUNT");
    m_registerBtn->setFixedHeight(52);
    m_registerBtn->setStyleSheet(R"(
        QPushButton{background:#1cb0f6;color:white;border:none;
            border-bottom:4px solid #1899d6;border-radius:12px;
            font-size:14px;font-weight:800;letter-spacing:1px;}
        QPushButton:hover{background:#3dbff8;}
        QPushButton:pressed{background:#1899d6;border-bottom:2px solid #1899d6;margin-top:2px;}
    )");
    lay->addWidget(m_registerBtn);

    m_loginBtn = new QPushButton("Already have an account? LOG IN");
    m_loginBtn->setFixedHeight(46);
    m_loginBtn->setStyleSheet(R"(
        QPushButton{background:#1a1f2e;color:#1cb0f6;
            border:2px solid #2a3040;border-radius:12px;font-size:13px;font-weight:700;}
        QPushButton:hover{border-color:#1cb0f6;}
    )");
    lay->addWidget(m_loginBtn);

    root->addWidget(frame);
    root->addStretch(1);

    auto* shadow = new QGraphicsDropShadowEffect();
    shadow->setBlurRadius(40);
    shadow->setColor(QColor(28,176,246,25));
    shadow->setOffset(0,8);
    frame->setGraphicsEffect(shadow);

    connect(m_registerBtn, &QPushButton::clicked,
            this, &SignUpWindow::onRegisterClicked);
    connect(m_loginBtn, &QPushButton::clicked,
            this, &SignUpWindow::goToLogin);
    connect(m_confirmEdit, &QLineEdit::returnPressed,
            this, &SignUpWindow::onRegisterClicked);
}

void SignUpWindow::onRegisterClicked() {
    m_errorLabel->clear();
    if (m_passwordEdit->text() != m_confirmEdit->text()) {
        m_errorLabel->setText("Passwords do not match.");
        return;
    }
    QString err;
    if (AuthController::instance().registerUser(
            m_usernameEdit->text().trimmed(),
            m_emailEdit->text().trimmed(),
            m_passwordEdit->text(), err)) {
        emit signUpSuccess();
    } else {
        m_errorLabel->setText(err);
    }
}
