#ifndef PERSON_HPP
#define PERSON_HPP

#include <iostream>
#include <string>


struct PersonID {
	int series;
	int number;

	PersonID(int s = 0, int n = 0) noexcept : series(s), number(n) {}

	bool operator<(const PersonID& other) const noexcept {
		if (series != other.series) {
			return series < other.series;
		}
		return number < other.number;
	}

	bool operator==(const PersonID& other) const noexcept {
		return series == other.series && number == other.number;
	}

	bool operator!=(const PersonID& other) const noexcept {
		return !(*this == other);
	}
};


class Student {
	private:
		PersonID id;
		std::string firstName;
		std::string lastName;
		double gpa;

	public:
		Student(PersonID personId = PersonID(0, 0),
		        const std::string& fName = "",
		        const std::string& lName = "",
		        double grade = 0.0)
		    : id(personId), firstName(fName), lastName(lName), gpa(grade) {}

		PersonID getID() const noexcept { return id; }
		std::string getFirstName() const { return firstName; }
		std::string getLastName() const { return lastName; }
		double getGPA() const noexcept { return gpa; }

		bool operator<(const Student& other) const noexcept {
			return id < other.id;
		}

		bool operator==(const Student& other) const noexcept {
			return id == other.id;
		}

		bool operator!=(const Student& other) const noexcept {
			return id != other.id;
		}

		friend std::ostream& operator<<(std::ostream& os, const Student& st) {
			os << "[" << st.id.series << "-" << st.id.number << "] "
			   << st.firstName << " " << st.lastName << " (GPA: " << st.gpa << ")";
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
		Teacher(PersonID personId = PersonID(0, 0),
		        const std::string& fName = "",
		        const std::string& lName = "",
		        const std::string& dept = "")
		    : id(personId), firstName(fName), lastName(lName), department(dept) {}

		PersonID getID() const noexcept { return id; }

		std::string getFirstName() const { return firstName; }
		std::string getLastName() const { return lastName; }
		std::string getDepartment() const { return department; }

		bool operator<(const Teacher& other) const noexcept {
			return id < other.id;
		}

		bool operator==(const Teacher& other) const noexcept {
			return id == other.id;
		}

		bool operator!=(const Teacher& other) const noexcept {
			return id != other.id;
		}

		friend std::ostream& operator<<(std::ostream& os, const Teacher& tc) {
			os << "[" << tc.id.series << "-" << tc.id.number << "] "
			   << tc.firstName << " " << tc.lastName << " (Dept: " << tc.department << ")";
			return os;
		}
};


#endif