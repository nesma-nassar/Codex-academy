#pragma once
#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>

class SignUpWindow : public QWidget {
    Q_OBJECT
public:
    explicit SignUpWindow(QWidget* parent = nullptr);
signals:
    void signUpSuccess();
    void goToLogin();
private slots:
    void onRegisterClicked();
private:
    void setupUI();
    QLineEdit   *m_usernameEdit, *m_emailEdit, *m_passwordEdit, *m_confirmEdit;
    QPushButton *m_registerBtn,  *m_loginBtn;
    QLabel      *m_errorLabel;
};
