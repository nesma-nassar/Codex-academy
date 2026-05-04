#pragma once
#include <QWidget>
#include <QListWidget>
#include <QPushButton>
#include <QLabel>
#include "Lesson.h"

class LessonListWindow : public QWidget {
    Q_OBJECT
public:
    explicit LessonListWindow(QWidget* parent = nullptr);
    void refreshLessons();

signals:
    void lessonSelected(const QString& lessonId);
    void goBack();

private:
    void setupUI();
    void onItemDoubleClicked(QListWidgetItem* item);

    QListWidget* m_list;
    QVector<Lesson> m_lessons;
};
