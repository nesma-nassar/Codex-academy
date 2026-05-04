#include "User.h"
#include <QCryptographicHash>
#include <QJsonArray>
#include <QJsonDocument>
#include <QFile>
#include <QDir>

// ── Person ──────────────────────────────────────────────────
Person::Person(const QString& name, const QString& email)
    : m_name(name), m_email(email) {}

// ── User ────────────────────────────────────────────────────
User::User(const QString& username, const QString& email, const QString& passwordHash)
    : Person(username, email)
    , m_username(username)
    , m_passwordHash(passwordHash)
    , m_createdAt(QDateTime::currentDateTime())
{}

// ── Password ─────────────────────────────────────────────────
QString User::hashPassword(const QString& plain) {
    return QString(QCryptographicHash::hash(
        plain.toUtf8(), QCryptographicHash::Sha256).toHex());
}

void User::setPassword(const QString& plain) {
    m_passwordHash = hashPassword(plain);
}

bool User::verifyPassword(const QString& plain) const {
    return m_passwordHash == hashPassword(plain);
}

// ── XP / Level ───────────────────────────────────────────────
int User::xpForLevel(int level) {
    return level * 200;   // Level 1→2 = 200 XP, Level 2→3 = 400 XP …
}

void User::addXP(int amount) {
    m_xp += amount;
    checkLevelUp();
}

void User::checkLevelUp() {
    while (m_xp >= xpForLevel(m_level)) {
        m_xp   -= xpForLevel(m_level);
        m_level++;
        addBadge(QString("Level %1 Achieved").arg(m_level));
    }
}

float User::getLevelProgress() const {
    int needed = xpForLevel(m_level);
    return needed > 0 ? (float)m_xp / needed : 0.0f;
}

int User::getXPToNextLevel() const {
    return xpForLevel(m_level) - m_xp;
}

// ── Lessons ──────────────────────────────────────────────────
bool User::hasCompletedLesson(const QString& lessonId) const {
    return m_completedLessons.contains(lessonId);
}

void User::markLessonComplete(const QString& lessonId, int xpEarned) {
    if (!hasCompletedLesson(lessonId)) {
        m_completedLessons.append(lessonId);
        addXP(xpEarned);
    }
}

// ── Topic Stats ──────────────────────────────────────────────
void User::recordAnswer(const QString& topic, bool correct) {
    auto& p = m_topicStats[topic];
    if (correct) p.first++;
    p.second++;
}

float User::getTopicScore(const QString& topic) const {
    if (!m_topicStats.contains(topic)) return 0.0f;
    auto p = m_topicStats[topic];
    return p.second > 0 ? (float)p.first / p.second : 0.0f;
}

// ── Badges ───────────────────────────────────────────────────
void User::addBadge(const QString& badge) {
    if (!m_badges.contains(badge))
        m_badges.append(badge);
}

// ── JSON ─────────────────────────────────────────────────────
QJsonObject User::toJson() const {
    QJsonObject obj;
    obj["username"]     = m_username;
    obj["email"]        = m_email;
    obj["passwordHash"] = m_passwordHash;
    obj["xp"]           = m_xp;
    obj["level"]        = m_level;
    obj["avatarColor"]  = m_avatarColor;
    obj["createdAt"]    = m_createdAt.toString(Qt::ISODate);

    QJsonArray lessons;
    for (const auto& l : m_completedLessons) lessons.append(l);
    obj["completedLessons"] = lessons;

    QJsonArray badges;
    for (const auto& b : m_badges) badges.append(b);
    obj["badges"] = badges;

    QJsonObject stats;
    for (auto it = m_topicStats.begin(); it != m_topicStats.end(); ++it) {
        QJsonObject tp;
        tp["correct"] = it.value().first;
        tp["total"]   = it.value().second;
        stats[it.key()] = tp;
    }
    obj["topicStats"] = stats;

    return obj;
}

User User::fromJson(const QJsonObject& obj) {
    User u;
    u.m_username     = obj["username"].toString();
    u.m_email        = obj["email"].toString();
    u.m_name         = u.m_username;
    u.m_passwordHash = obj["passwordHash"].toString();
    u.m_xp           = obj["xp"].toInt();
    u.m_level        = obj["level"].toInt(1);
    u.m_avatarColor  = obj["avatarColor"].toString("#7c3aed");
    u.m_createdAt    = QDateTime::fromString(obj["createdAt"].toString(), Qt::ISODate);

    for (const auto& v : obj["completedLessons"].toArray())
        u.m_completedLessons.append(v.toString());

    for (const auto& v : obj["badges"].toArray())
        u.m_badges.append(v.toString());

    QJsonObject stats = obj["topicStats"].toObject();
    for (auto it = stats.begin(); it != stats.end(); ++it) {
        QJsonObject tp = it.value().toObject();
        u.m_topicStats[it.key()] = {tp["correct"].toInt(), tp["total"].toInt()};
    }
    return u;
}

// ── File persistence ─────────────────────────────────────────
bool User::saveToFile(const QString& path) const {
    QFile f(path);
    if (!f.open(QIODevice::WriteOnly)) return false;
    f.write(QJsonDocument(toJson()).toJson());
    return true;
}

User User::loadFromFile(const QString& path) {
    QFile f(path);
    if (!f.open(QIODevice::ReadOnly)) return User();
    return fromJson(QJsonDocument::fromJson(f.readAll()).object());
}
