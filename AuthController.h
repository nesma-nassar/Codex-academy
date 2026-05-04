#pragma once
#include <QObject>
#include <QString>
#include <QMap>
#include "User.h"

// ── AuthController  (Controller layer – MVC) ─────────────────
class AuthController : public QObject {
    Q_OBJECT
public:
    static AuthController& instance();

    bool        registerUser(const QString& username,
                             const QString& email,
                             const QString& password,
                             QString& errorMsg);

    bool        loginUser(const QString& username,
                          const QString& password,
                          QString& errorMsg);

    void        logout();
    bool        isLoggedIn() const;
    User&       currentUser();
    void        saveCurrentUser();

signals:
    void userLoggedIn(const User& user);
    void userLoggedOut();

private:
    AuthController() = default;
    QString userFilePath(const QString& username) const;
    bool    usernameExists(const QString& username) const;

    User m_currentUser;
    bool m_loggedIn = false;
};
