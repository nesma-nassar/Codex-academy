#pragma once
#include <QObject>
#include "Lesson.h"
#include "User.h"

class LessonController : public QObject {
    Q_OBJECT
public:
    static LessonController& instance();
    QVector<Lesson> getLessons() const { return Lesson::defaultCppLessons(); }
    bool isUnlocked(const Lesson& l, const User& u) const {
        return l.getPrerequisiteId().isEmpty()
               || u.hasCompletedLesson(l.getPrerequisiteId());
    }
private:
    LessonController() = default;
};
