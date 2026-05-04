#include "AuthController.h"
#include <QDir>
#include <QStandardPaths>

AuthController& AuthController::instance() {
    static AuthController inst;
    return inst;
}

QString AuthController::userFilePath(const QString& username) const {
    QString dir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)
                  + "/users/";
    QDir().mkpath(dir);
    return dir + username + ".json";
}

bool AuthController::usernameExists(const QString& username) const {
    return QFile::exists(userFilePath(username));
}

bool AuthController::registerUser(const QString& username,
                                   const QString& email,
                                   const QString& password,
                                   QString& errorMsg) {
    if (username.trimmed().isEmpty()) { errorMsg = "Username cannot be empty."; return false; }
    if (password.length() < 6)        { errorMsg = "Password must be at least 6 characters."; return false; }
    if (!email.contains("@"))         { errorMsg = "Invalid email address."; return false; }
    if (usernameExists(username))     { errorMsg = "Username already taken."; return false; }

    User newUser(username, email, User::hashPassword(password));
    newUser.addBadge("first_step");
    return newUser.saveToFile(userFilePath(username));
}

bool AuthController::loginUser(const QString& username,
                                const QString& password,
                                QString& errorMsg) {
    if (!usernameExists(username)) { errorMsg = "Username not found."; return false; }

    User u = User::loadFromFile(userFilePath(username));
    if (!u.verifyPassword(password)) { errorMsg = "Incorrect password."; return false; }

    m_currentUser = u;
    m_loggedIn    = true;
    emit userLoggedIn(m_currentUser);
    return true;
}

void AuthController::logout() {
    saveCurrentUser();
    m_loggedIn = false;
    emit userLoggedOut();
}

bool AuthController::isLoggedIn() const { return m_loggedIn; }

User& AuthController::currentUser() { return m_currentUser; }

void AuthController::saveCurrentUser() {
    if (m_loggedIn)
        m_currentUser.saveToFile(userFilePath(m_currentUser.getUsername()));
}
