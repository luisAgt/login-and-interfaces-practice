/**
 * @file login_and_interface_system.c
 * @author Agustin Luis, Hernandez Omar, Sanchez Cristopher, Olmedo Alexandra
 * @date 2025
 * @details
 *Sistema de autenticación de usuarios mediante inicios de sesión y registros de usuarios
 *
 *Caracteristicas principales:
 *- Resgitro de usuarios mediante correo electronico
 *- Autenticación de datos mediante método de cifrado
 *- Recuperación de contraseñas mediante uso de correo electronico registrado
 *- Base de datos de usuarios en arhivos de texto
 *
 * @version 2.0
 **/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
#include <crypt.h>
#include <ctype.h>

// Prototipos
/**
 * @brief Muestra el menú principal del sistema
 */
void menuPrincipal();
void registrarUsuario();
void iniciarSesion();
void olvideContrasena();
void cifrarContrasena(const char *original, char *cifrada);
int compararUsuario(const char *correo, const char *passCifrada);
int validarCorreo(const char *correo);
int validarContrasena(const char *pass);
int correoDuplicado(const char *correo);

int main()
{
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);

    menuPrincipal();

    endwin();
    return 0;
}

/**
 * @brief Registra un nuevo usuario en el sistema
 * @details Solicita el nombre, correo y contraseña, valida los datos
 * y se almacenan en el archivo con la contraseña cifrada
 **/
void menuPrincipal()
{
    const char *opciones[] = {
        "Registrar usuario",
        "Iniciar sesion",
        "Olvide mi contrasena",
        "Salir"};
    int nOpciones = 4;
    int seleccion = 0;
    int ch;

    while (1)
    {
        clear();
        mvprintw(2, 10, "=== SISTEMA DE INICIO DE SESION ===");

        for (int i = 0; i < nOpciones; i++)
        {
            if (i == seleccion)
            {
                attron(A_REVERSE);
                mvprintw(4 + i, 12, "%s", opciones[i]);
                attroff(A_REVERSE);
            }
            else
            {
                mvprintw(4 + i, 12, "%s", opciones[i]);
            }
        }

        ch = getch();
        switch (ch)
        {
        case KEY_UP:
            seleccion--;
            if (seleccion < 0)
                seleccion = nOpciones - 1;
            break;
        case KEY_DOWN:
            seleccion++;
            if (seleccion >= nOpciones)
                seleccion = 0;
            break;
        case 10: // Enter
            switch (seleccion)
            {
            case 0:
                registrarUsuario(); // llamada a ventana de registro de usuario
                break;
            case 1:
                iniciarSesion(); // llamada a ventana de login
                break;
            case 2:
                olvideContrasena(); // llamada a forget password
                break;
            case 3:
                clear();
                mvprintw(10, 10, "Saliendo del sistema...");
                refresh();
                getch();
                return;
            }
            break;
        }
    }
}

/**
 * @brief Valida el formato del correo
 *
 * @param correo Cadena de texto
 * @return 1 si es valido, 0 si es invalido
 */
int validarCorreo(const char *correo)
{
    const char *at = strchr(correo, '@');
    if (!at)
        return 0;
    const char *punto = strchr(at, '.');
    if (!punto)
        return 0;
    return 1;
}

/**
 * @brief Valida que una contraseña sea segura
 *
 * @param pass cadena de caracteres a validar
 * @return 1 si cumple los requisitos, 0 si no los cumple
 *
 * Requrimientos:
 * - Mínimo 8 caracteres
 * - Al menos una letra mayúscula
 * - Al menos un número
 * - Al menos un carácter especial
 *
 * @see registrarUsuario
 * @see olvideContraseña
 */
int validarContrasena(const char *pass) int len = strlen(pass);
if (len < 8)
    return 0;

int tieneMayus = 0, tieneNumero = 0, tieneEspecial = 0;
for (int i = 0; i < len; i++)
{
    if (isupper(pass[i]))
        tieneMayus = 1;
    else if (isdigit(pass[i]))
        tieneNumero = 1;
    else if (!isalnum(pass[i]))
        tieneEspecial = 1;
}
return (tieneMayus && tieneNumero && tieneEspecial);
}

int correoDuplicado(const char *correo)
{
    FILE *archivo = fopen("usuarios.txt", "r");
    if (!archivo)
        return 0;

    char nombre[50], correoGuardado[50], pass[120];
    while (fscanf(archivo, "%s %s %s", nombre, correoGuardado, pass) == 3)
    {
        if (strcmp(correo, correoGuardado) == 0)
        {
            fclose(archivo);
            return 1; // duplicado encontrado
        }
    }
    fclose(archivo);
    return 0; // no hay duplicado
}

/**
 * @brief Cifra una contraseña usando el algoritmo DES
 *
 * @param original contraseña original
 * @param cifrada cadena donde se almacenará la contraseña cifrada
 */
void cifrarContrasena(const char *original, char *cifrada)
{
    char *hash = crypt(original, "xx");
    strcpy(cifrada, hash);
}

/**
 * @brief Ventana de registro de usuario
 *
 */
void registrarUsuario()
{
    char nombre[50];
    char correo[50];
    char contrasena[50];
    char passCifrada[120];

    clear();
    mvprintw(3, 10, "=== REGISTRO DE USUARIO ===");

    // Nombre
    while (1)
    {
        mvprintw(5, 10, "Nombre: ");
        echo();
        getnstr(nombre, 49);
        noecho();
        if (strlen(nombre) == 0)
        {
            mvprintw(6, 10, "Nombre no puede estar vacio.");
            clrtoeol();
            refresh();
            getch();
            clear();
            mvprintw(3, 10, "=== REGISTRO DE USUARIO ===");
        }
        else
            break;
    }

    // Correo
    while (1)
    {
        mvprintw(6, 10, "Correo: ");
        echo();
        getnstr(correo, 49);
        noecho();
        if (!validarCorreo(correo))
        {
            mvprintw(7, 10, "Correo invalido. Debe contener '@' y '.'");
            clrtoeol();
            refresh();
            getch();
            clear();
            mvprintw(3, 10, "=== REGISTRO DE USUARIO ===");
        }
        else if (correoDuplicado(correo))
        {
            mvprintw(7, 10, "Correo ya registrado. Intente otro.");
            clrtoeol();
            refresh();
            getch();
            clear();
            mvprintw(3, 10, "=== REGISTRO DE USUARIO ===");
        }
        else
            break;
    }

    // Contraseña
    while (1)
    {
        mvprintw(7, 10, "Contrasena: ");
        noecho();
        getnstr(contrasena, 49);
        if (!validarContrasena(contrasena))
        {
            mvprintw(8, 10, "Contrasena invalida. Min 8 chars, 1 mayus, 1 numero, 1 especial.");
            clrtoeol();
            refresh();
            getch();
            mvprintw(7, 10, "                                ");
        }
        else
            break;
    }

    cifrarContrasena(contrasena, passCifrada);

    FILE *archivo = fopen("usuarios.txt", "a");
    if (archivo == NULL)
    {
        mvprintw(9, 10, "Error al abrir el archivo.");
        refresh();
        getch();
        return;
    }
    fprintf(archivo, "%s %s %s\n", nombre, correo, passCifrada);
    fclose(archivo);

    mvprintw(9, 10, "Usuario registrado correctamente!");
    refresh();
    getch();
}

/**
 * @brief Ventana de inicio de sesión
 *
 */
void iniciarSesion()
{
    char correo[50];
    char contrasena[50];
    char passCifrada[120];

    clear();
    mvprintw(3, 10, "=== INICIO DE SESION ===");
    mvprintw(5, 10, "Correo: ");
    echo();
    getnstr(correo, 49);
    noecho();

    mvprintw(6, 10, "Contrasena: ");
    noecho();
    getnstr(contrasena, 49);

    cifrarContrasena(contrasena, passCifrada);

    if (compararUsuario(correo, passCifrada))
        mvprintw(8, 10, "Bienvenido al sistema!");
    else
        mvprintw(8, 10, "Correo o contrasena incorrectos.");

    refresh();
    getch();
}

/**
 * @brief Revisa dentro de la base de datos si el usuario y contraseña coinciden
 *
 * @param correo
 * @param passCifrada
 * @return 1 si coinciden, 0 si no coinciden
 */
int compararUsuario(const char *correo, const char *passCifrada)
{
    FILE *archivo = fopen("usuarios.txt", "r");
    if (!archivo)
        return 0;

    char nombre[50], correoGuardado[50], passGuardada[120];
    while (fscanf(archivo, "%s %s %s", nombre, correoGuardado, passGuardada) == 3)
    {
        if (strcmp(correo, correoGuardado) == 0 && strcmp(passCifrada, passGuardada) == 0)
        {
            fclose(archivo);
            return 1;
        }
    }
    fclose(archivo);
    return 0;
}

/**
 * @brief Ventana para recuperar la contraseña olvidada
 *
 */
void olvideContrasena()
{
    char correo[50];
    char nueva[50];
    char cifrada[120];
    char nombre[50], tempCorreo[50], tempPass[120];
    int encontrado = 0;

    clear();
    mvprintw(3, 10, "=== OLVIDE MI CONTRASENA ===");
    mvprintw(5, 10, "Ingrese su correo: ");
    echo();
    getnstr(correo, 49);
    noecho();

    FILE *archivo = fopen("usuarios.txt", "r");
    FILE *temp = fopen("temp.txt", "w");

    if (!archivo || !temp)
    {
        mvprintw(7, 10, "Error abriendo archivos.");
        refresh();
        getch();
        return;
    }

    while (fscanf(archivo, "%s %s %s", nombre, tempCorreo, tempPass) == 3)
    {
        if (strcmp(tempCorreo, correo) == 0)
        {
            encontrado = 1;

            while (1)
            {
                mvprintw(7, 10, "Nueva contrasena: ");
                noecho();
                getnstr(nueva, 49);
                if (!validarContrasena(nueva))
                {
                    mvprintw(8, 10, "Contrasena invalida. Min 6 chars, 1 mayus, 1 numero, 1 especial.");
                    clrtoeol();
                    refresh();
                    getch();
                    mvprintw(7, 10, "                                ");
                }
                else
                    break;
            }

            cifrarContrasena(nueva, cifrada);
            fprintf(temp, "%s %s %s\n", nombre, tempCorreo, cifrada);
        }
        else
        {
            fprintf(temp, "%s %s %s\n", nombre, tempCorreo, tempPass);
        }
    }

    fclose(archivo);
    fclose(temp);
    remove("usuarios.txt");
    rename("temp.txt", "usuarios.txt");

    if (encontrado)
        mvprintw(9, 10, "Contrasena actualizada correctamente!");
    else
        mvprintw(9, 10, "Correo no encontrado.");

    refresh();
    getch();
}