#pragma once
#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QTextEdit>
#include <QStackedWidget>
#include <QSlider>

class LessonViewWindow : public QWidget {
    Q_OBJECT
public:
    explicit LessonViewWindow(QWidget* parent = nullptr);
    void loadLesson(const QString& lessonId);

signals:
    void lessonDone(const QString& lessonId);
    void goBack();

private slots:
    void onPlayPause();
    void onVideoFinished();

private:
    void setupUI();
    void setupVideoPlayer();
    void setupTextFallback();
    bool tryLoadVideo(const QString& videoPath);

    // Nav
    QPushButton* m_backBtn    = nullptr;
    QLabel*      m_titleLabel = nullptr;

    // Content switcher
    QStackedWidget* m_contentStack = nullptr; // 0=video, 1=text

    // Video player widgets
    QWidget*     m_videoContainer  = nullptr;
    QLabel*      m_videoPlaceholder= nullptr;
    QPushButton* m_playPauseBtn    = nullptr;
    QLabel*      m_videoStatus     = nullptr;
    QLabel*      m_videoPath       = nullptr;

    // Text fallback
    QTextEdit*   m_contentView = nullptr;
    QTextEdit*   m_codeView    = nullptr;

    // Quiz button
    QPushButton* m_quizBtn = nullptr;

    QString m_currentLessonId;
    bool    m_videoLoaded = false;
};
