#include "Users.h"

#include <algorithm>

// Initialize static members
std::mutex Users::instance_mutex_;
Users *Users::instance_ = nullptr;

Users &Users::getInstance() {
  std::lock_guard<std::mutex> lock(instance_mutex_);
  if (instance_ == nullptr) {
    instance_ = new Users();
  }
  return *instance_;
}

Users::Users()
    : user_id_(0), name_(""), email_(""), password_(""),
      identity_(Identity::Unknown), id_(""), creation_time_(""),
      last_updated_at_("") {}

// Getters
int Users::getUserId() const { return user_id_; }
std::string Users::getName() const { return name_; }
std::string Users::getEmail() const { return email_; }
std::string Users::getPassword() const { return password_; }
Users::Identity Users::getIdentity() const { return identity_; }
std::string Users::getId() const { return id_; }
std::string Users::getCreationTime() const { return creation_time_; }
std::string Users::getLastUpdatedAt() const { return last_updated_at_; }

// Setters
void Users::setUserId(int userId) { user_id_ = userId; }
void Users::setName(const std::string &name) { name_ = name; }
void Users::setEmail(const std::string &email) { email_ = email; }
void Users::setPassword(const std::string &password) { password_ = password; }
void Users::setIdentity(Identity identity) { identity_ = identity; }
void Users::setIdentity(std::string i) {
  if (i == "teacher")
    identity_ = Identity::Teacher;
  else if (i == "student")
    identity_ = Identity::Student;
  else
    identity_ = Identity::Unknown;
}
void Users::setId(const std::string &id) { id_ = id; }
void Users::setCreationTime(const std::string &creationTime) {
  creation_time_ = creationTime;
}
void Users::setLastUpdatedAt(const std::string &lastUpdatedAt) {
  last_updated_at_ = lastUpdatedAt;
}

// Helpers for enum conversion
Users::Identity Users::identityFromString(const std::string &str) {
  std::string lower_str = str;
  std::transform(lower_str.begin(), lower_str.end(), lower_str.begin(),
                 ::tolower);
  if (lower_str == "teacher")
    return Identity::Teacher;
  if (lower_str == "student")
    return Identity::Student;
  return Identity::Unknown;
}

std::string Users::identityToString(Identity identity) {
  switch (identity) {
  case Identity::Teacher:
    return "teacher";
  case Identity::Student:
    return "student";
  default:
    return "unknown";
  }
}
