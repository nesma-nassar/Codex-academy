#pragma once
#include <QObject>
#include "Question.h"
#include "User.h"

class QuizController : public QObject {
    Q_OBJECT
public:
    static QuizController& instance();
    // XP calculation: base reward + streak bonus
    int calculateXP(bool correct, int baseXP, int streak) const {
        return correct ? baseXP + (streak > 2 ? 5 : 0) : 0;
    }
private:
    QuizController() = default;
};
