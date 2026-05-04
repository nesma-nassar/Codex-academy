#include "ProfileController.h"
ProfileController& ProfileController::instance() {
    static ProfileController inst; return inst;
}
