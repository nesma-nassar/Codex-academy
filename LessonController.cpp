#include "LessonController.h"
LessonController& LessonController::instance() {
    static LessonController inst;
    return inst;
}
