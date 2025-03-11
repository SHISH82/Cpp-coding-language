#include <iostream>
#include <fstream>
#include <cmath>
#include <sstream>

bool Coef(const std::string& line, double& a, double& b, double& c) {
    std::istringstream iss(line);
    iss >> a >> b >> c;
    return !iss.fail();
}

double Discriminant(double a, double b, double c) {
    return b * b - 4 * a * c;
}

void Roots(double a, double b, double c, double& r1, double& r2, bool& hasRoots) {
    double d = Discriminant(a, b, c);
    if (d < 0) {
        hasRoots = false;
    } else {
        hasRoots = true;
        r1 = (-b + sqrt(d)) / (2 * a);
        r2 = (-b - sqrt(d)) / (2 * a);
    }
}

void Result(std::ofstream& file, double a, double b, double c, double r1, double r2, bool hasRoots) {
    file << "Коэффициенты: a = " << a << ", b = " << b << ", c = " << c << std::endl;
    if (!hasRoots) {
        file << "Действительных корней нет" << std::endl;
    } else {
        file << "Корень 1: " << r1 << std::endl;
        file << "Корень 2: " << r2 << std::endl;
    }
    file << "-------------------------" << std::endl;
}

int main() {
    std::ifstream in("input.txt");
    if (!in.is_open()) {
        std::cerr << "Ошибка: Не удалось открыть input.txt" << std::endl;
        return 1;
    }

    std::ofstream out("output.txt");
    if (!out.is_open()) {
        std::cerr << "Ошибка: Не удалось открыть output.txt" << std::endl;
        return 1;
    }

    std::string line;
    int lineNum = 0;

    while (std::getline(in, line)) {
        lineNum++;
        double a, b, c;

        if (line.empty()) {
            out << "Строка " << lineNum << ": Пустая строка" << std::endl;
            out << "-------------------------" << std::endl;
            continue;
        }

        if (!Coef(line, a, b, c)) {
            out << "Строка " << lineNum << ": Некорректные данные" << std::endl;
            out << "-------------------------" << std::endl;
            continue;
        }

        if (a == 0) {
            out << "Строка " << lineNum << ": Коэффициент a не может быть нулем" << std::endl;
            out << "-------------------------" << std::endl;
            continue;
        }

        double r1, r2;
        bool hasRoots;
        Roots(a, b, c, r1, r2, hasRoots);
        Result(out, a, b, c, r1, r2, hasRoots);
    }

    in.close();
    out.close();

    std::cout << "Результаты записаны в output.txt" << std::endl;

    return 0;
}
