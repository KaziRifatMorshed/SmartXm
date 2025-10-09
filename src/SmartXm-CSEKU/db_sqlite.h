#ifndef DB_SQLITE_H
#define DB_SQLITE_H

#include <QDateTime>
#include <QDebug>
#include <QString>
#include <QVariant>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>

class SQliteDB {
public:
  static SQliteDB *cacheDbInstance;

  // Get the singleton instance
  static SQliteDB *instance() {
    if (cacheDbInstance) {
          qDebug() << "new instance sqlite db created";
      cacheDbInstance = new SQliteDB();
    }
    return cacheDbInstance;
  }

  // Delete copy and assignment
  SQliteDB(const SQliteDB &) = delete;
  SQliteDB &operator=(const SQliteDB &) = delete;

  // Open the database (encrypted with SQLCipher)
  bool openDB(const QString &dbPath = "./cache.sqlite",
              const QString &passphrase = "S3Jc>P(f*$.&E$!j+.c") {
    if (db.isOpen())
      return true;

    qDebug() << "opening db sqlite...";

    db = QSqlDatabase::addDatabase("QSQLITE", "cache_connection");
    db.setDatabaseName(dbPath);

    if (!db.open()) {
      qCritical() << "Failed to open SQLite DB:" << db.lastError().text();
      return false;
    }

    // Set SQLCipher key
    QSqlQuery pragmaQuery(db);
    if (!pragmaQuery.exec(QString("PRAGMA key = '%1';").arg(passphrase))) {
      qCritical() << "Failed to set SQLCipher key:"
                  << pragmaQuery.lastError().text();
      db.close();
      return false;
    }

    // Optionally set SQLCipher4 defaults for compatibility
    pragmaQuery.exec("PRAGMA cipher_compatibility = 4;");

    return true;
  }

  // Execute a query and return QSqlQuery object
  QSqlQuery execQuery(const QString &queryStr) {
    QSqlQuery query(db);
    if (!query.exec(queryStr)) {
      qCritical() << "Query failed:" << queryStr
                  << "Error:" << query.lastError().text();
    }
    return query;
  }

  // Check if DB is open
  bool isOpen() const { return db.isOpen(); }

  // Close the database connection
  void closeDB() {
    if (db.isOpen())
      db.close();
  }

  // Insert login cache entry
  bool insertLoginCache(int user_id, const std::string &identity,
                        const std::string &id, const std::string &user_mail,
                        const std::string &last_login_time) {
    qDebug() << "inserting Login Cache ";
    QSqlQuery insertionQuery(db);
    insertionQuery.prepare("INSERT INTO login_cache (user_id, identity, id, "
                           "user_mail, last_login_time) "
                           "VALUES (?, ?, ?, ?, ?);");
    insertionQuery.addBindValue(user_id);
    insertionQuery.addBindValue(QString::fromStdString(identity));
    insertionQuery.addBindValue(QString::fromStdString(id));
    insertionQuery.addBindValue(QString::fromStdString(user_mail));
    insertionQuery.addBindValue(QString::fromStdString(last_login_time));

    if (!insertionQuery.exec()) {
      qCritical() << "Failed to insert into sqlite cache:"
                  << insertionQuery.lastError().text();
      return false;
    }
    return true;
  }

  /*
   * if last login was within 3 hours from current time, return true, else false
   */
  bool checkLastLogin() {
    QSqlQuery query(db);
    // Assuming there's only one user or you have a way to get the current
    // user's id Replace 'user_id' with the actual user id if needed
    // query.prepare("SELECT last_login_time FROM login_cache WHERE user_id =
    // ?"); query.addBindValue(currentUserId); // Replace with your variable for
    // user id
    query.prepare("SELECT last_login_time FROM login_cache ORDER BY "
                  "last_login_time DESC LIMIT 1");

    if (!query.exec() || !query.next()) {
      qCritical() << "Failed to fetch last_login_time:"
                  << query.lastError().text();
      return false;
    }

    QString lastLoginStr = query.value(0).toString();
    QDateTime lastLogin = QDateTime::fromString(lastLoginStr, Qt::ISODate);
    QDateTime now = QDateTime::currentDateTime();

    if (!lastLogin.isValid()) {
      qCritical() << "Invalid last_login_time format:" << lastLoginStr;
      return false;
    }

    qint64 secondsDiff = lastLogin.secsTo(now);
    return (secondsDiff <= (3 * 3600)); // 3 hours in seconds
  }

  // Get raw QSqlDatabase for advanced operations
  QSqlDatabase &database() { return db; }

private:
  SQliteDB() { openDB(); }
  ~SQliteDB() { closeDB(); }

  QSqlDatabase db;
};

#endif // DB_SQLITE_H
