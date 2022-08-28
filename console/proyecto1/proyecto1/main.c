#include <mysql.h>
#include <stdio.h>
#include <stdlib.h>




//Function that reads a string until enter is pressed and returns it
char* readString() {
    char* string = (char*)malloc(sizeof(char) * 100);
    int i = 0;
    char c;
    while ((c = getchar()) != '\n' && string != NULL) {
        string[i] = c;
        i++;
    }
    if (string != NULL)
        string[i] = '\0';
    return string;
}


// Function that verifies login from strings
void login(MYSQL* con) {
    char* user;
    char* password;

    while (1) {
        printf("Ingrese usuario: ");
        user = readString();
        printf("Ingrese contrasena: ");
        password = readString();
		
        if (mysql_real_connect(con, "localhost", user, password,
            "sag", 0, NULL, 0) == NULL) {
            fprintf(stderr, "%s\n", mysql_error(con));
            mysql_close(con);
            con = mysql_init(NULL);
            printf("\nUsuario o contrasena es incorrecto.\n");
        }
        else
            break;
    }
    free(user);
	free(password);
}


// Function that connects to MYSQL database and returns connection

MYSQL* connectServer() {

    // Connect to MYSQL database
    MYSQL* con = mysql_init(NULL);

    if (con == NULL)
    {
        fprintf(stderr, "%s\n", mysql_error(con));
        exit(1);
    }

    login(con);

    return con;
}

void getTable(MYSQL* con, char* table) {
    char query[100];
    sprintf_s(query, sizeof(query), "SELECT * FROM %s", table);
    mysql_query(con, query);
    MYSQL_RES* res = mysql_store_result(con);
    if (res == NULL) {
        fprintf(stderr, "%s\n", mysql_error(con));
        return;
    }
    int num_fields = mysql_num_fields(res);
    MYSQL_ROW row;
    while ((row = mysql_fetch_row(res))) {
        for (int i = 0; i < num_fields; i++)
        {
            printf("%s ", row[i] ? row[i] : "NULL");
        }

        printf("\n");
    }
    mysql_free_result(res);
}

int main() {
    //menuPrincipal();
	MYSQL* con = connectServer();
    getTable(con, "verPlanillaElegida");
    //mysql_query(con, "CREATE TABLE prueba2 ( nombre VARCHAR(50) NOT NULL)");
}

