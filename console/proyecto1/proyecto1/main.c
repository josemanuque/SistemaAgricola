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

struct employee {
    char* cedula;
    char* nombre;
    char* apellido1;
    char* apellido2;
    char* labor;
    float salario;
    float salarioCargas;
    int index;
};

struct sale {
    char* code;
    char* name;
    float price;
    float tax;
    int index;
    int cantidad;
    int aproved;
};

struct sale* listFact;
struct product* productsList;
struct areas* areasList;
struct employee* employeeList;



/*
Funcion que se conecta a la base de datos MYSQL y retorna la conexion
- No recibe entradas
- Salida: conexion a la base
- Restricciones: se verifica que la conexion a la base sea válida (ip, login, pwd, base)
*/
MYSQL* connectServer() {

    // Connect to MYSQL database
    MYSQL* con = mysql_init(NULL);

    // si la conexion es nula lanza error
    if (con == NULL) {
        fprintf(stderr, "%s\n", mysql_error(con));
        exit(1);
    }
    //se verifica la conexion a la base (ip, login, pwd, base) si es nula da error.
    if (mysql_real_connect(con, "localhost", "root", "Basad0.",
        "sag", 0, NULL, 0) == NULL) {
        fprintf(stderr, "%s\n", mysql_error(con));
        mysql_close(con);
        exit(1);
    }

    return con;
}


/*
Funcion que lee un string hasta que se presione enter y la retorna
-Entradas no hay
-Salida: string leído
*/
char* readString() {
    // alloca memoria dinamica para el string de tamaño desconocido
    char* string = (char*)malloc(sizeof(char) * 100);
    int i = 0;
    char c;
    // lee caracter mientras no haya un enter y los inserta en el string
    while ((c = getchar()) != '\n' && string != NULL) {
        string[i] = c;
        i++;
    }
    // al final del string se inserta \0 para indicar que es un string y es el final
    if (string != NULL)
        string[i] = '\0';
    return string;
}


/*
Funcion encargada de localizar un archivo y abrirlo
-Entrada: conexion a BD
-Salida: Ninguna, lee el archivo, llama a cargarProductos
*/
FILE* openFile(MYSQL* con) {
    FILE* fd;
    char* path;
    while (1) {
        printf("\n Inserte el path del archivo local: ");
        path = readString();
        fd = fopen(path, "r");
        if (fd == NULL) {
            printf(" No existe  \n");
        }
        else {
            printf("Encontrado \n");
            free(path);
            return fd;
        }
        free(path);
    }
}



// ------------------------- Structs Prints
//Function to display employees struct
void showEmployees() {
    for (int i = 0; i < employeeList[0].index; i++) {
        printf("%d. Cedula: %s, Nombre: %s, Apellido 1: %s, Apellido 2: %s, Labor: %s, Salario: %f, Cargas %f \n", (i + 1),
            employeeList[i].cedula, employeeList[i].nombre, employeeList[i].apellido1, employeeList[i].apellido2,
            employeeList[i].labor, employeeList[i].salario, employeeList[i].salarioCargas);
    }
}

//Function to display areas struct
void showAreas() {
    for (int i = 0; i < areasList[0].index; i++) {
        printf("Codigo: %s, Nombre: %s, Dimension: %f, Producto pricipal: %s \n",
            areasList[i].id, areasList[i].name, areasList[i].dimension, areasList[i].mainProduct);
    }
}

//Function to display product struct
//*************************************Modificada
void showProducts() {
    printf("Numero\tCodigo \t Nombre \t Precio \t I.V.A \n");

    for (int i = 0; i < productsList[0].index; i++) {
        printf("%d\t", i + 1);
        printf("%s\t", productsList[i].code);
        //printf("%s\t", productsList[i].name);
        printf("%f\t", productsList[i].price);
        printf("%f\t  \n", productsList[i].tax);
    }


}

// --------------------------------- Saves database tables in Structs  (Save)

//This function saves the employee data in a struct
void createEmployees(MYSQL_RES* res_ptr) {
    MYSQL_FIELD* headers;
    MYSQL_ROW* res_rows;

    unsigned int rows, columns;

    rows = mysql_num_rows(res_ptr);
    columns = mysql_num_fields(res_ptr);

    free(employeeList);
    employeeList = malloc(rows * sizeof(*employeeList));
    if (employeeList == NULL) {
        printf("Error al reservar memoria para la lista de empleados\n");
        exit(1);
    }
    employeeList[0].index = rows;

    while (headers = mysql_fetch_field(res_ptr)) {
        //printf("%10s \t", headers->name);
    }
    //printf("\n");
    for (int i = 0; i < rows; i++) {
        res_rows = mysql_fetch_row(res_ptr);

        for (int j = 0; j < columns; j++) {

            if (j == 0) {
                unsigned int n = sizeof(res_rows[j]);
                employeeList[i].cedula = malloc(n * sizeof(char*));
                strcpy((employeeList[i].cedula), res_rows[j]);
                //printf("%10s \t", employeeList[i].cedula);
            }
            else if (j == 1) {
                int n = sizeof(res_rows[j]);
                employeeList[i].nombre = (char*)malloc(n * sizeof(char*));
                strcpy(employeeList[i].nombre, res_rows[j]);
                //printf("%10s \t", employeeList[i].nombre);
            }
            else if (j == 2) {
                int n = sizeof(res_rows[j]);
                employeeList[i].apellido1 = (char*)malloc(n * sizeof(char*));
                strcpy(employeeList[i].apellido1, res_rows[j]);
                //printf("%10s \t", employeeList[i].apellido1);
            }
            else if (j == 3) {
                int n = sizeof(res_rows[j]);
                employeeList[i].apellido2 = (char*)malloc(n * sizeof(char*));
                strcpy(employeeList[i].apellido2, res_rows[j]);
                //printf("%10s \t", employeeList[i].apellido2);
            }
            else if (j == 4) {
                int n = sizeof(res_rows[j]);
                employeeList[i].labor = (char*)malloc(n * sizeof(char*));
                strcpy(employeeList[i].labor, res_rows[j]);
                //printf("%10s \t", employeeList[i].labor);
            }
            else if (j == 5) {
                employeeList[i].salario = atof(res_rows[j]);
                //printf("%10f \t", employeeList[i].salario);
            }
            else if (j == 6) {
                employeeList[i].salarioCargas = atof(res_rows[j]);
                //printf("%10f \t", employeeList[i].salarioCargas);
            }
        }
        //printf("\n");
    }
    //showEmployees();
}

//This function saves the area data in a struct
void createAreas(MYSQL_RES* res_ptr) {
    MYSQL_FIELD* headers;
    MYSQL_ROW* res_rows;

    unsigned int rows, columns;

    rows = mysql_num_rows(res_ptr);
    columns = mysql_num_fields(res_ptr);

    areasList = malloc(rows * sizeof(*areasList));
    if (areasList == NULL) {
        printf("Error al reservar memoria para la lista de areas\n");
        exit(1);
    }
    areasList[0].index = rows;

    while (headers = mysql_fetch_field(res_ptr)) {
        //printf("%10s \t", headers->name);
    }
    //printf("\n");
    for (int i = 0; i < rows; i++) {
        res_rows = mysql_fetch_row(res_ptr);

        for (int j = 0; j < columns; j++) {

            if (j == 0) {
                int n = sizeof(res_rows[j]);
                areasList[i].id = malloc(n * sizeof(char*));
                strcpy(areasList[i].id, res_rows[j]);
                //printf("%10s \t", areasList[i].id);
            }
            else if (j == 1) {
                int n = sizeof(res_rows[j]);
                areasList[i].name = malloc(n * sizeof(char*));
                strcpy(areasList[i].name, res_rows[j]);
                //printf("%10s \t", areasList[i].name);
            }
            else if (j == 2) {
                areasList[i].dimension = atof(res_rows[j]);
                //printf("%10f \t", areasList[i].dimension);
            }
            else if (j == 3) {
                int n = sizeof(res_rows[j]);
                areasList[i].mainProduct = malloc(n * sizeof(char*));
                strcpy(areasList[i].mainProduct, res_rows[j]);
                //printf("%10s \t", areasList[i].mainProduct);
            }


        }
        //printf("\n");



    }

    //showAreas();
}

//This function saves the product data in a struct
void createProducts(MYSQL_RES* res_ptr) {

    MYSQL_FIELD* headers;
    MYSQL_ROW* res_rows;

    unsigned int rows, columns;

    rows = mysql_num_rows(res_ptr);
    columns = mysql_num_fields(res_ptr);

    productsList = malloc(rows * sizeof(productsList));
    if (productsList == NULL) {
        printf("Error al reservar memoria para la lista de productos\n");
        exit(1);
    }
    productsList[0].index = rows;
    while (headers = mysql_fetch_field(res_ptr)) {
        //printf("%10s \t", headers->name);
    }
    //printf("\n");

    for (int i = 0; i < rows; i++) {

        res_rows = mysql_fetch_row(res_ptr);

        for (int j = 0; j < columns; j++)
        {
            if (j == 0) {
                int n = sizeof(res_rows[j]);
                productsList[i].code = (char*)malloc(n * sizeof(char*));
                strcpy(productsList[i].code, res_rows[j]);
                //printf("%10s \t", productsList[i].code);
            }
            else if (j == 1) {
                int n = sizeof(res_rows[j]);
                productsList[i].name = (char*)malloc(n * sizeof(char*));
                strcpy(productsList[i].name, res_rows[j]);
                //printf("%10s \t", productsList[i].name);
            }
            else if (j == 2) {
                productsList[i].price = atof(res_rows[j]);
                //printf("%10f \t", productsList[i].price);
            }
            else if (j == 3) {
                productsList[i].tax = atof(res_rows[j]);
                //printf("%10f \t", productsList[i].tax);
            }
        }
        //printf("\n");
    }
}







/*-------------------------- - Reading from txt file and saving to database------------------ */

//Function to insert products in SQL
void insertProducts(MYSQL* conn, char productCode[], char productName[], float productPrice, float productTax) {

    char sql_statement[2048];

    sprintf(sql_statement, "INSERT INTO  producto (id, nombre, costo, impuesto) VALUES('%s', '%s', %f,%f )", productCode, productName, productPrice, productTax);

    if (mysql_query(conn, sql_statement) != 0)
    {
        printf("Query failed  with the following message:\n");
        printf("%s\n", mysql_error(conn));
    }
}

// Function to empty temp char and reuse it without garbage.
void emptyTemp(char temp[]) {

    for (int i = 0; i < 10; i++) {
        temp[i] = '\0';
    }
}

// Reading from txt file and saving to database
void readProducts(MYSQL* con, FILE* products) {
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
    fclose(products);
}



/*
Funcion encargada de ejecutar queries del
- Entradas: Conexion a la BD, string con el nombre de la tabla.
- Salida: Ninguna. Imprime en consola todos los datos de la tabla
*/
void runQuery(MYSQL* conn, char* query, int op) {
    int error;
    MYSQL_RES* res_ptr; //Here is where the info is located

    

    //printf("\nQuery: %s\n", query);
    error = mysql_query(conn, query);
    if (!error) {

        res_ptr = mysql_store_result(conn); //Saving the result in the pointer
        if (res_ptr == NULL) {
            fprintf(stderr, "%s\n", mysql_error(conn));
            return;
        }
        if (res_ptr) {
            if (op == 1) {
                createProducts(res_ptr);
            }
            else if (op == 2) {
                createAreas(res_ptr);
            }
            else if (op == 3) {
                createEmployees(res_ptr);
            }
        }
        else {
            printf("Error al mostrar resultados...");
        }
    }
    else {
        printf("\n Error al ejecutar la consulta \n ");
        fprintf(stderr, "%s\n", mysql_error(conn));
    }
}


/*
Funcion encargada de pedir a la base una tabla e imprimirla
- Entradas: Conexion a la BD, string con el nombre de la tabla.
- Salida: retorna la cantidad de filas
*/

void printTable(MYSQL* con, char* table, char* columns) {
    char query[100];
    sprintf_s(query, sizeof(query), "SELECT %s FROM %s", columns, table);
    mysql_query(con, query);
    MYSQL_RES* res = mysql_store_result(con);
    if (res == NULL) {
        fprintf(stderr, "%s\n", mysql_error(con));
        return;
    }
    int num_fields = mysql_num_fields(res);
    int num_rows = mysql_num_rows(res);
    MYSQL_ROW row;
    MYSQL_FIELD* field;
    printf("\n");
    while ((row = mysql_fetch_row(res))) {
        for (int i = 0; i < num_fields; i++) {
            if (i == 0) {
                while (field = mysql_fetch_field(res)) {
                    printf("%10s \t", field->name);
                }
                printf("\n");
            }
            printf("%10s \t", row[i]);
        }

    }
    res = mysql_store_result(con);
    printf("\n");
    mysql_free_result(res);
}




void showAreas2() {

    printf("\nNumero\tId\tNombre \n");
    for (int i = 0; i < areasList[0].index; i++)
    {
        printf("%d\t", i + 1);
        printf("%s\t", areasList[i].id);
        printf("%s\t \n", areasList[i].name);
    }
}


void buy(int n);

void carrito();

void completePurchase(MYSQL* conn, char name[], int day[], int month, int year, char idArea[]) {
  
    char query[100] = " ";
    sprintf_s(query, sizeof(query), "call ingresaFactura('%s',%d,%d,%d,'%s');", name, day, month, year, idArea);
    runQuery(conn, query, 0);
    for (int i = 0; i < listFact[0].index; i++) {
        if (listFact[i].aproved == 1) {
            MYSQL* con = connectServer();
            sprintf_s(query, sizeof(query), "call ingresaProductoFactura('%s', %d);", listFact[i].code, listFact[i].cantidad);
            runQuery(conn, query, 0);
            mysql_close(con);

        }
    }
    //  printTable(conn, "Planilla", "*");
    //  break;

}


void invoice(MYSQL* conn) {
    char* name;
    int year;
    int month;
    int day;
    int narea;
    char* areaId;

    printf("\n************ Facturacion ************\n");
    printf("Ingrese el nombre del cliente: ");
    name = readString();
    do {
        printf("\n\n------------ Ingrese la fecha en formata de numeros ------------\n");
        printf("Ingrese el año: ");
        year = atoi(readString());

        printf("Ingrese el mes: ");
        month = atoi(readString());
        printf("Ingrese el dia: ");
        day = atoi(readString());

        if (year < 1000 || month>12 || day > 31) {
            printf("*Mensaje:\n\tDebes ingresar un formato valido para la fecha. \n\n");
        }
    } while (year < 1000 || month>12 || day > 31);

    printf("\n------------ Areas ------------\n");

    showAreas2();

    do {
        printf("\nSelecciona el area desde donde se factura: ");
        narea = atoi(readString());
        if (narea > areasList[0].index || narea <= 0) {
            printf("Mensaje:\n\tDebes ingresar un area de la lista. \n\n");
        }
        else {
            areaId = areasList[narea - 1].id;
            
            completePurchase(conn, name, day, month, year, areaId);
            
        }
    } while (narea > areasList[0].index || narea <= 0);



}


void deleteProduct(int del) {
    int op2;
    int amount;

    if (listFact[del - 1].cantidad > 1) {
        printf("1.Elimnar todos.\n");
        printf("2.Corregir cantidad\n");
        printf("Ingrese una opcion: ");

        op2 = atoi(readString());
        if (op2 == 1) {

            listFact[del - 1].cantidad = 0;
            listFact[del - 1].aproved = 0;
            printf("*Mensaje:\n\tProducto eliminado del carrito. \n\n");
        }
        else if (op2 == 2) {
            printf("Inserte la nueva cantidad: ");
            amount = atoi(readString());

            if (amount > 0) {
                listFact[del - 1].cantidad = amount;
                printf("*Mensaje:\n\tCantidad de productos modificada. \n\n");
            }
            else {
                printf("*Mensaje:\n\tProducto eliminado del carrito. \n\n");
            }
        }
    }
    else {
        listFact[del - 1].cantidad = 0;
        listFact[del - 1].aproved = 0;
        printf("*Mensaje:\n\tProducto eliminado del carrito. \n\n");
    }
}



void carrito(MYSQL* conn) {

    char* op;
    do {
        printf("\n************ Productos en el carrito ************\n");
        printf("\nNumero \t id \t Nombre \t Precio \t I.V.A  \t Cantidad \n");

        for (int i = 0; i < listFact[0].index; i++) {
            if (listFact[i].aproved == 1) {


                printf("%d\t", i + 1);
                printf("%s\t", listFact[i].code);
                printf("%s\t", listFact[i].name);
                printf("%f\t", listFact[i].price);
                printf("%f\t", listFact[i].tax);
                printf("%d\t\n", listFact[i].cantidad);
                // printf("%d\t\n", listFact[i].aproved);
            }
        }

        printf("\n1.Seleccionar otro producto. \n");
        printf("2.Elimar producto. \n");
        printf("3.Proceder a facturar. \n");
        printf("4.Volver.\n");

        printf("Ingrese una opcion: ");

        op = readString()[0];

        if (op == '1') {
            buy(conn, listFact[0].index);
        }

        else if (op == '2') {
            int del;
            printf("\n------------ Eliminar Producto ------------ \n");

            printf("Inserte el numero del producto que desea elimnar: ");
            del = atoi(readString());
            if (del > 0 && del <= listFact[0].index && listFact[del - 1].aproved == 1) {

                deleteProduct(del);
            }
            else {
                printf("\n*Mensaje:\n\tDebe seleccionar un producto del carrito. \n\n");
            }

        }

        else if (op == '3') {
            invoice(conn);
        }

    } while (op != '4');

}

int addCart(MYSQL* conn, int n, int num) {

    char* cmp = 'n';
    printf("Valor n: %d \n", n);
    if (n == 1) {
        listFact = malloc(n * sizeof(*listFact));

        listFact[n - 1].code = (char*)malloc(n * sizeof(char));
        listFact[n - 1].code = productsList[num - 1].code;

        listFact[n - 1].name = (char*)malloc(n * sizeof(char));
        listFact[n - 1].name = productsList[num - 1].name;

        listFact[n - 1].price = productsList[num - 1].price;
        listFact[n - 1].tax = productsList[num - 1].tax;
        listFact[n - 1].cantidad = 1;
        listFact[n - 1].aproved = 1;

        listFact[0].index = n;
        printf("\n*Mensaje:\n\tProducto %s agregado al carrito. \n\n", listFact[n - 1].name);
    }
    else {

        for (int i = 0; i < (n - 1); i++) {
            if (listFact[i].code == productsList[num - 1].code && listFact[i].aproved == 1) {

                cmp = 's';
                listFact[i].cantidad++;
                n--;
                printf("\n*Mensaje:\n\tProducto %s agregado al carrito. \n\n", listFact[i].name);

            }


        }

        if (cmp != 's') {

            listFact = (struct sale*)realloc(listFact, n * sizeof(*listFact));


            listFact[n - 1].code = (char*)malloc(n * sizeof(char));
            listFact[n - 1].code = productsList[num - 1].code;

            listFact[n - 1].name = (char*)malloc(n * sizeof(char));
            listFact[n - 1].name = productsList[num - 1].name;

            listFact[n - 1].price = productsList[num - 1].price;
            listFact[n - 1].tax = productsList[num - 1].price;
            listFact[n - 1].cantidad = 1;
            listFact[n - 1].aproved = 1;

            listFact[0].index = n;
            printf("\n*Mensaje:\n\tProducto %s agregado al carrito. \n\n", listFact[n - 1].name);
        }

    }
    return n;
}



void buy(MYSQL* conn, int n) {

    int num;
    char* op;

    do {

        fflush(stdin);


        printf("\n------------ Productos Disponibles ------------\n\n");
        showProducts();

        printf("\nIngrese el numero del producto que desea: ");
        num = atoi(readString());
        n++;

        if (num > 0 && num <= productsList[0].index) {
            n = addCart(conn, n, num);

        }
        else {
            n--;
            printf("\n*Mensaje:\n\tDebes seleccionar un producto de la lista. \n\n");
        }
        printf("\n------------ Productos ------------\n\n");
        printf("\n1.Seleccionar otro producto\n");
        printf("2.Ir al carrito\n");
        printf("3.Terminar Compra\n");
        printf("4.Volver\n\n");

        printf("Ingrese una opcion: ");


        op = readString()[0];

        if (op == '2') {

            listFact[0].index = n;
            carrito(conn);
            op = '4';
        }
        else if (op == '3') {
            invoice(conn);
        }


    } while (op != '4');

}


void makeSale(MYSQL* con) {
    int n = 1;


    listFact = (n * sizeof(*listFact));


    char first = { 0 };

    char op;

    do {

        fflush(stdin);
        printf("\n************ Ventas ************\n");
        printf("\n 1. Comprar producto \n");
        printf(" 2. Ver productos en el carrito. \n");
        printf(" 3. Concluir compra \n");
        printf(" 4. Salir \n");

        printf("Elige una opcion: ");
        op = readString()[0];

        if (op == '1') {
            buy(con, 0);

        }
        else if (op == '2') {

            printf("");
        }
        else if (op == '3') {
            invoice(con);
        }

    } while (op != '4');


}







/*
Funcion encargada del proceso de verificar usuario para las funciones de la aplicacion
- Entradas: Recibe la conexion a la base y el tipo de usuario (admin u operativo)
- Salida: No hay.
- Restricciones: Lee un string con usuario y contraseña, verifica y si no es correcta se vuelve a pedir.
*/
void login(MYSQL* con, int type) {
    char* user;
    char* password;

    MYSQL_RES* res;
    while (1) {
        // pide input del usuario para obtener usuario y contraseña
        printf("Ingrese usuario: ");
        user = readString();
        printf("Ingrese contrasena: ");
        password = readString();

        // manda query a MYSQL y se verifica que el resultado no sea nulo
        char query[100];
        sprintf_s(query, sizeof(query), "select * from users where username = '%s' and pwd = '%s' and tipo = %d", user, password, type);
        mysql_query(con, query);
        res = mysql_store_result(con);
        if (res == NULL) {
            fprintf(stderr, "%s\n", mysql_error(con));
            return;
        }
        // si coincide el usuario, contraseña y tipo, significa que exista una fila en la BD,
        // se sale del while y se sale de la funcion
        int num_rows = mysql_num_rows(res);
        if (num_rows == 1)
            break;
        printf("Usuario Invalido..\n");
        free(user);
        free(password);
    }
    free(user);
    free(password);
}



/*
Funcion encargada de pedir a la base una query e imprimir lo que retorna
- Entradas: Conexion a la BD, string con el nombre de la tabla.
- Salida: Ninguna. Imprime en consola todos los datos de la tabla
*/
void getDatabaseResult(MYSQL* conn, char* query) {
    MYSQL* con = connectServer();
    mysql_query(con, query);
    MYSQL_RES* res = mysql_store_result(con);
    if (res == NULL) {
        fprintf(stderr, "%s\n", mysql_error(con));
        return;
    }
    int num_fields = mysql_num_fields(res);
    MYSQL_ROW row;
    MYSQL_FIELD* field;
    printf("\n");
    while ((row = mysql_fetch_row(res))) {
        for (int i = 0; i < num_fields; i++) {
            if (i == 0) {
                while (field = mysql_fetch_field(res)) {
                    printf("%10s \t", field->name);
                }
                printf("\n");
            }
            printf("%10s \t", row[i]);
        }

    }
    printf("\n");
    mysql_close(con);
    mysql_free_result(res);
}

/*
Funcion encargada de mostrar en consola menu de registro de planilla
-Entradas: Conexion a la base de datos para mandar a funciones dentro de esta funcion
-Salida: Ninguna, imprime en consola el menu.
- Restriccion: Caracter pedido en opiones de menu debe estar en el rango de las opciones
*/
void menuPlanilla(MYSQL* conn) {
    printf("\n************ Registro de Planilla ************\n");
    printf("Ingrese el mes: ");
	char* month = readString();
	printf("Ingrese el ano: ");
	char* year = readString();
	
    //printTable(conn, "empleado");
    //showEmployees();
    int quantity = employeeList[0].index;
    int opInt = 0;

    // Crea arreglo para almacenar cedulas de empleados seleccionados
    char cedulas[200][10];
    // Para verificar si esta vacio, se le asigna un 0 al inicio del arreglo
    strcpy(cedulas[0], "NULL");
    // Contador de cantidad de elementos añadidos a lista empleados
    unsigned int counter = 0;
    char* stringOp;
    char op;
    do {;
        // Submenu
            showEmployees();
            printf("G. Guardar\n");
            printf("Q. Salir sin guardar\n");
            printf("Elige una opcion: ");
        
        // Lee opción
        stringOp = readString();
		op = stringOp[0];

        // Transforma el char elegido en un int que 
        opInt = atoi(stringOp) - 1;
		// Chequea que no este el mismo empleado en la lista.
        int flag = 0;
        // Verifica que la opcion elegida sea mayor a 0 y menor a la cantidad de empleados
        if (flag != 1 && opInt >= 0 && opInt < quantity) {
            // 
            for (int i = 0; i < quantity; i++) {
                if (cedulas[0] != NULL && strcmp(employeeList[opInt].cedula, cedulas[i]) == 0) {
                    flag = 1;
                    printf("\nEmpleado ya existe.\n\n");
                }
            }
            if (flag != 1) {
                strcpy(cedulas[counter], employeeList[opInt].cedula);
                cedulas[counter][9] = '\0';
                printf("\nSe ha anadido empleado: %s\n", cedulas[counter]);
                counter++;
            }
        }
        else if (op == 'G' || op == 'g') {
            printf("Inicio del array: %c\n", cedulas[0][0]);
            if (strcmp(cedulas[0], "NULL") == 0)
                printf("Debe incluir a un empleado.\n\n");

            else {
                printf("Ingrese el detalle de la planilla a generar: ");
                char* detalle = readString();
                printf("\n\n");
                char query[100] = " ";
                sprintf_s(query, sizeof(query), "call ingresaPlanilla(%s, %s, '%s');", month, year, detalle);
                runQuery(conn, query, 0);
                for (int i = 0; i < counter; i++) {
                    //printf("Cedula: %s\n", cedulas[i]);
                    sprintf_s(query, sizeof(query), "call ingresaPlanillaEmpleado('%s');", cedulas[i]);
                    runQuery(conn, query, 0);
                }
                printTable(conn, "Planilla", "*");
                break;
            }
        }
		else if (op == 'Q' || op == 'q') {
            break;
        }
		else if (flag == 1) {
			printf("El empleado ya esta en la lista.");
		}
        else {
            printf("\nOpcion no valida.\n");
        }
    } while (op != 'Q');
}


/*
Funcion encargada de mostrar en consola menu de opciones operativas
-Entradas: Conexion a la base de datos para mandar a funciones dentro de esta funcion
-Salida: Ninguna, imprime en consola el menu.
- Restriccion: Caracter pedido en opiones de menu debe estar en el rango de las opciones
*/
void operationsMenu(MYSQL* conn) {
    login(conn, 0);

    printf("\n************ Opciones Operativas ************\n");
    char op;
    do {

        printf("\n1. Cargar Productos \n");
        printf("2. Lista de Areas \n");
        printf("3. Lista de Empleados \n");
        printf("4. Salir \n");

        printf("Elige una opcion: ");
        op = readString()[0];

        if (op == '1') {
            FILE* fd = openFile(conn);
            readProducts(conn, fd);
            char* query = "SELECT* FROM producto";
            runQuery(conn, query, 1);

        }
        else if (op == '2') {
            printTable(conn, "area", "*");
            
        }
        else if (op == '3') {
            printTable(conn, "empleado", "*");
            /*char* query3 = "SELECT* FROM empleado";
            runQuery(conn, query3, 3);*/
        }
        else if (op == '4') {
            break;
        }
        else {
            printf("\nOpcion no valida.\n");
        }

    } while (op != '4');
}


/*
Funcion encargada de mostrar en consola menu de opciones administrativas
-Entradas: Conexion a la base de datos para mandar a funciones dentro de esta funcion
-Salida: Ninguna, imprime en consola el menu.
- Restriccion: Caracter pedido en opiones de menu debe estar en el rango de las opciones
*/
void adminMenu(MYSQL* conn) {

    login(conn, 1);
    printf("\n************ Opciones Administrativas ************\n");
    char op;
    do {
        printf("\n1. Valores Iniciales\n");
        printf("2. Registro Planilla\n");
        printf("3. Registro Ventas\n");
        printf("4. Consulta Planilla\n");
        printf("5. Consulta Ventas\n");
        printf("6. Balance Anual\n");
        printf("7. Salir \n");

        printf("Elige una opcion: ");
        op = readString()[0];

        if (op == '1') {
            printTable(conn, "infoLocal", "*");
        }
        else if (op == '2') {
            menuPlanilla(conn);
        }
        else if (op == '3') {
            makeSale(conn);
        }
        else if (op == '4') {
            printf("*** Planillas ***\n");
            printTable(conn, "planilla", "mes, ano, subtotal, total");
            int idPlanilla;
            while (1) {
                printf("\nIngrese una opcion: ");
                idPlanilla = atoi(readString());
                if (idPlanilla > 0)
                    break;
            }
            char query[100];
            sprintf_s(query, sizeof(query), "call verPlanillaElegida(%d);", idPlanilla);

            getDatabaseResult(conn, query);
            //mysql_close(con);
        }
        else if (op == '5') {
            
        }
        else if (op == '6') {
            printTable(conn, "balanceAnual", "*");
        }
        else {
            printf("\nOpcion no valida.\n");
        }

    } while (op != '7');
}

void mainMenu(MYSQL* conn) {
    //conn = connectServer();
    printf("****************** Sistema de Gestion de Agricola ******************\n");
    char op;
    do {

        printf("\n1. Opciones Operativas \n");
        printf("2. Opciones Administrativas \n");

        printf("Elige una opcion: ");
        op = readString()[0];

        if (op == '1') {
            operationsMenu(conn);
        }
        else if (op == '2') {
            adminMenu(conn);
        }

    } while (op != '4');

}


int main() {
    MYSQL* con = connectServer();

    char* query1 = "SELECT* FROM producto";
    runQuery(con, query1, 1);
	// Inserta areas y empleados en structs
    char* query2 = "SELECT* FROM area";
    runQuery(con, query2, 2);
    char* query3 = "SELECT* FROM empleado";
    runQuery(con, query3, 3);

	// Menu principal
    mainMenu(con);

}

