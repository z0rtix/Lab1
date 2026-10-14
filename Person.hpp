#ifndef PERSON_HPP
#define PERSON_HPP

#include <string>
#include <ostream>


struct PersonID {
    int series;
    int number;

    bool operator<(const PersonID& other) const noexcept {
        if (series != other.series) {
            return series < other.series;
        }
        return number < other.number;
    }

    bool operator==(const PersonID& other) const noexcept {
        return series == other.series && number == other.number;
    }
};

class Student {
    private:
        PersonID id;
        std::string firstName;
        std::string lastName;
        double gpa;

    public:
        Student() : id{0, 0}, firstName(""), lastName(""), gpa(0.0) {}

        Student(PersonID personId, std::string first, std::string last, double studentGpa)
            : id(personId), firstName(std::move(first)), lastName(std::move(last)), gpa(studentGpa) {}

        PersonID getID() const noexcept {
            return id;
        }

        const std::string& getFirstName() const noexcept {
            return firstName;
        }

        const std::string& getLastName() const noexcept {
            return lastName;
        }

        double getGPA() const noexcept {
            return gpa;
        }

        bool operator<(const Student& other) const noexcept {
            return id < other.id;
        }

        friend std::ostream& operator<<(std::ostream& os, const Student& s) {
            os << "[" << s.id.series << "-" << s.id.number << "] " 
            << s.lastName << " " << s.firstName << " (GPA: " << s.gpa << ")";
            return os;
        }
};

class Teacher {
    private:
        PersonID id;
        std::string firstName;
        std::string lastName;
        std::string department;

    public:
        Teacher() : id{0, 0}, firstName(""), lastName(""), department("") {}

        Teacher(PersonID personId, std::string first, std::string last, std::string dept)
            : id(personId), firstName(std::move(first)), lastName(std::move(last)), department(std::move(dept)) {}

        PersonID getID() const noexcept {
            return id;
        }

        bool operator<(const Teacher& other) const noexcept {
            return id < other.id;
        }

        friend std::ostream& operator<<(std::ostream& os, const Teacher& t) {
            os << "[" << t.id.series << "-" << t.id.number << "] " 
            << t.lastName << " " << t.firstName << " (" << t.department << ")";
            return os;
        }
};


#endif