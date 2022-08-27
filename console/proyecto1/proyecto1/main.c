#include <mysql.h>
#include <stdio.h>
#include <stdlib.h>

// Function that connects to MYSQL database and returns connection
MYSQL* connectServer() {
    // Connect to MYSQL database
    MYSQL* con = mysql_init(NULL);

    if (con == NULL)
    {
        fprintf(stderr, "%s\n", mysql_error(con));
        exit(1);
    }

    if (mysql_real_connect(con, "localhost", "root", "Basad0.",
        NULL, 0, NULL, 0) == NULL)
    {
        fprintf(stderr, "%s\n", mysql_error(con));
        mysql_close(con);
        exit(1);
    }
    return con;
}

//Function that reads a string until enter is pressed and returns it
char* readString(){
    char* string = (char*)malloc(sizeof(char)*100);
    int i = 0;
    char c;
    while((c = getchar()) != '\n' && string != NULL){
        string[i] = c;
        i++;
    }
	if (string != NULL)
        string[i] = '\0';
    return string;
}

int main() {
    printf("%s",readString());
    //menuPrincipal();
	MYSQL* con = connectServer();
    mysql_query(con, "USE TEST"); 
    //mysql_query(con, "CREATE TABLE prueba2 ( nombre VARCHAR(50) NOT NULL)");
}

