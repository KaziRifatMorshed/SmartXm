#ifndef USERS_H
#define USERS_H

#pragma once

#include <mutex>
#include <string>

class Users {
public:
  // Identity type
  enum class Identity { Teacher, Student, Unknown };

  // Get the singleton instance
  static Users &getInstance();

  // Getters
  int getUserId() const;
  std::string getName() const;
  std::string getEmail() const;
  std::string getPassword() const;
  Identity getIdentity() const;
  std::string getId() const;
  std::string getCreationTime() const;
  std::string getLastUpdatedAt() const;

  // Setters
  void setUserId(int userId);
  void setName(const std::string &name);
  void setEmail(const std::string &email);
  void setPassword(const std::string &password);
  void setIdentity(std::string identity);
  void setIdentity(Identity identity);
  void setId(const std::string &id);
  void setCreationTime(const std::string &creationTime);
  void setLastUpdatedAt(const std::string &lastUpdatedAt);

  // Helpers for enum conversion
  static Identity identityFromString(const std::string &str);
  static std::string identityToString(Identity identity);

  // Prevent copy and assignment
  Users(const Users &) = delete;
  Users &operator=(const Users &) = delete;

private:
  // Private constructor for singleton
  Users();

  // Data members
  int user_id_;
  std::string name_;
  std::string email_;
  std::string password_;
  Identity identity_;
  std::string id_;
  std::string creation_time_;
  std::string last_updated_at_;

  static std::mutex instance_mutex_;
  static Users *instance_;
};

#endif // USERS_H
