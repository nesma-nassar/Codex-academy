#pragma once
#include <QObject>
#include "User.h"

class ProfileController : public QObject {
    Q_OBJECT
public:
    static ProfileController& instance();
    // Returns a summary stats string for the profile page
    QString getSummary(const User& u) const {
        return QString("Level %1 | %2 XP | %3 lessons completed")
            .arg(u.getLevel()).arg(u.getXP())
            .arg(u.completedLessons().size());
    }
private:
    ProfileController() = default;
};
