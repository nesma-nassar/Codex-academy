#include "Lesson.h"
#include <QJsonArray>

Lesson::Lesson(const QString& id, const QString& title,
               const QString& topic, int xpReward,
               const QString& prerequisiteId)
    : m_id(id), m_title(title), m_topic(topic),
      m_prerequisiteId(prerequisiteId), m_xpReward(xpReward)
{}

QJsonObject Lesson::toJson() const {
    QJsonObject o;
    o["id"]             = m_id;
    o["title"]          = m_title;
    o["topic"]          = m_topic;
    o["description"]    = m_description;
    o["videoPath"]      = m_videoPath;
    o["prerequisiteId"] = m_prerequisiteId;
    o["xpReward"]       = m_xpReward;
    o["order"]          = m_order;
    return o;
}

Lesson Lesson::fromJson(const QJsonObject& o) {
    Lesson l(o["id"].toString(), o["title"].toString(),
             o["topic"].toString(), o["xpReward"].toInt(50),
             o["prerequisiteId"].toString());
    l.setDescription(o["description"].toString());
    l.setVideoPath(o["videoPath"].toString());
    l.setOrder(o["order"].toInt());
    return l;
}

// ════════════════════════════════════════════════════════════
//  5 Complete C++ / OOP Lessons
// ════════════════════════════════════════════════════════════
QVector<Lesson> Lesson::defaultCppLessons() {
    QVector<Lesson> lessons;

    // ── Lesson 1: Intro to C++ & OOP ─────────────────────────
    {
        Lesson l("cpp_01",
                 "Lesson 1: Introduction to C++ & OOP",
                 "OOP Basics", 50);
        l.setOrder(1);
        l.setVideoPath("videos/cpp_01.mp4");
        l.setDescription(
            "Introduction to C++, what is OOP, "
            "and the difference between Class and Object.");

        l.addContent(new TextContent(
            "What is C++?\n"
            "C++ is a powerful general-purpose programming language created by Bjarne Stroustrup in 1979.\n"
            "It supports Object-Oriented Programming (OOP), which helps us write organised, reusable code.\n\n"
            "What is OOP?\n"
            "OOP (Object-Oriented Programming) is a way of designing programs using 'objects'.\n"
            "It has 4 main principles:\n"
            "  1. Encapsulation\n"
            "  2. Inheritance\n"
            "  3. Polymorphism\n"
            "  4. Abstraction\n\n"
            "Class vs Object:\n"
            "A CLASS is a blueprint (like a car design on paper).\n"
            "An OBJECT is a real instance made from that class (like an actual car).\n"
            "Example: 'Car' is the class. Your red Toyota is an object of class Car."
        ));

        l.addContent(new CodeContent(
            "#include <iostream>\n"
            "using namespace std;\n\n"
            "// Class = blueprint\n"
            "class Car {\n"
            "public:\n"
            "    string brand;\n"
            "    string color;\n"
            "    int    speed;\n\n"
            "    void drive() {\n"
            "        cout << brand << \" is driving at \" << speed << \" km/h\" << endl;\n"
            "    }\n"
            "};\n\n"
            "int main() {\n"
            "    // Object = real instance\n"
            "    Car myCar;           // create object\n"
            "    myCar.brand = \"Toyota\";\n"
            "    myCar.color = \"Red\";\n"
            "    myCar.speed = 120;\n"
            "    myCar.drive();       // output: Toyota is driving at 120 km/h\n\n"
            "    Car friendCar;       // another object, same class\n"
            "    friendCar.brand = \"BMW\";\n"
            "    friendCar.speed = 200;\n"
            "    friendCar.drive();\n"
            "    return 0;\n"
            "}"
        ));
        lessons.append(l);
    }

    // ── Lesson 2: Encapsulation & Constructors ────────────────
    {
        Lesson l("cpp_02",
                 "Lesson 2: Encapsulation & Constructors",
                 "Encapsulation", 60, "cpp_01");
        l.setOrder(2);
        l.setVideoPath("videos/cpp_02.mp4");
        l.setDescription(
            "Learn how to protect data using private members, "
            "getters/setters, and how constructors work.");

        l.addContent(new TextContent(
            "What is Encapsulation?\n"
            "Encapsulation means hiding the internal data of a class and only allowing\n"
            "access through controlled methods (getters and setters).\n"
            "This protects data from being changed accidentally.\n\n"
            "Access Modifiers:\n"
            "  public:  accessible from anywhere\n"
            "  private: only accessible inside the class\n"
            "  protected: accessible inside class and its children\n\n"
            "What is a Constructor?\n"
            "A constructor is a special function that runs automatically when an object is created.\n"
            "It has the SAME NAME as the class and NO return type.\n"
            "It is used to set initial values for object attributes."
        ));

        l.addContent(new CodeContent(
            "#include <iostream>\n"
            "using namespace std;\n\n"
            "class BankAccount {\n"
            "private:\n"
            "    string owner;   // hidden from outside\n"
            "    double balance; // hidden from outside\n\n"
            "public:\n"
            "    // Constructor: runs automatically on creation\n"
            "    BankAccount(string name, double amount) {\n"
            "        owner   = name;\n"
            "        balance = amount;\n"
            "    }\n\n"
            "    // Getter: read balance\n"
            "    double getBalance() {\n"
            "        return balance;\n"
            "    }\n\n"
            "    // Setter: deposit money (with validation)\n"
            "    void deposit(double amount) {\n"
            "        if (amount > 0)\n"
            "            balance += amount;\n"
            "    }\n\n"
            "    void display() {\n"
            "        cout << owner << \" has $\" << balance << endl;\n"
            "    }\n"
            "};\n\n"
            "int main() {\n"
            "    BankAccount acc(\"Ahmed\", 500.0); // constructor called\n"
            "    acc.display();          // Ahmed has $500\n"
            "    acc.deposit(200);       // add money via setter\n"
            "    acc.display();          // Ahmed has $700\n"
            "    // acc.balance = 9999; // ERROR! balance is private\n"
            "    return 0;\n"
            "}"
        ));
        lessons.append(l);
    }

    // ── Lesson 3: Inheritance ─────────────────────────────────
    {
        Lesson l("cpp_03",
                 "Lesson 3: Inheritance",
                 "Inheritance", 70, "cpp_02");
        l.setOrder(3);
        l.setVideoPath("videos/cpp_03.mp4");
        l.setDescription(
            "Learn how a child class inherits properties and methods "
            "from a parent class to reuse code efficiently.");

        l.addContent(new TextContent(
            "What is Inheritance?\n"
            "Inheritance allows a class (child) to inherit attributes and methods\n"
            "from another class (parent). This avoids code repetition.\n\n"
            "Syntax:\n"
            "  class Child : public Parent { ... };\n\n"
            "Types of Inheritance:\n"
            "  public:    inherited members keep their access level\n"
            "  private:   all inherited members become private\n"
            "  protected: all inherited members become protected\n\n"
            "Key Terms:\n"
            "  Base class  = Parent class\n"
            "  Derived class = Child class\n"
            "The child gets all public/protected members of the parent\n"
            "AND can add its own new members."
        ));

        l.addContent(new CodeContent(
            "#include <iostream>\n"
            "using namespace std;\n\n"
            "// Parent (Base) class\n"
            "class Animal {\n"
            "public:\n"
            "    string name;\n"
            "    int    age;\n\n"
            "    void eat() {\n"
            "        cout << name << \" is eating.\" << endl;\n"
            "    }\n"
            "    void sleep() {\n"
            "        cout << name << \" is sleeping.\" << endl;\n"
            "    }\n"
            "};\n\n"
            "// Child class inherits from Animal\n"
            "class Dog : public Animal {\n"
            "public:\n"
            "    string breed;\n\n"
            "    void bark() {\n"
            "        cout << name << \" says: Woof!\" << endl;\n"
            "    }\n"
            "};\n\n"
            "// Another child class\n"
            "class Cat : public Animal {\n"
            "public:\n"
            "    void meow() {\n"
            "        cout << name << \" says: Meow!\" << endl;\n"
            "    }\n"
            "};\n\n"
            "int main() {\n"
            "    Dog d;\n"
            "    d.name  = \"Rex\";     // inherited from Animal\n"
            "    d.breed = \"Labrador\"; // Dog's own attribute\n"
            "    d.eat();   // inherited method\n"
            "    d.bark();  // Dog's own method\n\n"
            "    Cat c;\n"
            "    c.name = \"Mimi\";\n"
            "    c.eat();\n"
            "    c.meow();\n"
            "    return 0;\n"
            "}"
        ));
        lessons.append(l);
    }

    // ── Lesson 4: Polymorphism ────────────────────────────────
    {
        Lesson l("cpp_04",
                 "Lesson 4: Polymorphism",
                 "Polymorphism", 80, "cpp_03");
        l.setOrder(4);
        l.setVideoPath("videos/cpp_04.mp4");
        l.setDescription(
            "Learn how the same function can behave differently "
            "in different classes using virtual functions.");

        l.addContent(new TextContent(
            "What is Polymorphism?\n"
            "Polymorphism means 'many forms'. It allows the same function name\n"
            "to behave differently depending on which object calls it.\n\n"
            "Two Types:\n"
            "  1. Compile-time (Function Overloading): same name, different parameters\n"
            "  2. Runtime (Virtual Functions): parent pointer calls child method\n\n"
            "Virtual Functions:\n"
            "Use the 'virtual' keyword in the parent class.\n"
            "The child class 'overrides' the function with its own version.\n"
            "This lets you write general code that works for ALL child classes.\n\n"
            "override keyword:\n"
            "Use 'override' in the child to make sure you're overriding correctly."
        ));

        l.addContent(new CodeContent(
            "#include <iostream>\n"
            "using namespace std;\n\n"
            "class Shape {\n"
            "public:\n"
            "    string color;\n\n"
            "    // virtual = can be overridden by child\n"
            "    virtual void draw() {\n"
            "        cout << \"Drawing a shape\" << endl;\n"
            "    }\n\n"
            "    virtual double area() {\n"
            "        return 0;\n"
            "    }\n"
            "};\n\n"
            "class Circle : public Shape {\n"
            "public:\n"
            "    double radius;\n\n"
            "    void draw() override {\n"
            "        cout << \"Drawing a Circle\" << endl;\n"
            "    }\n"
            "    double area() override {\n"
            "        return 3.14 * radius * radius;\n"
            "    }\n"
            "};\n\n"
            "class Rectangle : public Shape {\n"
            "public:\n"
            "    double width, height;\n\n"
            "    void draw() override {\n"
            "        cout << \"Drawing a Rectangle\" << endl;\n"
            "    }\n"
            "    double area() override {\n"
            "        return width * height;\n"
            "    }\n"
            "};\n\n"
            "int main() {\n"
            "    // Polymorphism: same pointer type, different behaviour\n"
            "    Shape* s1 = new Circle();\n"
            "    Shape* s2 = new Rectangle();\n\n"
            "    s1->draw(); // Drawing a Circle\n"
            "    s2->draw(); // Drawing a Rectangle\n\n"
            "    Circle* c = (Circle*)s1;\n"
            "    c->radius = 5;\n"
            "    cout << \"Area = \" << c->area() << endl; // 78.5\n\n"
            "    delete s1;\n"
            "    delete s2;\n"
            "    return 0;\n"
            "}"
        ));
        lessons.append(l);
    }

    // ── Lesson 5: Abstraction ─────────────────────────────────
    {
        Lesson l("cpp_05",
                 "Lesson 5: Abstraction",
                 "Abstraction", 100, "cpp_04");
        l.setOrder(5);
        l.setVideoPath("videos/cpp_05.mp4");
        l.setDescription(
            "Learn abstract classes and pure virtual functions "
            "to define interfaces that child classes must implement.");

        l.addContent(new TextContent(
            "What is Abstraction?\n"
            "Abstraction means hiding complex implementation details and showing\n"
            "only the essential features to the user.\n\n"
            "Abstract Class:\n"
            "A class that has at least ONE pure virtual function.\n"
            "You CANNOT create objects from an abstract class directly.\n"
            "It acts as a contract — child classes MUST implement all pure virtual functions.\n\n"
            "Pure Virtual Function syntax:\n"
            "  virtual void functionName() = 0;\n\n"
            "Why use Abstraction?\n"
            "- Forces all child classes to follow the same interface\n"
            "- Hides unnecessary details from the user\n"
            "- Makes code more organised and maintainable\n\n"
            "Real-life example:\n"
            "A 'Vehicle' is abstract — you can't buy a generic vehicle.\n"
            "You buy a Car or a Bike (concrete classes)."
        ));

        l.addContent(new CodeContent(
            "#include <iostream>\n"
            "using namespace std;\n\n"
            "// Abstract class (has pure virtual function)\n"
            "class Vehicle {\n"
            "public:\n"
            "    string brand;\n\n"
            "    // Pure virtual = must be implemented by child\n"
            "    virtual void start()   = 0;\n"
            "    virtual void stop()    = 0;\n"
            "    virtual int  maxSpeed() = 0;\n\n"
            "    // Regular method (not pure)\n"
            "    void showInfo() {\n"
            "        cout << \"Brand: \" << brand;\n"
            "        cout << \" | Max Speed: \" << maxSpeed() << \" km/h\" << endl;\n"
            "    }\n"
            "};\n\n"
            "class Car : public Vehicle {\n"
            "public:\n"
            "    void start()   override { cout << brand << \" car started.\" << endl; }\n"
            "    void stop()    override { cout << brand << \" car stopped.\" << endl; }\n"
            "    int  maxSpeed() override { return 200; }\n"
            "};\n\n"
            "class Bike : public Vehicle {\n"
            "public:\n"
            "    void start()   override { cout << brand << \" bike started.\" << endl; }\n"
            "    void stop()    override { cout << brand << \" bike stopped.\" << endl; }\n"
            "    int  maxSpeed() override { return 120; }\n"
            "};\n\n"
            "int main() {\n"
            "    // Vehicle v; // ERROR! Cannot create abstract class object\n\n"
            "    Car c;\n"
            "    c.brand = \"Toyota\";\n"
            "    c.start();\n"
            "    c.showInfo();\n"
            "    c.stop();\n\n"
            "    Bike b;\n"
            "    b.brand = \"Honda\";\n"
            "    b.start();\n"
            "    b.showInfo();\n"
            "    return 0;\n"
            "}"
        ));
        lessons.append(l);
    }

    return lessons;
}
