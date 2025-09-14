#ifndef STUDENT_H
#define STUDENT_H

#include <string>
#include <sstream> // You'll need this header

class Student {
private:
    int id;
    std::string name;
    std::string email;
    std::string password; // This should be a hashed password in a real application
    std::string studentId;
    std::string role;
    std::string createdAt;

public:
    // Constructor
    Student(int id, const std::string& name, const std::string& email,
            const std::string& password, const std::string& studentId,
            const std::string& role, const std::string& createdAt)
        : id(id), name(name), email(email), password(password),
        studentId(studentId), role(role), createdAt(createdAt) {}

    // Getters
    int getId() const { return id; }
    std::string getName() const { return name; }
    std::string getEmail() const { return email; }
    std::string getPassword() const { return password; }
    std::string getStudentId() const { return studentId; }
    std::string getRole() const { return role; }
    std::string getCreatedAt() const { return createdAt; }

    // Setters
    void setName(const std::string& newName) { name = newName; }
    void setEmail(const std::string& newEmail) { email = newEmail; }
    void setPassword(const std::string& newPassword) { password = newPassword; }
    void setStudentId(const std::string& newStudentId) { studentId = newStudentId; }
    void setRole(const std::string& newRole) { role = newRole; }

    // toString method
    std::string toString() const {
        std::stringstream ss; // notun janlam
        ss << "Student{"
           << "id=" << id
           << ", name='" << name << '\''
           << ", email='" << email << '\''
           << ", student_id='" << studentId << '\''
           << ", role='" << role << '\''
           << ", created_at='" << createdAt << '\''
           << '}';
        return ss.str();
    }
};

#endif // STUDENT_H
