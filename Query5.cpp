#include <iostream>
#include <sqlite3.h>

using namespace std;

int callback(void* NotUsed, int argc, char** argv, char** azColName) {
for (int i = 0; i < argc; i++) {
cout << azColName[i] << " = " << (argv[i] ? argv[i] : "NULL") << " | ";
}
cout << endl;
return 0;
}

int main() {
sqlite3* db;
char* errMsg = 0;

// Open database
int rc = sqlite3_open("tpch.db", &db);
if (rc) {
    cerr << "Can't open database: " << sqlite3_errmsg(db) << endl;
    return 1;
}
// SQL Query (TPC-H Q5)
const char* sql =
    "SELECT n.n_name, "
    "SUM(l.l_extendedprice * (1 - l.l_discount)) AS revenue "
    "FROM customer c "
    "JOIN orders o     ON c.c_custkey = o.o_custkey "
    "JOIN lineitem l   ON l.l_orderkey = o.o_orderkey "
    "JOIN supplier s   ON l.l_suppkey = s.s_suppkey "
    "JOIN nation n     ON s.s_nationkey = n.n_nationkey "
    "JOIN region r     ON n.n_regionkey = r.r_regionkey "
    "WHERE r.r_name = 'ASIA' "
    "AND o.o_orderdate >= '1994-01-01' "
    "AND o.o_orderdate <  '1995-01-01' "
    "GROUP BY n.n_name;";
cout << "Running query...\n\n";
rc = sqlite3_exec(db, sql, callback, 0, &errMsg);
if (rc != SQLITE_OK) {
    cerr << "SQL error: " << errMsg << endl;
    sqlite3_free(errMsg);
}
sqlite3_close(db);
return 0;
}
