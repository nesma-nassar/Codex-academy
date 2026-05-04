#pragma once
#include <QWidget>
#include <QLabel>
#include <QPushButton>

class DashboardWindow : public QWidget {
    Q_OBJECT
public:
    explicit DashboardWindow(QWidget* parent = nullptr);
    void refreshUser();

signals:
    void logoutRequested();
    void openLearnSection();
    void openProfile();
    void openChatBot();
    void openCodeHelper();

private:
    void setupUI();

    QLabel*      m_welcomeLabel  = nullptr;
    QLabel*      m_xpLabel       = nullptr;
    QLabel*      m_levelLabel    = nullptr;
    QPushButton* m_learnBtn      = nullptr;
    QPushButton* m_profileBtn    = nullptr;
    QPushButton* m_logoutBtn     = nullptr;
    QPushButton* m_chatBotBtn    = nullptr;
    QPushButton* m_codeHelperBtn = nullptr;
    QPushButton* m_smartPathBtn  = nullptr;
};
