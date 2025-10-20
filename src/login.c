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
            seleccion = (seleccion - 1 + nOpciones) % nOpciones;
            break;
        case KEY_DOWN:
            seleccion = (seleccion + 1) % nOpciones;
            break;
        case 10: // Enter
            switch (seleccion)
            {
            case 0:
                registrarUsuario();
                break;
            case 1:
                iniciarSesion();
                break;
            case 2:
                olvideContrasena();
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
    return (punto != NULL);
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
int validarContrasena(const char *pass)
{
    int len = strlen(pass);
    if (len < 6)
        return 0;
    int mayus = 0, num = 0, esp = 0;
    for (int i = 0; i < len; i++)
    {
        if (isupper(pass[i]))
            mayus = 1;
        else if (isdigit(pass[i]))
            num = 1;
        else if (!isalnum(pass[i]))
            esp = 1;
    }
    return (mayus && num && esp);
}

/**
 * @brief Verifica si un correo ya está registrado
 *
 * @param correo
 * @return int
 */
int correoDuplicado(const char *correo)
{
    FILE *archivo = fopen("usuarios.txt", "r");
    if (!archivo)
        return 0;
    char n[100], c[50], p[120];
    while (fscanf(archivo, "%[^;];%[^;];%[^\n]\n", n, c, p) == 3)
    {
        if (strcmp(correo, c) == 0)
        {
            fclose(archivo);
            return 1;
        }
    }
    fclose(archivo);
    return 0;
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
    char nombre[100], correo[50], contrasena[50], passCifrada[120];
    clear();
    mvprintw(3, 10, "=== REGISTRO DE USUARIO ===");

    while (1)
    {
        mvprintw(5, 10, "Nombre completo: ");
        echo();
        getnstr(nombre, 99);
        noecho();
        if (strlen(nombre) == 0)
        {
            mvprintw(6, 10, "El nombre no puede estar vacio.");
            refresh();
            getch();
            clear();
            mvprintw(3, 10, "=== REGISTRO DE USUARIO ===");
        }
        else
            break;
    }

    while (1)
    {
        mvprintw(6, 10, "Correo: ");
        echo();
        getnstr(correo, 49);
        noecho();
        if (!validarCorreo(correo))
        {
            mvprintw(7, 10, "Correo invalido (falta '@' o '.').");
            refresh();
            getch();
            clear();
            mvprintw(3, 10, "=== REGISTRO DE USUARIO ===");
        }
        else if (correoDuplicado(correo))
        {
            mvprintw(7, 10, "Correo ya registrado.");
            refresh();
            getch();
            clear();
            mvprintw(3, 10, "=== REGISTRO DE USUARIO ===");
        }
        else
            break;
    }

    while (1)
    {
        mvprintw(7, 10, "Contrasena: ");
        noecho();
        getnstr(contrasena, 49);
        if (!validarContrasena(contrasena))
        {
            mvprintw(8, 10, "Debe tener 6+, mayus, numero y caracter especial.");
            refresh();
            getch();
            mvprintw(8, 10, "                                                 ");
            mvprintw(7, 22, "                                                 ");
        }
        else
            break;
    }

    cifrarContrasena(contrasena, passCifrada);
    FILE *archivo = fopen("usuarios.txt", "a");
    if (!archivo)
    {
        mvprintw(10, 10, "Error guardando.");
        refresh();
        getch();
        return;
    }
    fprintf(archivo, "%s;%s;%s\n", nombre, correo, passCifrada);
    fclose(archivo);

    mvprintw(10, 10, "Usuario registrado correctamente!");
    refresh();
    getch();
}

/**
 * @brief Ventana de inicio de sesión
 *
 */
void iniciarSesion()
{
    char correo[50], contrasena[50], passCifrada[120], nombre[100];
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

    if (compararUsuario(correo, passCifrada, nombre))
    {
        pantallaBienvenida(nombre);
    }
    else
    {
        mvprintw(8, 10, "Correo o contrasena incorrectos.");
        refresh();
        getch();
    }
}

/**
 * @brief Pantalla de bienvenida tras iniciar sesión
 *
 * @param nombre Nombre del usuario
 */
void pantallaBienvenida(const char *nombre)
{
    const char *opciones[] = {"Salir"};
    int seleccion = 0, ch;

    while (1)
    {
        clear();
        mvprintw(3, 10, "=== BIENVENIDO AL SISTEMA ===");
        mvprintw(5, 12, "Hola, %s!", nombre);

        for (int i = 0; i < 1; i++)
        {
            if (i == seleccion)
            {
                attron(A_REVERSE);
                mvprintw(7 + i, 12, "%s", opciones[i]);
                attroff(A_REVERSE);
            }
            else
            {
                mvprintw(7 + i, 12, "%s", opciones[i]);
            }
        }

        ch = getch();
        if (ch == 10)
            break; // Enter
    }
}

/**
 * @brief Revisa dentro de la base de datos si el usuario y contraseña coinciden
 *
 * @param correo
 * @param passCifrada
 * @return 1 si coinciden, 0 si no coinciden
 */
int compararUsuario(const char *correo, const char *passCifrada, char *nombre)
{
    FILE *archivo = fopen("usuarios.txt", "r");
    if (!archivo)
        return 0;
    char n[100], c[50], p[120];
    while (fscanf(archivo, "%[^;];%[^;];%[^\n]\n", n, c, p) == 3)
    {
        if (strcmp(correo, c) == 0 && strcmp(p, passCifrada) == 0)
        {
            strcpy(nombre, n);
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
    char correo[50], nueva[50], cifrada[120];
    char nombre[100], c[50], p[120];
    int encontrado = 0;
    clear();
    mvprintw(3, 10, "=== RESTABLECER CONTRASENA ===");
    mvprintw(5, 10, "Correo: ");
    echo();
    getnstr(correo, 49);
    noecho();

    FILE *a = fopen("usuarios.txt", "r");
    FILE *tmp = fopen("temp.txt", "w");
    if (!a || !tmp)
    {
        mvprintw(7, 10, "Error abriendo archivo.");
        refresh();
        getch();
        return;
    }

    while (fscanf(a, "%[^;];%[^;];%[^\n]\n", nombre, c, p) == 3)
    {
        if (strcmp(c, correo) == 0)
        {
            encontrado = 1;
            while (1)
            {
                mvprintw(7, 10, "Nueva contrasena: ");
                noecho();
                getnstr(nueva, 49);
                if (!validarContrasena(nueva))
                {
                    mvprintw(8, 10, "Debe tener 6+, mayus, numero y caracter especial.");
                    refresh();
                    getch();
                    mvprintw(8, 10, "                                                   ");
                    mvprintw(7, 30, "                                                   ");
                }
                else
                    break;
            }
            cifrarContrasena(nueva, cifrada);
            fprintf(tmp, "%s;%s;%s\n", nombre, c, cifrada);
        }
        else
            fprintf(tmp, "%s;%s;%s\n", nombre, c, p);
    }

    fclose(a);
    fclose(tmp);
    remove("usuarios.txt");
    rename("temp.txt", "usuarios.txt");

    if (encontrado)
        mvprintw(10, 10, "Contrasena actualizada correctamente!");
    else
        mvprintw(10, 10, "Correo no encontrado.");
    refresh();
    getch();
}