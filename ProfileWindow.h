#pragma once
#include <QWidget>
#include <QLabel>
#include <QProgressBar>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLayout>

class ProfileWindow : public QWidget {
    Q_OBJECT
public:
    explicit ProfileWindow(QWidget* parent = nullptr);
    void refreshProfile();

    // Called from main.cpp AI connections
    void onSmartPathReady(const QString& text);
    void onSmartPathError(const QString& msg);

signals:
    void goBack();
    void openSmartPath();

private:
    void setupUI();
    void clearLayout(QLayout* layout);

    QLabel*       m_nameLabel       = nullptr;
    QLabel*       m_levelLabel      = nullptr;
    QLabel*       m_xpLabel         = nullptr;
    QProgressBar* m_xpBar           = nullptr;
    QPushButton*  m_smartPathBtn    = nullptr;
    QLabel*       m_smartPathResult = nullptr;
    QVBoxLayout*  m_badgesLay       = nullptr;
    QVBoxLayout*  m_topicsLay       = nullptr;
    QVBoxLayout*  m_lessonsLay      = nullptr;
};
