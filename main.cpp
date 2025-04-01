#include <iostream>
#include <vector>
#include <cmath>
#include <sstream>
#include <string>
#include <map>
#include <fstream>

class QuadraticEquation {
private:
    double a, b, c;

public:
    QuadraticEquation(double a, double b, double c) : a(a), b(b), c(c) {}

    std::vector<double> solve() const {
        std::vector<double> roots;
        if (a == 0) {
            if (b != 0) {
                roots.push_back(-c / b);
            }
            return roots;
        }
        double discriminant = b * b - 4 * a * c;
        if (discriminant > 0) {
            roots.push_back((-b + std::sqrt(discriminant)) / (2 * a));
            roots.push_back((-b - std::sqrt(discriminant)) / (2 * a));
        } else if (discriminant == 0) {
            roots.push_back(-b / (2 * a));
        }
        return roots;
    }

    bool isLinear() const {
        return a == 0 && b != 0;
    }

    std::string toString() const {
        std::ostringstream oss;
        oss << a << "x^2";
        if (b >= 0) oss << " + " << b << "x";
        else oss << " - " << -b << "x";
        if (c >= 0) oss << " + " << c;
        else oss << " - " << -c;
        oss << " = 0";
        return oss.str();
    }
};

class Student {
protected:
    std::string name;

public:
    Student(const std::string& name) : name(name) {}
    virtual std::vector<double> solve(const QuadraticEquation& equation) = 0;
    std::string getName() const {
        return name;
    }
    virtual ~Student() {}
};

class GoodStudent : public Student {
public:
    GoodStudent(const std::string& name) : Student(name) {}
    std::vector<double> solve(const QuadraticEquation& equation) override {
        return equation.solve();
    }
};

class AverageStudent : public Student {
public:
    AverageStudent(const std::string& name) : Student(name) {}
    std::vector<double> solve(const QuadraticEquation& equation) override {
        if (rand() % 2 == 0) {
            return equation.solve();
        } else {
            return {};
        }
    }
};

class BadStudent : public Student {
public:
    BadStudent(const std::string& name) : Student(name) {}
    std::vector<double> solve(const QuadraticEquation& equation) override {
        return {0};
    }
};

class Teacher {
private:
    struct Submission {
        QuadraticEquation equation;
        std::vector<double> studentSolution;
        std::string studentName;
    };

    std::vector<Submission> submissions;
    std::map<std::string, int> results;

public:
    void initializeStudent(const std::string& studentName) {
        results[studentName] = 0;
    }

    void addSubmission(const QuadraticEquation& equation, const std::vector<double>& solution, const std::string& studentName) {
        submissions.push_back({equation, solution, studentName});
    }

    void checkSubmissions() {
        for (const auto& submission : submissions) {
            std::vector<double> correctSolution = submission.equation.solve();
            if (correctSolution == submission.studentSolution) {
                results[submission.studentName]++;
            }
        }
        submissions.clear();
    }

    void publishResultsToFile(const std::string& filename) const {
        std::ofstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Ошибка: не удалось открыть файл для записи результатов.\n";
            return;
        }
        file << "Таблица успеваемости:\n";
        for (const auto& [name, score] : results) {
            file << name << ": " << score << " успешно решенных задач\n";
        }
        file.close();
        std::cout << "Результаты успешно записаны в файл: " << filename << "\n";
    }

    void publishResultsToConsole() const {
        std::cout << "Таблица успеваемости:\n";
        for (const auto& [name, score] : results) {
            std::cout << name << ": " << score << " успешно решенных задач\n";
        }
    }
};

int main() {
    srand(static_cast<unsigned>(time(0)));

    std::vector<Student*> students = {
        new GoodStudent("Микаил Абубаширов"),
        new AverageStudent("Мария Балабулова"),
        new BadStudent("Артем Шишь"),
        new GoodStudent("Соснысеева Мария"),
        new AverageStudent("Глеб Неферов"),
        new BadStudent("Никита Лунин"),
        new GoodStudent("Александр Ждуков"),
        new AverageStudent("Артем Лугашев"),
        new BadStudent("Леонид Покрасов"),
        new GoodStudent("Роман Сенцов")
    };

    Teacher teacher;

    for (const auto& student : students) {
        teacher.initializeStudent(student->getName());
    }

    std::ifstream inputFile("../input");
    if (!inputFile.is_open()) {
        std::cerr << "не удалось открыть файл\n";
        return 1;
    }

    std::vector<QuadraticEquation> equations;
    double a, b, c;
    while (inputFile >> a >> b >> c) {
        equations.emplace_back(a, b, c);
    }
    inputFile.close();

    for (const auto& equation : equations) {
        for (auto* student : students) {
            std::vector<double> solution = student->solve(equation);
            teacher.addSubmission(equation, solution, student->getName());
        }
    }

    teacher.checkSubmissions();
    teacher.publishResultsToConsole();
    teacher.publishResultsToFile("results.txt");

    for (auto* student : students) {
        delete student;
    }

    return 0;
}