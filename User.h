#pragma once
#include <QString>
#include <QVector>
#include <QJsonObject>
#include <QDateTime>
#include <QMap>
#include <QPair>

// ════════════════════════════════════════════════════════════
//  Person  –  Abstract base class  (Abstraction + Inheritance)
// ════════════════════════════════════════════════════════════
class Person {
public:
    Person() = default;
    Person(const QString& name, const QString& email);
    virtual ~Person() = default;

    // Pure virtual → forces subclasses to implement (Abstraction)
    virtual QString getRole()    const = 0;
    virtual QJsonObject toJson() const = 0;

    // Getters / Setters  (Encapsulation)
    QString getName()  const { return m_name;  }
    QString getEmail() const { return m_email; }
    void    setName (const QString& n) { m_name  = n; }
    void    setEmail(const QString& e) { m_email = e; }

protected:
    QString m_name;
    QString m_email;
};

// ════════════════════════════════════════════════════════════
//  User  –  Inherits Person  (Inheritance)
// ════════════════════════════════════════════════════════════
class User : public Person {
public:
    User() = default;
    User(const QString& username,
         const QString& email,
         const QString& passwordHash);

    // ── Overrides (Polymorphism) ──
    QString    getRole()    const override { return "Student"; }
    QJsonObject toJson()   const override;
    static User fromJson(const QJsonObject& obj);

    // ── Authentication ──
    bool   verifyPassword(const QString& plainPassword) const;
    void   setPassword(const QString& plainPassword);
    static QString hashPassword(const QString& plain);

    // ── Progress / XP ──
    int    getXP()             const { return m_xp;    }
    int    getLevel()          const { return m_level; }
    void   addXP(int amount);
    float  getLevelProgress()  const;   // 0.0 – 1.0
    int    getXPToNextLevel()  const;

    // ── Lessons ──
    bool   hasCompletedLesson(const QString& lessonId) const;
    void   markLessonComplete(const QString& lessonId, int xpEarned);
    const QVector<QString>& completedLessons() const { return m_completedLessons; }

    // ── Topic performance (for Smart Path) ──
    void  recordAnswer(const QString& topic, bool correct);
    float getTopicScore(const QString& topic) const;  // 0.0 – 1.0
    QMap<QString,QPair<int,int>> getTopicStats() const { return m_topicStats; }

    // ── Badges ──
    void   addBadge(const QString& badge);
    const QVector<QString>& getBadges() const { return m_badges; }

    // ── Persistence ──
    bool saveToFile(const QString& path) const;
    static User loadFromFile(const QString& path);

    // Basic getters
    QString getUsername()     const { return m_username;     }
    QString getPasswordHash() const { return m_passwordHash; }
    QString getAvatarColor()  const { return m_avatarColor;  }
    QDateTime getCreatedAt()  const { return m_createdAt;    }

private:
    QString   m_username;
    QString   m_passwordHash;
    QString   m_avatarColor  = "#7c3aed";
    int       m_xp           = 0;
    int       m_level        = 1;
    QDateTime m_createdAt;

    QVector<QString>             m_completedLessons;
    QVector<QString>             m_badges;
    QMap<QString,QPair<int,int>> m_topicStats; // topic → {correct, total}

    void checkLevelUp();
    static int xpForLevel(int level);
};
