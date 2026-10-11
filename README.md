# Compilers

Mini Compilador en C++

Mini compilador funcional desarrollado en C++ para la Evaluación Final de la asignatura de Compiladores. Integra análisis léxico, sintáctico, semántico, generación de código intermedio (TAC) y optimización básica.

Estructura del Proyecto

Token.h: Definición de tipos de tokens y estructuras de errores.

Lexer.h / Lexer.cpp: Analizador léxico (soporte multilínea, strings con comillas dobles y manejo continuo de errores).

Parser.h / Parser.cpp: Analizador sintáctico, semántico (tabla de símbolos), TAC y optimizador (Constant Folding).

main.cpp: Programa principal con casos de prueba integrados.

Requisitos

Compilador de C++ con soporte para C++11 o superior (ej. g++, clang, MinGW).

Compilación y Ejecución (Linux / macOS / Windows con g++)

Clonar o descargar el repositorio.

Compilar todos los archivos fuente juntos mediante la terminal:

g++ main.cpp Lexer.cpp Parser.cpp -o compilador


Ejecutar el programa:

En Linux / macOS:

./compilador


En Windows:

./compilador.exe
