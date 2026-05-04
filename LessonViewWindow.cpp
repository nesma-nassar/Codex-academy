#include "LessonViewWindow.h"
#include "Lesson.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFrame>
#include <QScrollArea>
#include <QLabel>
#include <QPixmap>
#include <QFileInfo>
#include <QDir>
#include <QStandardPaths>

LessonViewWindow::LessonViewWindow(QWidget* parent) : QWidget(parent) {
    setWindowTitle("Lesson — Codex Academy");
    setMinimumSize(860, 680);
    setupUI();
}

void LessonViewWindow::setupUI() {
    auto* root = new QVBoxLayout(this);
    root->setContentsMargins(0,0,0,0);
    root->setSpacing(0);

    // ── Nav ────────────────────────────────────────────────────
    auto* nav = new QFrame();
    nav->setFixedHeight(58);
    nav->setStyleSheet("QFrame{background:#1a1f2e;border-bottom:2px solid #2a3040;}");
    auto* nl = new QHBoxLayout(nav);
    nl->setContentsMargins(20,0,20,0);
    m_backBtn = new QPushButton("<- Back");
    m_backBtn->setStyleSheet(
        "QPushButton{background:transparent;color:#6b7280;border:none;"
        "font-size:14px;font-weight:700;}"
        "QPushButton:hover{color:#1cb0f6;}");
    connect(m_backBtn, &QPushButton::clicked, this, &LessonViewWindow::goBack);
    nl->addWidget(m_backBtn); nl->addStretch();
    auto* brand = new QLabel("Codex Academy");
    brand->setStyleSheet("color:#e8eaf0;font-size:15px;font-weight:800;");
    nl->addWidget(brand); nl->addStretch(); nl->addSpacing(70);
    root->addWidget(nav);

    // ── Scroll ─────────────────────────────────────────────────
    auto* scroll = new QScrollArea();
    scroll->setWidgetResizable(true);
    scroll->setStyleSheet("QScrollArea{border:none;background:#0f1117;}");
    auto* content = new QWidget();
    content->setStyleSheet("background:#0f1117;");
    auto* cLay = new QVBoxLayout(content);
    cLay->setContentsMargins(36,28,36,28);
    cLay->setSpacing(18);

    // Title
    m_titleLabel = new QLabel("Lesson");
    m_titleLabel->setStyleSheet(
        "font-size:24px;font-weight:900;color:#e8eaf0;");
    cLay->addWidget(m_titleLabel);

    // ── Content Stack: Video (0) or Text (1) ──────────────────
    m_contentStack = new QStackedWidget();

    // ── PAGE 0: Video player ───────────────────────────────────
    m_videoContainer = new QWidget();
    m_videoContainer->setStyleSheet("background:#0f1117;");
    auto* vcLay = new QVBoxLayout(m_videoContainer);
    vcLay->setContentsMargins(0,0,0,0); vcLay->setSpacing(12);

    // Video area - placeholder until video loads
    auto* videoFrame = new QFrame();
    videoFrame->setFixedHeight(360);
    videoFrame->setStyleSheet(
        "QFrame{background:#0d1117;border:2px solid #2a3040;"
        "border-radius:14px;}");
    auto* vfLay = new QVBoxLayout(videoFrame);

    m_videoPlaceholder = new QLabel();
    m_videoPlaceholder->setAlignment(Qt::AlignCenter);
    m_videoPlaceholder->setStyleSheet("color:#6b7280;font-size:14px;");
    m_videoPlaceholder->setWordWrap(true);
    vfLay->addWidget(m_videoPlaceholder);
    vcLay->addWidget(videoFrame);

    // Video status
    m_videoStatus = new QLabel();
    m_videoStatus->setAlignment(Qt::AlignCenter);
    m_videoStatus->setStyleSheet("color:#1cb0f6;font-size:13px;font-weight:700;");
    vcLay->addWidget(m_videoStatus);

    // Video path info (shows where to put the file)
    m_videoPath = new QLabel();
    m_videoPath->setAlignment(Qt::AlignCenter);
    m_videoPath->setWordWrap(true);
    m_videoPath->setStyleSheet(
        "color:#9ca3af;font-size:12px;"
        "background:#1a1f2e;border:2px solid #2a3040;"
        "border-radius:10px;padding:12px;");
    vcLay->addWidget(m_videoPath);

    m_contentStack->addWidget(m_videoContainer); // page 0

    // ── PAGE 1: Text fallback ──────────────────────────────────
    auto* textPage = new QWidget();
    textPage->setStyleSheet("background:#0f1117;");
    auto* tpLay = new QVBoxLayout(textPage);
    tpLay->setContentsMargins(0,0,0,0); tpLay->setSpacing(14);

    auto* theoryHdr = new QHBoxLayout();
    auto* tIcon = new QLabel("📖");
    tIcon->setStyleSheet("font-size:18px;");
    auto* tLbl = new QLabel("Theory");
    tLbl->setStyleSheet("color:#1cb0f6;font-size:14px;font-weight:800;letter-spacing:1px;");
    theoryHdr->addWidget(tIcon); theoryHdr->addSpacing(6);
    theoryHdr->addWidget(tLbl); theoryHdr->addStretch();
    tpLay->addLayout(theoryHdr);

    m_contentView = new QTextEdit();
    m_contentView->setReadOnly(true);
    m_contentView->setMinimumHeight(120);
    m_contentView->setStyleSheet(
        "QTextEdit{background:#1a1f2e;border:2px solid #2a3040;"
        "border-radius:12px;padding:16px;color:#e8eaf0;font-size:14px;}");
    tpLay->addWidget(m_contentView);

    // Code example
    auto* codeHdr = new QHBoxLayout();
    auto* cIcon = new QLabel("💻");
    cIcon->setStyleSheet("font-size:18px;");
    auto* cLbl2 = new QLabel("Code Example");
    cLbl2->setStyleSheet("color:#a5f3fc;font-size:14px;font-weight:800;letter-spacing:1px;");
    codeHdr->addWidget(cIcon); codeHdr->addSpacing(6);
    codeHdr->addWidget(cLbl2); codeHdr->addStretch();
    tpLay->addLayout(codeHdr);

    auto* codeFrame = new QFrame();
    codeFrame->setStyleSheet(
        "QFrame{background:#0d1117;border:2px solid #2a3040;border-radius:14px;}");
    auto* cfLay = new QVBoxLayout(codeFrame);
    cfLay->setContentsMargins(0,0,0,0);

    auto* codeBar = new QFrame();
    codeBar->setFixedHeight(34);
    codeBar->setStyleSheet(
        "QFrame{background:#1a1f2e;border-radius:12px 12px 0 0;"
        "border-bottom:2px solid #2a3040;}");
    auto* cbLay = new QHBoxLayout(codeBar);
    cbLay->setContentsMargins(12,0,12,0);
    auto* d1=new QLabel("●"); d1->setStyleSheet("color:#ff5f57;font-size:13px;");
    auto* d2=new QLabel("●"); d2->setStyleSheet("color:#ffbd2e;font-size:13px;");
    auto* d3=new QLabel("●"); d3->setStyleSheet("color:#28c940;font-size:13px;");
    auto* fn=new QLabel("example.cpp");
    fn->setStyleSheet("color:#6b7280;font-size:12px;margin-left:8px;");
    cbLay->addWidget(d1); cbLay->addWidget(d2); cbLay->addWidget(d3);
    cbLay->addWidget(fn); cbLay->addStretch();
    cfLay->addWidget(codeBar);

    m_codeView = new QTextEdit();
    m_codeView->setReadOnly(true);
    m_codeView->setMinimumHeight(160);
    m_codeView->setStyleSheet(
        "QTextEdit{background:#0d1117;border:none;border-radius:0 0 12px 12px;"
        "padding:16px;color:#a5f3fc;"
        "font-family:'Consolas','Courier New',monospace;font-size:13px;}");
    cfLay->addWidget(m_codeView);
    tpLay->addWidget(codeFrame);

    m_contentStack->addWidget(textPage); // page 1

    cLay->addWidget(m_contentStack, 1);

    // ── Quiz button ────────────────────────────────────────────
    m_quizBtn = new QPushButton("Start Quiz  ->");
    m_quizBtn->setFixedHeight(54);
    m_quizBtn->setStyleSheet(R"(
        QPushButton{background:#1cb0f6;color:white;border:none;
            border-bottom:4px solid #1899d6;border-radius:14px;
            font-size:16px;font-weight:800;letter-spacing:1px;}
        QPushButton:hover{background:#3dbff8;}
        QPushButton:pressed{background:#1899d6;border-bottom:2px solid #1899d6;margin-top:2px;}
    )");
    connect(m_quizBtn, &QPushButton::clicked, this, [this]{
        emit lessonDone(m_currentLessonId);
    });
    cLay->addWidget(m_quizBtn);

    scroll->setWidget(content);
    root->addWidget(scroll);
}

void LessonViewWindow::loadLesson(const QString& lessonId) {
    m_currentLessonId = lessonId;
    auto lessons = Lesson::defaultCppLessons();

    for (const auto& l : lessons) {
        if (l.getId() != lessonId) continue;

        m_titleLabel->setText(l.getTitle());

        // Check for video file
        // Videos should be placed in: <AppDir>/videos/  or  <Downloads>/CodexAcademy/videos/
        QStringList searchPaths = {
            QDir::currentPath() + "/videos/" + lessonId + ".mp4",
            QDir::currentPath() + "/videos/" + lessonId + ".avi",
            QDir::currentPath() + "/videos/" + lessonId + ".mkv",
            QStandardPaths::writableLocation(QStandardPaths::DownloadLocation)
                + "/CodexAcademy/videos/" + lessonId + ".mp4",
            QStandardPaths::writableLocation(QStandardPaths::MoviesLocation)
                + "/" + lessonId + ".mp4",
        };

        // Also check the video path stored in lesson
        if (!l.getVideoPath().isEmpty())
            searchPaths.prepend(l.getVideoPath());

        QString foundVideo;
        for (const QString& p : searchPaths) {
            if (QFileInfo::exists(p)) { foundVideo = p; break; }
        }

        if (!foundVideo.isEmpty()) {
            // Video found — show video player page
            m_contentStack->setCurrentIndex(0);
            m_videoPlaceholder->setText(
                "🎬\n\nVideo: " + QFileInfo(foundVideo).fileName() +
                "\n\nVideo playback requires QMultimedia.\n"
                "Place your video at the path shown below.");
            m_videoStatus->setText("✅  Video file found: " + foundVideo);
            m_videoPath->setText(
                "📁  Video path:\n" + foundVideo);
        } else {
            // No video — show text content
            m_contentStack->setCurrentIndex(1);

            QString theory, code;
            for (auto* item : l.getContent()) {
                if (item->getType() == "text") theory += item->getContent();
                else                           code   += item->getContent();
            }
            m_contentView->setPlainText(theory.trimmed());
            m_codeView->setPlainText(code.trimmed());

            // Show where to put the video
            QString expectedPath = QDir::currentPath() + "/videos/" + lessonId + ".mp4";
            m_videoPath->hide(); // hide video path label on text page
        }

        // Update video path label with instructions
        if (!m_videoPath->isHidden()) {
            m_videoPath->setText(
                "📁  To add your video, copy it to:\n" +
                QDir::currentPath() + "/videos/" + lessonId + ".mp4\n\n"
                "Supported formats: .mp4  .avi  .mkv");
        }

        break;
    }
}

void LessonViewWindow::onPlayPause() {}
void LessonViewWindow::onVideoFinished() {}
bool LessonViewWindow::tryLoadVideo(const QString&) { return false; }
