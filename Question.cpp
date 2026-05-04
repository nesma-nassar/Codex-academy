#include "Question.h"
#include <QJsonArray>
#include <QRandomGenerator>
#include <algorithm>

// ── Question base ─────────────────────────────────────────────
Question::Question(const QString& id, const QString& lessonId,
                   const QString& text, const QString& topic, int xp)
    : m_id(id), m_lessonId(lessonId), m_text(text),
      m_topic(topic), m_xpReward(xp) {}

QJsonObject Question::toJson() const {
    QJsonObject o;
    o["id"] = m_id; o["lessonId"] = m_lessonId;
    o["text"] = m_text; o["topic"] = m_topic; o["xp"] = m_xpReward;
    return o;
}

// ── MCQ ───────────────────────────────────────────────────────
MCQQuestion::MCQQuestion(const QString& id, const QString& lessonId,
                         const QString& text, const QString& topic,
                         const QVector<QString>& choices, int correctIndex, int xp)
    : Question(id, lessonId, text, topic, xp)
    , m_choices(choices), m_correctIndex(correctIndex) {}

bool MCQQuestion::checkAnswer(const QString& answer) const {
    bool ok; int idx = answer.toInt(&ok);
    return ok && idx == m_correctIndex;
}
QString MCQQuestion::getCorrectAnswer() const {
    if (m_correctIndex >= 0 && m_correctIndex < m_choices.size())
        return m_choices[m_correctIndex];
    return "";
}
QJsonObject MCQQuestion::toJson() const {
    QJsonObject o = Question::toJson(); o["type"] = QString("MCQ");
    QJsonArray arr; for (const auto& c : m_choices) arr.append(c);
    o["choices"] = arr; o["correctIndex"] = m_correctIndex; return o;
}

// ── Writing ───────────────────────────────────────────────────
WritingQuestion::WritingQuestion(const QString& id, const QString& lessonId,
                                 const QString& text, const QString& topic,
                                 const QString& expectedOutput,
                                 const QString& hint, int xp)
    : Question(id, lessonId, text, topic, xp)
    , m_expectedOutput(expectedOutput), m_hint(hint) {}

QString WritingQuestion::normalise(const QString& s) {
    return s.simplified().toLower()
        .replace(" ","").replace("\t","").replace("\n","").replace(";","");
}
bool WritingQuestion::checkAnswer(const QString& answer) const {
    return normalise(answer) == normalise(m_expectedOutput);
}
QJsonObject WritingQuestion::toJson() const {
    QJsonObject o = Question::toJson(); o["type"] = QString("WRITING");
    o["expectedOutput"] = m_expectedOutput; o["hint"] = m_hint; return o;
}

// ════════════════════════════════════════════════════════════
//  FULL QUESTION BANK - 10 questions per lesson
//  Each lesson has TWO SETS of 10 → shuffle picks 10 randomly
//  so retrying gives different questions
// ════════════════════════════════════════════════════════════
QVector<Question*> QuestionFactory::questionsForLesson(const QString& id) {
    QVector<Question*> pool; // full pool, then shuffle and return all

    // ── LESSON 1: C++ & OOP Basics ───────────────────────────
    if (id == "cpp_01") {
        pool << new MCQQuestion("q01_01","cpp_01",
            "What does OOP stand for?","OOP Basics",
            {"Object Oriented Programming","Open Output Programming",
             "Ordered Object Process","Overloaded Operator Protocol"},0,20);

        pool << new MCQQuestion("q01_02","cpp_01",
            "Which year was C++ created?","OOP Basics",
            {"1972","1979","1991","2001"},1,20);

        pool << new MCQQuestion("q01_03","cpp_01",
            "What is a CLASS in OOP?","OOP Basics",
            {"A real instance of an object",
             "A blueprint/template for creating objects",
             "A variable that stores data",
             "A function that returns a value"},1,20);

        pool << new MCQQuestion("q01_04","cpp_01",
            "What is an OBJECT in OOP?","OOP Basics",
            {"A blueprint for creating instances",
             "A type of variable",
             "A real instance created from a class",
             "A function inside a class"},2,20);

        pool << new MCQQuestion("q01_05","cpp_01",
            "Which of these is an OBJECT (not a class)?","OOP Basics",
            {"Car","Animal",
             "myCar (Toyota, Red, 120 km/h)","BankAccount"},2,20);

        pool << new MCQQuestion("q01_06","cpp_01",
            "How many main principles does OOP have?","OOP Basics",
            {"2","3","4","5"},2,20);

        pool << new MCQQuestion("q01_07","cpp_01",
            "Which keyword defines a class in C++?","OOP Basics",
            {"object","struct","class","define"},2,20);

        pool << new MCQQuestion("q01_08","cpp_01",
            "In C++, how do you access a member of an object?","OOP Basics",
            {"object->member","object::member",
             "object.member","object[member]"},2,20);

        pool << new WritingQuestion("q01_09","cpp_01",
            "Name the 4 OOP principles separated by commas:",
            "OOP Basics",
            "Encapsulation, Inheritance, Polymorphism, Abstraction",
            "Think: E I P A",30);

        pool << new WritingQuestion("q01_10","cpp_01",
            "In the lesson example, what does myCar.drive() print?\n"
            "(brand is Toyota, speed is 120)",
            "OOP Basics",
            "Toyota is driving at 120 km/h",
            "Look at the drive() function output.",30);

        pool << new MCQQuestion("q01_11","cpp_01",
            "Can two objects from the same class have different values?","OOP Basics",
            {"No, they share all values",
             "Yes, each object has its own data",
             "Only if the class is abstract",
             "Only static members differ"},1,20);

        pool << new MCQQuestion("q01_12","cpp_01",
            "Which of these is NOT an OOP principle?","OOP Basics",
            {"Encapsulation","Inheritance","Compilation","Polymorphism"},2,20);
    }

    // ── LESSON 2: Encapsulation & Constructors ────────────────
    else if (id == "cpp_02") {
        pool << new MCQQuestion("q02_01","cpp_02",
            "What does Encapsulation mean?","Encapsulation",
            {"Making all variables public",
             "Hiding data and controlling access through methods",
             "Writing the same function multiple times",
             "Inheriting from another class"},1,20);

        pool << new MCQQuestion("q02_02","cpp_02",
            "Which access modifier hides members from outside the class?",
            "Encapsulation",
            {"public","protected","private","static"},2,20);

        pool << new MCQQuestion("q02_03","cpp_02",
            "What is a getter function used for?","Encapsulation",
            {"To set a private variable",
             "To delete an object",
             "To read a private variable safely",
             "To create a new object"},2,20);

        pool << new MCQQuestion("q02_04","cpp_02",
            "What is a setter function used for?","Encapsulation",
            {"To read a private variable",
             "To modify a private variable with validation",
             "To destroy an object",
             "To inherit from a parent class"},1,20);

        pool << new MCQQuestion("q02_05","cpp_02",
            "What is special about a Constructor?","Encapsulation",
            {"It must return an integer",
             "It has the same name as the class and no return type",
             "It can only be called once in a program",
             "It must always be private"},1,20);

        pool << new MCQQuestion("q02_06","cpp_02",
            "When does a constructor run?","Encapsulation",
            {"When you call it manually",
             "Only once when the program starts",
             "Automatically when an object is created",
             "When the program ends"},2,20);

        pool << new MCQQuestion("q02_07","cpp_02",
            "In class BankAccount, if 'balance' is private, which is valid from outside?",
            "Encapsulation",
            {"account.balance = 500;",
             "account.getBalance();",
             "account.balance++;",
             "cout << account.balance;"},1,20);

        pool << new MCQQuestion("q02_08","cpp_02",
            "Which is the correct constructor syntax for class Car?","Encapsulation",
            {"void Car() {}","int Car() {}",
             "Car() {}","create Car() {}"},2,20);

        pool << new WritingQuestion("q02_09","cpp_02",
            "If balance is private, can you write:\n"
            "myAccount.balance = 1000;  from outside the class?\nAnswer Yes or No:",
            "Encapsulation","No",
            "Private members can't be accessed from outside.",30);

        pool << new WritingQuestion("q02_10","cpp_02",
            "What is the output of:\n"
            "BankAccount acc(\"Ahmed\", 500);\n"
            "acc.deposit(200);\n"
            "cout << acc.getBalance();",
            "Encapsulation","700",
            "500 + 200 = 700",30);

        pool << new MCQQuestion("q02_11","cpp_02",
            "What happens if you don't define a constructor?","Encapsulation",
            {"Compile error",
             "C++ provides a default constructor automatically",
             "The object can't be created",
             "All members are set to null"},1,20);

        pool << new MCQQuestion("q02_12","cpp_02",
            "Which principle does using private + getters/setters represent?",
            "Encapsulation",
            {"Inheritance","Polymorphism","Abstraction","Encapsulation"},3,20);
    }

    // ── LESSON 3: Inheritance ─────────────────────────────────
    else if (id == "cpp_03") {
        pool << new MCQQuestion("q03_01","cpp_03",
            "What is Inheritance in OOP?","Inheritance",
            {"Creating two objects from one class",
             "A child class getting attributes and methods from a parent",
             "Hiding data inside a class",
             "Calling the same function with different parameters"},1,20);

        pool << new MCQQuestion("q03_02","cpp_03",
            "Correct syntax for Dog inheriting from Animal?","Inheritance",
            {"class Dog extends Animal","class Dog inherits Animal",
             "class Dog : public Animal","class Animal : public Dog"},2,20);

        pool << new MCQQuestion("q03_03","cpp_03",
            "What is another name for the parent class?","Inheritance",
            {"Derived class","Child class","Base class","Sub class"},2,20);

        pool << new MCQQuestion("q03_04","cpp_03",
            "What is another name for the child class?","Inheritance",
            {"Base class","Super class","Derived class","Abstract class"},2,20);

        pool << new MCQQuestion("q03_05","cpp_03",
            "In the lesson, which method does Dog inherit from Animal?",
            "Inheritance",
            {"bark()","meow()","eat()","fly()"},2,20);

        pool << new MCQQuestion("q03_06","cpp_03",
            "Which members does a child class inherit?","Inheritance",
            {"Only public members",
             "Public and protected members",
             "All members including private",
             "Only static members"},1,20);

        pool << new MCQQuestion("q03_07","cpp_03",
            "Can a child class add its OWN new methods?","Inheritance",
            {"No, only inherited methods are allowed",
             "Yes, child can add its own methods",
             "Only if the parent allows it",
             "Only static methods"},1,20);

        pool << new MCQQuestion("q03_08","cpp_03",
            "What access modifier is used in public inheritance?","Inheritance",
            {"private","protected","public","virtual"},2,20);

        pool << new WritingQuestion("q03_09","cpp_03",
            "What do we call the class that GIVES its methods?\nAnswer: Parent or Child?",
            "Inheritance","Parent","The giver is the Base/Parent class.",30);

        pool << new WritingQuestion("q03_10","cpp_03",
            "In the lesson:\nDog d; d.name = \"Rex\"; d.eat();\n"
            "What does eat() print?",
            "Inheritance","Rex is eating.",
            "eat() is inherited from Animal, uses the name member.",30);

        pool << new MCQQuestion("q03_11","cpp_03",
            "In class Cat : public Animal { }; which is true?","Inheritance",
            {"Cat has no access to Animal methods",
             "Cat can use eat() and sleep() from Animal",
             "Animal inherits from Cat",
             "Cat must redefine all Animal methods"},1,20);

        pool << new MCQQuestion("q03_12","cpp_03",
            "What keyword prevents a class from being inherited?","Inheritance",
            {"abstract","sealed","final","private"},2,20);
    }

    // ── LESSON 4: Polymorphism ────────────────────────────────
    else if (id == "cpp_04") {
        pool << new MCQQuestion("q04_01","cpp_04",
            "What does Polymorphism mean?","Polymorphism",
            {"One class, one method, one behaviour",
             "The same function name behaving differently in different classes",
             "Making all functions private",
             "Using constructors to create objects"},1,20);

        pool << new MCQQuestion("q04_02","cpp_04",
            "Which keyword allows a function to be overridden in a child?",
            "Polymorphism",
            {"abstract","override","virtual","static"},2,20);

        pool << new MCQQuestion("q04_03","cpp_04",
            "Which keyword does the child use to override a virtual function?",
            "Polymorphism",
            {"virtual","override","inherit","super"},1,20);

        pool << new MCQQuestion("q04_04","cpp_04",
            "What is the output of s->draw() when s = new Circle()?",
            "Polymorphism",
            {"Drawing a shape","Drawing a Circle",
             "Drawing a Rectangle","Compile error"},1,20);

        pool << new MCQQuestion("q04_05","cpp_04",
            "What type of polymorphism uses virtual functions?","Polymorphism",
            {"Compile-time polymorphism","Static polymorphism",
             "Runtime polymorphism","Template polymorphism"},2,20);

        pool << new MCQQuestion("q04_06","cpp_04",
            "What is function overloading?","Polymorphism",
            {"Same name, different parameters (compile-time)",
             "Same name, same parameters",
             "A virtual function in a child class",
             "Hiding a parent function"},0,20);

        pool << new MCQQuestion("q04_07","cpp_04",
            "In the lesson, what does Circle::area() return for radius=5?",
            "Polymorphism",
            {"25","50","78.5","15.7"},2,20);

        pool << new MCQQuestion("q04_08","cpp_04",
            "Can a parent pointer point to a child object?","Polymorphism",
            {"No, they are different types",
             "Yes, this enables runtime polymorphism",
             "Only if the child is abstract",
             "Only with casting"},1,20);

        pool << new WritingQuestion("q04_09","cpp_04",
            "What keyword in the PARENT enables a function to be overridden?\n(one word)",
            "Polymorphism","virtual","Hint: v_r_u_l",30);

        pool << new WritingQuestion("q04_10","cpp_04",
            "Shape* s = new Rectangle();\ns->draw();\nWhat is the output?",
            "Polymorphism","Drawing a Rectangle",
            "Virtual dispatch calls Rectangle version.",30);

        pool << new MCQQuestion("q04_11","cpp_04",
            "What does the 'override' keyword help with?","Polymorphism",
            {"Makes the function faster",
             "Confirms you are correctly overriding a virtual function",
             "Creates a new virtual function",
             "Hides the parent function"},1,20);

        pool << new MCQQuestion("q04_12","cpp_04",
            "Which principle does virtual function overriding demonstrate?",
            "Polymorphism",
            {"Encapsulation","Inheritance","Abstraction","Polymorphism"},3,20);
    }

    // ── LESSON 5: Abstraction ─────────────────────────────────
    else if (id == "cpp_05") {
        pool << new MCQQuestion("q05_01","cpp_05",
            "What is a Pure Virtual Function?","Abstraction",
            {"A function with no parameters",
             "Declared with = 0; child classes MUST implement it",
             "A function that returns void",
             "A static function"},1,20);

        pool << new MCQQuestion("q05_02","cpp_05",
            "Can you create an object from an Abstract Class?","Abstraction",
            {"Yes, always","Yes, with a constructor",
             "No, you cannot","Only if it has one pure virtual function"},2,20);

        pool << new MCQQuestion("q05_03","cpp_05",
            "Correct syntax for a pure virtual function?","Abstraction",
            {"virtual void start() {}","void start() = 0;",
             "virtual void start() = 0;","abstract void start();"},2,20);

        pool << new MCQQuestion("q05_04","cpp_05",
            "A class becomes abstract when it has...","Abstraction",
            {"Only private members",
             "At least one pure virtual function",
             "More than 5 methods",
             "A constructor with parameters"},1,20);

        pool << new MCQQuestion("q05_05","cpp_05",
            "In the lesson, what does Vehicle::showInfo() print for brand=Toyota?",
            "Abstraction",
            {"Toyota","Brand: Toyota | Max Speed: 200 km/h",
             "Vehicle started","Toyota car started."},1,20);

        pool << new MCQQuestion("q05_06","cpp_05",
            "What does Abstraction mean in OOP?","Abstraction",
            {"Copying code from another class",
             "Hiding details and showing only essential features",
             "Making all methods public",
             "Creating multiple objects from one class"},1,20);

        pool << new MCQQuestion("q05_07","cpp_05",
            "If Car inherits abstract Vehicle, what MUST Car do?","Abstraction",
            {"Implement all pure virtual functions",
             "Declare new pure virtual functions",
             "Override all public methods",
             "Add a destructor"},0,20);

        pool << new MCQQuestion("q05_08","cpp_05",
            "In the lesson, what is Bike's maxSpeed()?","Abstraction",
            {"200","150","120","100"},2,20);

        pool << new WritingQuestion("q05_09","cpp_05",
            "What keyword makes a function pure virtual?\n(complete: virtual void f() __;)",
            "Abstraction","= 0",
            "The syntax is:  virtual void f() = 0;",30);

        pool << new WritingQuestion("q05_10","cpp_05",
            "Can you write: Vehicle v; in main()?\nAnswer Yes or No.",
            "Abstraction","No",
            "Vehicle is abstract — cannot instantiate directly.",30);

        pool << new MCQQuestion("q05_11","cpp_05",
            "Real-life example of Abstraction?","Abstraction",
            {"A Car blueprint (class)",
             "Driving a car without knowing the engine internals",
             "Two cars with different colors",
             "A car inheriting from Vehicle"},1,20);

        pool << new MCQQuestion("q05_12","cpp_05",
            "Which principle forces child classes to implement a contract?",
            "Abstraction",
            {"Encapsulation","Inheritance","Polymorphism","Abstraction"},3,20);
    }

    // ── Shuffle and return 10 ─────────────────────────────────
    // Use Fisher-Yates shuffle with Qt's random generator
    for (int i = pool.size() - 1; i > 0; i--) {
        int j = (int)(QRandomGenerator::global()->generate() % (quint32)(i + 1));
        pool.swapItemsAt(i, j);
    }

    // Return first 10 (or all if less than 10)
    return pool.mid(0, qMin(10, pool.size()));
}
