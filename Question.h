#pragma once
#include <QString>
#include <QVector>
#include <QJsonObject>

// ── Abstract Question (Abstraction + Polymorphism) ────────────
class Question {
public:
    enum Type { MCQ, WRITING };

    explicit Question(const QString& id,
                      const QString& lessonId,
                      const QString& text,
                      const QString& topic,
                      int xpReward = 20);
    virtual ~Question() = default;

    virtual Type    getType()                            const = 0;
    virtual bool    checkAnswer(const QString& answer)   const = 0;
    virtual QString getCorrectAnswer()                   const = 0;
    virtual QJsonObject toJson()                         const;

    // Common getters (Encapsulation)
    QString getId()       const { return m_id;       }
    QString getLessonId() const { return m_lessonId; }
    QString getText()     const { return m_text;     }
    QString getTopic()    const { return m_topic;    }
    int     getXPReward() const { return m_xpReward; }

protected:
    QString m_id;
    QString m_lessonId;
    QString m_text;
    QString m_topic;
    int     m_xpReward;
};

// ── MCQ Question ─────────────────────────────────────────────
class MCQQuestion : public Question {
public:
    MCQQuestion(const QString& id,
                const QString& lessonId,
                const QString& text,
                const QString& topic,
                const QVector<QString>& choices,
                int correctIndex,
                int xpReward = 20);

    Type    getType()                          const override { return MCQ; }
    bool    checkAnswer(const QString& answer) const override;
    QString getCorrectAnswer()                 const override;
    QJsonObject toJson()                       const override;

    const QVector<QString>& getChoices() const { return m_choices; }
    int getCorrectIndex()                const { return m_correctIndex; }

private:
    QVector<QString> m_choices;
    int              m_correctIndex;
};

// ── Writing (code) Question ───────────────────────────────────
class WritingQuestion : public Question {
public:
    WritingQuestion(const QString& id,
                    const QString& lessonId,
                    const QString& text,
                    const QString& topic,
                    const QString& expectedOutput,
                    const QString& hint = "",
                    int xpReward = 30);

    Type    getType()                          const override { return WRITING; }
    bool    checkAnswer(const QString& answer) const override;
    QString getCorrectAnswer()                 const override { return m_expectedOutput; }
    QJsonObject toJson()                       const override;

    QString getHint()           const { return m_hint;           }
    QString getExpectedOutput() const { return m_expectedOutput; }
    QString getSampleSolution() const { return m_sampleSolution; }
    void    setSampleSolution(const QString& s) { m_sampleSolution = s; }

private:
    QString m_expectedOutput;
    QString m_hint;
    QString m_sampleSolution;

    // Normalise whitespace before comparing
    static QString normalise(const QString& s);
};

// ── Factory ──────────────────────────────────────────────────
class QuestionFactory {
public:
    static QVector<Question*> questionsForLesson(const QString& lessonId);
};
