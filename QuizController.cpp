#include "QuizController.h"
QuizController& QuizController::instance() {
    static QuizController inst; return inst;
}
