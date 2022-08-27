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
            NULL, 0, NULL, 0) == NULL) {
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


int main() {
    //menuPrincipal();
	MYSQL* con = connectServer();
    mysql_query(con, "USE TEST"); 
    mysql_query(con, "CREATE TABLE prueba2 ( nombre VARCHAR(50) NOT NULL)");
}

