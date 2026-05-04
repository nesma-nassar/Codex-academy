#pragma once
#include <QString>
#include <QVector>
#include <QJsonObject>

// ── Content item base (Abstraction + Polymorphism) ───────────
class ContentItem {
public:
    virtual ~ContentItem() = default;
    virtual QString getType()    const = 0;
    virtual QString getContent() const = 0;
};

class TextContent : public ContentItem {
public:
    explicit TextContent(const QString& text) : m_text(text) {}
    QString getType()    const override { return "text"; }
    QString getContent() const override { return m_text; }
private:
    QString m_text;
};

class CodeContent : public ContentItem {
public:
    explicit CodeContent(const QString& code) : m_code(code) {}
    QString getType()    const override { return "code"; }
    QString getContent() const override { return m_code; }
private:
    QString m_code;
};

// ── Lesson ───────────────────────────────────────────────────
class Lesson {
public:
    Lesson() = default;
    Lesson(const QString& id,
           const QString& title,
           const QString& topic,
           int xpReward,
           const QString& prerequisiteId = "");

    // Getters (Encapsulation)
    QString getId()            const { return m_id;             }
    QString getTitle()         const { return m_title;          }
    QString getTopic()         const { return m_topic;          }
    QString getDescription()   const { return m_description;    }
    QString getVideoPath()     const { return m_videoPath;      }
    QString getPrerequisiteId()const { return m_prerequisiteId; }
    int     getXPReward()      const { return m_xpReward;       }
    int     getOrder()         const { return m_order;          }

    // Setters
    void setDescription(const QString& d) { m_description = d; }
    void setVideoPath(const QString& v)   { m_videoPath   = v; }
    void setOrder(int o)                  { m_order       = o; }

    // Content blocks (theory text + code snippets)
    void addContent(ContentItem* item)     { m_content.append(item); }
    const QVector<ContentItem*>& getContent() const { return m_content; }

    // Serialization
    QJsonObject toJson()              const;
    static Lesson fromJson(const QJsonObject& obj);

    // Static factory: returns all built-in C++ lessons
    static QVector<Lesson> defaultCppLessons();

private:
    QString  m_id;
    QString  m_title;
    QString  m_topic;
    QString  m_description;
    QString  m_videoPath;
    QString  m_prerequisiteId;
    int      m_xpReward = 50;
    int      m_order    = 0;

    QVector<ContentItem*> m_content;
};
