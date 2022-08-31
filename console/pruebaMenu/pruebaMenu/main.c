#define _CRT_SECURE_NO_DEPRECATE
#include <mysql.h>
#include <stdio.h>
#include <stdlib.h>


//Structures of our data.

struct product {
    char* code;
    char* name;
    float price;
    float tax;
    int index;
};

struct areas {
    char* id;
    char* name;
    float dimension;
    char* mainProduct;
    int index;
};

struct product* productsList;
struct areas* areaslist;

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


//Function to display areas data
void showAreas() {
    for (int i = 0; i < areaslist[0].index; i++)
    {
        printf("Codigo: %s, Nombre: %s, Precio: %f, Tax: %f \n",
            areaslist[0].id, areaslist[0].name, areaslist[0].mainProduct, areaslist[0].dimension);
    }
}


//Function to display product data
void showProducts() {
    for (int i = 0; i < productsList[0].index; i++)
    {
        printf("Codigo: %s, Nombre: %s, Precio: %f, Tax: %f \n",
            productsList[0].code, productsList[0].name, productsList[0].price, productsList[0].tax);
    }

}



//This function saves the area data in a struct
void createAreas(MYSQL_RES* res_ptr) {
    MYSQL_FIELD* headers;
    MYSQL_ROW res_rows;

    int rows, columns;

    rows = mysql_num_rows(res_ptr);
    columns = mysql_num_fields(res_ptr);

    areaslist = malloc(rows * sizeof * areaslist);
    areaslist[0].index = rows;

    while (headers = mysql_fetch_field(res_ptr)) {
        printf("%10s \t", headers->name);
    }
    printf("\n");

    for (int i = 1; i <= rows; i++) {

        res_rows = mysql_fetch_row(res_ptr);

        for (int j = 0; j < columns; j++)
        {

            if (j == 0) {
                int n = sizeof(res_rows[j]);
                areaslist[i - 1].id = (char*)malloc(n * sizeof(char));
                strcpy(areaslist[i - 1].id, res_rows[j]);
                printf("%10s \t", areaslist[i - 1].id);
            }
            else if (j == 1) {
                int n = sizeof(res_rows[j]);
                areaslist[i - 1].name = (char*)malloc(n * sizeof(char));
                strcpy(areaslist[i - 1].name, res_rows[j]);
                printf("%10s \t", areaslist[i - 1].name);
            }
            else if (j == 2) {
                areaslist[i - 1].dimension = atof(res_rows[j]);
                printf("%10f \t", areaslist[i - 1].dimension);
            }
            else if (j == 3) {
                int n = sizeof(res_rows[j]);
                areaslist[i - 1].mainProduct = (char*)malloc(n * sizeof(char));
                strcpy(areaslist[i - 1].mainProduct, res_rows[j]);
                printf("%10s \t", areaslist[i - 1].mainProduct);
            }


        }
        printf("\n");



    }

    showAreas();
}



//This function saves the product data in a struct
void createProducts(MYSQL_RES* res_ptr) {

    MYSQL_FIELD* headers;
    MYSQL_ROW res_rows;

    int rows, columns;

    rows = mysql_num_rows(res_ptr);
    columns = mysql_num_fields(res_ptr);

    productsList = malloc(rows * sizeof * productsList);
    productsList[0].index = rows;

    while (headers = mysql_fetch_field(res_ptr)) {
        printf("%10s \t", headers->name);
    }
    printf("\n");

    for (int i = 1; i <= rows; i++) {

        res_rows = mysql_fetch_row(res_ptr);

        for (int j = 0; j < columns; j++)
        {
            if (j == 0) {
                int n = sizeof(res_rows[j]);
                productsList[i - 1].code = (char*)malloc(n * sizeof(char));
                strcpy(productsList[i - 1].code, res_rows[j]);
                printf("%10s \t", productsList[i - 1].code);
            }
            else if (j == 1) {
                int n = sizeof(res_rows[j]);
                productsList[i - 1].name = (char*)malloc(n * sizeof(char));
                strcpy(productsList[i - 1].name, res_rows[j]);
                printf("%10s \t", productsList[i - 1].name);
            }
            else if (j == 2) {
                productsList[i - 1].price = atof(res_rows[j]);
                printf("%10f \t", productsList[i - 1].price);
            }
            else if (j == 3) {
                productsList[i - 1].tax = atof(res_rows[j]);
                printf("%10f \t", productsList[i - 1].tax);
            }
        }
        printf("\n");
    }

    //showProducts();

}


//Function to execute queries
void runQuery(MYSQL* conn, char* query, int op) {

    int error;
    MYSQL_RES* res_ptr; //Here is where the info is located

    error = mysql_query(conn, query);

    if (!error) {

        res_ptr = mysql_store_result(conn); //Saving the result in the pointer

        if (res_ptr) {
            if (op == 1) {
                createProducts(res_ptr);
            }
            else if (op == 2) {
                createAreas(res_ptr);
            }
        }
        else {
            printf("Error al mostrar resultados...");
        }
    }
    else {
        printf("\n Error al ejecutar la consulta \n ");
    }

}





//void finish_with_error(MYSQL* con)
//{
//    fprintf(stderr, "%s\n", mysql_error(con));
//    mysql_close(con);
//    exit(1);
//}












/*-------------------------- - Reading from txt file and saving to database------------------ */

//Function to insert products in SQL
void insertProducts(MYSQL* conn, char productCode[], char productName[], float productPrice, float productTax) {

    char sql_statement[2048];

    sprintf(sql_statement, "INSERT INTO  productos (id, nombre, costo, impuesto) VALUES('%s', '%s', %f,%f )", productCode, productName, productPrice, productTax);

    if (mysql_query(conn, sql_statement) != 0)
    {
        printf("Query failed  with the following message:\n");
        printf("%s\n", mysql_error(conn));
        exit(1);
    }



}

// Function to empty tempand reuse it without garbage.
void emptyTemp(char temp[]) {

    for (int i = 0; i < 10; i++)
    {
        temp[i] = '\0';
    }
}


// Reading from txt file and saving to database
void loadProducts(MYSQL* con, FILE* products) {
    char temp[10];
    char* productCode = "";
    char* productName = "";
    float productPrice;
    float productTax;
    int k = 0;
    int product = 1;

    for (int i = 0; !feof(products); i++) {

        emptyTemp(temp);
        char aux = '0';

        for (int j = 0; aux != ","; j++) {

            aux = fgetc(products);

            if (aux == ',' && product == 1) {
                temp[k] = '\0';
                int n = strlen(temp) + 1;
                productCode = (char*)malloc(n * sizeof(char));
                strcpy(productCode, temp);
                emptyTemp(temp);
                k = 0;
                product++;
            }
            else if (aux == ',' && product == 2) {
                temp[k] = '\0';
                int n = strlen(temp) + 1;
                productName = (char*)malloc(n * sizeof(char));
                strcpy(productName, temp);
                emptyTemp(temp);
                k = 0;
                product++;
            }
            else if (aux == ',' && product == 3) {
                temp[k] = '\0';
                productPrice = atof(temp);
                emptyTemp(temp);
                k = 0;
                product++;
            }
            else if (aux == '\n' || feof(products) && product == 4) {
                temp[k] = '\0';
                productTax = atof(temp);
                emptyTemp(temp);
                product = 1;
                k = 0;
                break;
            }
            if (aux != ',' && aux != '\n') {
                temp[k] = aux;
                k++;
            }
        }
        printf("Codigo: %s, Nombre: %s, Precio: %f, Tax: %f \n",
            productCode, productName, productPrice, productTax);

        insertProducts(con, productCode, productName, productPrice, productTax);
    }

}

// Modify the path while inserting so that it is read properly by C.
char* readPaht() {

    char* string = (char*)malloc(sizeof(char) * 100);
    int i = 0;
    char c;
    while ((c = getchar()) != '\n' && string != NULL) {
        string[i] = c;
        i++;

        if (c == 92) {
            string[i] = c;
            i++;

        }

    }
    if (string != NULL)
        string[i] = '\0';
    return string;
}


//This function is responsible for locating and opening the file.
void readFile(MYSQL* con) {

    fflush(stdin);
    FILE* fd;
    char* path;
    printf("\n Inserte el path del archivo local: ");
    path = readPaht();
    fd = fopen(path, "r");
    if (fd == NULL) {
        printf(" No existe  \n");
    }
    else {
        printf("Encontrado \n");
        loadProducts(con, fd);
        fclose(fd);
    }


}







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


void mainMenu(MYSQL* conn) {

    char op;

    do {

        fflush(stdin);
        printf("\n1. Cargar Productos \n");
        printf(" 2. Lista de Areas \n");
        printf(" 3. Lista de empleados \n");
        printf(" 4. Salir \n");

        printf("Elige una opcion: ");
        scanf("%c", &op);

        if (op == '1') {

            fflush(stdin);
            readFile(conn);
            char* query = "SELECT* FROM productos";
            runQuery(conn, query, 1);

        }
        else if (op == '2') {

            printf("areas..\n");

            char* query2 = "SELECT* FROM areas";
            runQuery(conn, query2, 2);
        }
        else if (op == '3') {
            printf("opcion");
        }

    } while (op != '4');

}

int main() {

    MYSQL* con = connectServer();
    mysql_query(con, "USE TEST");

    //readFile(con);

  //  printf("%s",readString());
    mainMenu(con);


    // char* query = "SELECT* FROM productos";
    // runQuery(con, query,1);

   // mainMenu(con);

}