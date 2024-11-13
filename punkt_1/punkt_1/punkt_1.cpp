#include <iostream>
#include <windows.h>
#include <sql.h>
#include <sqlext.h>
#include <stdio.h>

using namespace std;

void showError(SQLHANDLE handle, SQLSMALLINT handleType) {
    SQLWCHAR sqlState[6], message[256];
    SQLINTEGER nativeError;
    SQLSMALLINT textLength;
    SQLGetDiagRec(handleType, handle, 1, sqlState, &nativeError, message, sizeof(message), &textLength);
    wcerr << L"SQLState: " << sqlState << L", Error: " << message << endl;
}

int main() {
    setlocale(LC_ALL, "rus");

    printf("Content-Type: text/html\r\n\r\n");

    fflush(stdout);

    printf("<html>\n");
    printf("<head>\n");
    printf("<title>Таблица авторов</title>\n");
    printf("<style>\n");
    printf("body { color: blue; font-family: Arial, sans-serif; }\n");
    printf("table { width: auto; border-collapse: collapse; background-color: gold; margin: auto; }\n");
    printf("th, td { padding: 5px; border: 1px solid black; }\n");
    printf("h1 { text-align: center; }\n");
    printf("</style>\n");
    printf("</head>\n");
    printf("<body { text-align: center; }>\n");
    printf("<h1>Таблица Авторов</h1><table border='1'><tr>\n");
    printf("<th>Идентификатор</th><th>Фамилия</th><th>Имя</th><th>Телефон</th><th>Зарплата</th></tr>\n");

    SQLHENV hEnv;
    SQLHDBC hDbc;
    SQLHSTMT hStmt;
    SQLRETURN ret;

    if (SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &hEnv) != SQL_SUCCESS) {
        cerr << "Ошибка при создании ODBC окружения." << endl;
        return 1;
    }

    SQLSetEnvAttr(hEnv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, 0);

    if (SQLAllocHandle(SQL_HANDLE_DBC, hEnv, &hDbc) != SQL_SUCCESS) {
        cerr << "Ошибка при создании соединения." << endl;
        SQLFreeHandle(SQL_HANDLE_ENV, hEnv);
        return 1;
    }

    //const WCHAR* szConn = L"DSN=Authors_ODBC_s"; 
    const WCHAR* szConn = L"Driver={Microsoft Access Driver (*.mdb); DBQ=C:\\DB\\Lab3\\pubs.mdb}";
    ret = SQLDriverConnect(hDbc, NULL, (SQLWCHAR*)szConn, SQL_NTS, NULL, 0, NULL, SQL_DRIVER_NOPROMPT);

    if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
        cerr << "Ошибка подключения к базе данных." << endl;
        showError(hDbc, SQL_HANDLE_DBC);
        SQLFreeHandle(SQL_HANDLE_DBC, hDbc);
        SQLFreeHandle(SQL_HANDLE_ENV, hEnv);
        return 1;
    }

    if (SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt) != SQL_SUCCESS) {
        cerr << "Ошибка при создании оператора." << endl;
        SQLDisconnect(hDbc);
        SQLFreeHandle(SQL_HANDLE_DBC, hDbc);
        SQLFreeHandle(SQL_HANDLE_ENV, hEnv);
        return 1;
    }

    SQLWCHAR cmdstr[] = L"SELECT TOP 20 Au_id, Au_lname, Au_fname, Au_phone, Au_salary FROM Authors ORDER BY Au_id";
    ret = SQLExecDirect(hStmt, cmdstr, SQL_NTS);

    if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
        cerr << "Не удалось выполнить запрос." << endl;
        showError(hStmt, SQL_HANDLE_STMT);
    }
    else {
        while ((ret = SQLFetch(hStmt)) != SQL_NO_DATA) {
            char au_id[50];
            char au_lname[50], au_fname[50], au_phone[20];
            double au_salary;

            SQLGetData(hStmt, 1, SQL_C_CHAR, au_id, sizeof(au_id), NULL);
            SQLGetData(hStmt, 2, SQL_C_CHAR, au_lname, sizeof(au_lname), NULL);
            SQLGetData(hStmt, 3, SQL_C_CHAR, au_fname, sizeof(au_fname), NULL);
            SQLGetData(hStmt, 4, SQL_C_CHAR, au_phone, sizeof(au_phone), NULL);
            SQLGetData(hStmt, 5, SQL_C_DOUBLE, &au_salary, sizeof(au_salary), NULL);

            printf("<tr>");
            printf("<td align='center'>%s</td>", au_id);
            printf("<td align='left'>%s</td>", au_lname);
            printf("<td align='left'>%s</td>", au_fname);
            printf("<td align='center'>%s</td>", au_phone);
            printf("<td align='right'>%.1f</td>", au_salary);
            printf("</tr>\n");

            fflush(stdout);
        }
    }

    SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
    SQLDisconnect(hDbc);
    SQLFreeHandle(SQL_HANDLE_DBC, hDbc);
    SQLFreeHandle(SQL_HANDLE_ENV, hEnv);

    printf("</table>\n");
    printf("</body>\n");
    printf("</html>\n");

    return 0;
}
