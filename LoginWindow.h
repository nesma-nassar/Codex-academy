#pragma once
#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>

class LoginWindow : public QWidget {
    Q_OBJECT
public:
    explicit LoginWindow(QWidget* parent = nullptr);

signals:
    void loginSuccess();
    void goToSignUp();

private slots:
    void onLoginClicked();

private:
    void setupUI();
    void setupConnections();

    QLineEdit*   m_usernameEdit;
    QLineEdit*   m_passwordEdit;
    QPushButton* m_loginBtn;
    QPushButton* m_signUpBtn;
    QLabel*      m_errorLabel;
    QLabel*      m_logoLabel;
};
