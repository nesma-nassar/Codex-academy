# Codex Academy 🚀
### منصة تعليم C++ بالذكاء الاصطناعي

---

## ⚡ خطوات التشغيل السريعة

1. افتح `CodeQuestAcademy.pro` في Qt Creator
2. افتح `src/main.cpp` وحط الـ Gemini API key:
   ```cpp
   AIManager::instance().setApiKey("YOUR_GEMINI_KEY_HERE");
   ```
3. اضغط Build (Ctrl+B) ثم Run (Ctrl+R)

---

## 🎬 إزاي تضيف فيديوهاتك

**المجلد:** اعمل مجلد `videos` جنب ملف الـ `.exe`

**أسماء الملفات:**
| الدرس | اسم الفيديو |
|-------|------------|
| Lesson 1: Intro to C++ & OOP | `cpp_01.mp4` |
| Lesson 2: Encapsulation | `cpp_02.mp4` |
| Lesson 3: Inheritance | `cpp_03.mp4` |
| Lesson 4: Polymorphism | `cpp_04.mp4` |
| Lesson 5: Abstraction | `cpp_05.mp4` |

**المسار النهائي:**
```
CodeQuestAcademy/
├── CodeQuestAcademy.exe
└── videos/
    ├── cpp_01.mp4   ← درس 1
    ├── cpp_02.mp4   ← درس 2
    ├── cpp_03.mp4   ← درس 3
    ├── cpp_04.mp4   ← درس 4
    └── cpp_05.mp4   ← درس 5
```

**لو الفيديو مش موجود:** البرنامج هيعرض الشرح النصي تلقائياً.
**الفورمات المدعومة:** `.mp4` و `.avi` و `.mkv`

---

## 🤖 AI Features

| Feature | كيفية الوصول |
|---------|-------------|
| AI Code Helper | Dashboard ← 🔍 AI Code Helper |
| AI Chatbot | Dashboard ← 💬 Ask AI Assistant |
| Smart Learning Path | Profile ← 🗺️ Get Smart Recommendation |
| AI Help في Quiz | زرار "💬 Ask AI" في كل سؤال |

---

## 🔑 Gemini API Key

1. روح على: **aistudio.google.com**
2. اضغط **Get API Key**
3. حط الـ key في `src/main.cpp`:
   ```cpp
   AIManager::instance().setApiKey("AIza...");
   ```

---

## 📁 Project Structure

```
CodeQuestAcademy/
├── src/
│   ├── main.cpp              ← Entry point + AI connections
│   ├── ai/
│   │   ├── AIManager.cpp     ← Gemini API wrapper
│   │   └── SmartPathEngine.cpp
│   ├── models/
│   │   ├── User.cpp
│   │   ├── Lesson.cpp        ← 5 lessons
│   │   └── Question.cpp      ← 12 questions/lesson (10 random)
│   ├── views/
│   │   ├── LoginWindow.cpp
│   │   ├── DashboardWindow.cpp
│   │   ├── LessonListWindow.cpp
│   │   ├── LessonViewWindow.cpp  ← Video support
│   │   ├── QuizWindow.cpp        ← Robot mascot
│   │   ├── ProfileWindow.cpp     ← Smart Path
│   │   ├── AIHelperWindow.cpp    ← Code Helper
│   │   └── ChatBotWindow.cpp     ← AI Chat
│   └── controllers/
└── include/
```
