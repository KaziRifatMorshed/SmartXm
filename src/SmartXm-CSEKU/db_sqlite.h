#ifndef DB_SQLITE_H
#define DB_SQLITE_H

#include <QDebug>
#include <QString>
#include <QVariant>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>
#include <mutex>

class SQliteDB {
public:
  SQliteDB* _dbInstance = nullptr;

  // Get the singleton instance
  static SQliteDB &instance() {
    static SQliteDB instance_;
    return instance_;
  }

  // Delete copy and assignment
  SQliteDB(const SQliteDB &) = delete;
  SQliteDB &operator=(const SQliteDB &) = delete;

  // Open the database (encrypted with SQLCipher)
  bool openDB(const QString &dbPath = "./db/cache.sqlite",
              const QString &passphrase = "S3Jc>P(f*$.&E$!j+.c") {
    if (db.isOpen()) return true;

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

  // Get raw QSqlDatabase for advanced operations
  QSqlDatabase &database() { return db; }

private:
  SQliteDB() {}
  ~SQliteDB() { closeDB(); }

  QSqlDatabase db;
};

#endif // DB_SQLITE_H
