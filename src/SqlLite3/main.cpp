#include <iostream>
#include <format>
#include <sqlite3.h> 

#pragma comment(lib, "sqlite3.lib")

/* 
* To build this sample, you need to install vcpkg and to install the x64 
* sqlite3 package using vcpkg with ".\vcpkg install sqlite3:x64-windows".
* Make sure you've integrate install to make VS can see the binaries.
*/
int main(int argc, char* argv[])
{
    // See the following resources:
    // https://www.tutorialspoint.com/sqlite/sqlite_c_cpp.htm
    // https://www.sqlite.org/cintro.html
    // https://www.geeksforgeeks.org/sql-using-c-c-and-sqlite/
    sqlite3* pDb;
    const int status = sqlite3_open("test.db", &pDb);
    if (status != SQLITE_OK)
    {
        std::cout << std::format("Sqlite failed to open database: {}\n", sqlite3_errmsg(pDb));
        return 1;
    }

    std::cout << "Database opened successfully.\n";
    sqlite3_close(pDb);

    return 0;
}
