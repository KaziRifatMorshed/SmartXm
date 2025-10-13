#include <sqlcipher/sqlite3.h>
#include <cstring>
#include <iostream>

int main() {
    sqlite3 *db;
    int rc = sqlite3_open("encrypted.db", &db);
    if (rc) {
        std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
        return rc;
    }

    // Set the encryption key
    sqlite3_key(db, "A1b#cD3e", strlen("A1b#cD3e"));

    // Create a table
    const char *sql = "CREATE TABLE test(id INTEGER PRIMARY KEY, value TEXT);";
    char *errMsg = nullptr;
    rc = sqlite3_exec(db, sql, nullptr, nullptr, &errMsg);
    if (rc != SQLITE_OK) {
        std::cerr << "SQL error: " << errMsg << std::endl;
        sqlite3_free(errMsg);
    }

    sqlite3_close(db);
    return 0;
}
