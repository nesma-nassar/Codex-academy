#pragma once
#include <QString>
#include <QVector>

struct Badge {
    QString id;
    QString title;
    QString description;
    QString emoji;
    QString color;

    static QVector<Badge> allBadges() {
        return {
            {"first_lesson",  "First Step",      "Completed your first lesson",    "🎯", "#10b981"},
            {"quiz_master",   "Quiz Master",      "Got 5 quiz questions correct",   "🧠", "#7c3aed"},
            {"cpp_basics",    "C++ Basics",       "Finished Hello World lesson",    "⚡", "#00d4ff"},
            {"loop_hero",     "Loop Hero",        "Mastered Loops lesson",          "🔄", "#f59e0b"},
            {"level_2",       "Level 2 Achieved", "Reached Level 2",               "🏆", "#f59e0b"},
            {"level_3",       "Level 3 Achieved", "Reached Level 3",               "💎", "#00d4ff"},
            {"streak_3",      "3-Day Streak",     "Learned 3 days in a row",       "🔥", "#ef4444"},
            {"all_lessons",   "Graduate",         "Completed all C++ lessons",      "🎓", "#10b981"},
        };
    }

    static Badge find(const QString& id) {
        for (const auto& b : allBadges())
            if (b.id == id) return b;
        return {id, id, "", "🏅", "#64748b"};
    }
};
