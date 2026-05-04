#pragma once
#include <QString>

namespace Style {

// ── Codex Academy – Duolingo-inspired palette ────────────────
// Primary green like Duolingo + dark theme
inline QString appStyle() {
    return R"(
QWidget {
    background-color: #0f1117;
    color: #e8eaf0;
    font-family: "Segoe UI", "Arial", sans-serif;
    font-size: 14px;
}

/* ── Primary Button (green like Duolingo) ── */
QPushButton {
    background-color: #1cb0f6;
    color: white;
    border: none;
    border-bottom: 4px solid #1899d6;
    border-radius: 12px;
    padding: 11px 24px;
    font-size: 14px;
    font-weight: 800;
    letter-spacing: 0.5px;
    text-transform: uppercase;
}
QPushButton:hover {
    background-color: #3dbff8;
}
QPushButton:pressed {
    background-color: #1899d6;
    border-bottom: 2px solid #1899d6;
    margin-top: 2px;
}
QPushButton:disabled {
    background-color: #1e2433;
    border-bottom: 4px solid #141820;
    color: #3d4455;
}

/* ── Secondary Button ── */
QPushButton#secondaryBtn {
    background-color: #1a1f2e;
    border: 2px solid #2a3040;
    border-bottom: 4px solid #141820;
    color: #e8eaf0;
}
QPushButton#secondaryBtn:hover {
    border-color: #1cb0f6;
    color: #1cb0f6;
}

/* ── Danger Button ── */
QPushButton#dangerBtn {
    background-color: #ff4b4b;
    border-bottom: 4px solid #cc0000;
}
QPushButton#dangerBtn:hover { background-color: #ff6060; }

/* ── LineEdit ── */
QLineEdit {
    background: #1a1f2e;
    border: 2px solid #2a3040;
    border-radius: 10px;
    padding: 10px 14px;
    color: #e8eaf0;
    font-size: 14px;
}
QLineEdit:focus { border-color: #1cb0f6; }

/* ── TextEdit / PlainTextEdit ── */
QTextEdit, QPlainTextEdit {
    background: #1a1f2e;
    border: 2px solid #2a3040;
    border-radius: 10px;
    padding: 10px;
    color: #a5f3fc;
    font-family: "Consolas","Courier New",monospace;
    font-size: 13px;
}
QTextEdit:focus, QPlainTextEdit:focus { border-color: #1cb0f6; }

/* ── Labels ── */
QLabel#titleLabel   { font-size:26px; font-weight:900; color:#e8eaf0; }
QLabel#subtitleLabel{ font-size:14px; color:#6b7280; }
QLabel#accentLabel  { color:#1cb0f6; font-weight:bold; }
QLabel#successLabel { color:#1cb0f6; font-weight:bold; }
QLabel#errorLabel   { color:#ff4b4b; font-weight:bold; }
QLabel#xpLabel      { color:#ffd900; font-weight:bold; }

/* ── Cards ── */
QFrame#card {
    background: #1a1f2e;
    border: 2px solid #2a3040;
    border-radius: 16px;
}
QFrame#card:hover { border-color: #1cb0f6; }

QFrame#lessonCard {
    background: #1a1f2e;
    border: 2px solid #2a3040;
    border-radius: 14px;
}
QFrame#lessonCard:hover { border-color: #1cb0f6; }

QFrame#lockedCard {
    background: #13161e;
    border: 2px solid #1e2433;
    border-radius: 14px;
}

/* ── Progress Bar (XP bar like Duolingo) ── */
QProgressBar {
    background: #1a1f2e;
    border: none;
    border-radius: 999px;
    height: 12px;
    text-align: center;
    color: transparent;
}
QProgressBar::chunk {
    background: qlineargradient(x1:0,y1:0,x2:1,y2:0,
        stop:0 #1cb0f6, stop:1 #5ecfff);
    border-radius: 999px;
}

/* ── ListWidget ── */
QListWidget {
    background: #0f1117;
    border: none;
    outline: none;
    padding: 4px;
}
QListWidget::item {
    background: #1a1f2e;
    border: 2px solid #2a3040;
    border-radius: 12px;
    padding: 14px 16px;
    margin: 4px 2px;
    color: #e8eaf0;
}
QListWidget::item:selected {
    background: rgba(28,176,246,0.15);
    border-color: #1cb0f6;
    color: white;
}
QListWidget::item:hover:!selected { border-color: #1cb0f6; }

/* ── ScrollBar ── */
QScrollBar:vertical {
    background: #0f1117;
    width: 6px;
    border-radius: 3px;
}
QScrollBar::handle:vertical {
    background: #2a3040;
    border-radius: 3px;
    min-height: 30px;
}
QScrollBar::handle:vertical:hover { background: #1cb0f6; }
QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical { height:0; }

/* ── ComboBox ── */
QComboBox {
    background: #1a1f2e;
    border: 2px solid #2a3040;
    border-radius: 10px;
    padding: 8px 14px;
    color: #e8eaf0;
}
QComboBox:focus { border-color: #1cb0f6; }
QComboBox QAbstractItemView {
    background: #1a1f2e;
    border: 2px solid #2a3040;
    selection-background-color: rgba(28,176,246,0.3);
}

/* ── Tabs ── */
QTabBar::tab {
    background: #1a1f2e;
    border: 2px solid #2a3040;
    border-radius: 10px;
    padding: 8px 20px;
    margin: 3px;
    color: #6b7280;
    font-weight: 700;
}
QTabBar::tab:selected {
    background: #1cb0f6;
    color: white;
    border-color: #1899d6;
}
QTabBar::tab:hover:!selected { color: #e8eaf0; }

/* ── Splitter ── */
QSplitter::handle { background: #2a3040; width: 2px; }

/* ── MessageBox ── */
QMessageBox { background: #1a1f2e; color: #e8eaf0; }
QMessageBox QPushButton { min-width:80px; padding:8px 16px; }
)";
}

} // namespace Style
