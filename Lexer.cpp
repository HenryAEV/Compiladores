#include "Lexer.h"
#include <cctype>

Lexer::Lexer(string _codigo)
{
    codigo = _codigo;
    posicion = 0;
    linea = 1;
    columna = 1;
}

bool Lexer::esFinal()
{
    return posicion >= codigo.length();
}

char Lexer::actual()
{
    return codigo[posicion];
}

char Lexer::avanzar()
{
    char caracter = codigo[posicion];
    posicion++;
    // controla salto de linea
    if (caracter == '\n')
    {
        linea++;
        columna = 1;
    }
    else
    {
        columna++;
    }

    return caracter;
}

void Lexer::ignorarEspacios()
{
    while (!esFinal() && isspace(actual()))
    {
        avanzar();
    }
}

Token Lexer::reconocerNumero()
{
    string lexema = "";

    int lineaInicio = linea;
    int columnaInicio = columna;

    bool tienePunto = false;

    while (!esFinal())
    {
        if (isdigit(actual()))
        {
            lexema += avanzar();
        }
        else if (actual() == '.')
        {
            tienePunto = true;
            lexema += avanzar();
        }
        else
        {
            break;
        }
    }

    if (tienePunto)
    {
        return {
            TipoToken::LITERAL_REAL,
            lexema,
            lineaInicio,
            columnaInicio};
    }

    return {
        TipoToken::LITERAL_ENTERO,
        lexema,
        lineaInicio,
        columnaInicio};
}

Token Lexer::reconocerIdentificador()
{
    string lexema = "";

    int lineaInicio = linea;
    int columnaInicio = columna;

    while (!esFinal() &&
           (isalnum(actual()) || actual() == '_'))
    {
        lexema += avanzar();
    }

    if (esPalabraClave(lexema))
    {
        return {
            TipoToken::PALABRA_CLAVE,
            lexema,
            lineaInicio,
            columnaInicio};
    }

    return {
        TipoToken::IDENTIFICADOR,
        lexema,
        lineaInicio,
        columnaInicio};
}

Token Lexer::reconocerOperador()
{
    string lexema = "";

    int lineaInicio = linea;
    int columnaInicio = columna;

    lexema += avanzar();

    return {
        TipoToken::OPERADOR,
        lexema,
        lineaInicio,
        columnaInicio};
}

// NUEVA FUNCIÓN PARA RECONOCER "STRING"
Token Lexer::reconocerString()
{
    string lexema = "";

    int lineaInicio = linea, columnaInicio = columna;

    avanzar(); // Salta la comilla inicial

    while (!esFinal() && actual() != '"' && actual() != '\n')
    {
        lexema += avanzar();
    }

    if (!esFinal() && actual() == '"')
    {
        avanzar(); // Salta la comilla final
    }
    else
    {
        registrarError(lexema, lineaInicio, columnaInicio, "String no cerrado con comilla doble");
    }

    return {TipoToken::LITERAL_STRING, lexema, lineaInicio, columnaInicio};
}

bool Lexer::esPalabraClave(string palabra)
{
    return palabra == "int" || palabra == "float" || palabra == "string" || palabra == "if" || palabra == "else";
}

// error lexico
vector<ErrorLexico> Lexer::obtenerErrores()
{
    return errores;
}

void Lexer::registrarError(string lexema, int lineaError, int columnaError, string mensaje)
{
    ErrorLexico error;
    error.lexema = lexema;
    error.linea = lineaError;
    error.columna = columnaError;
    error.mensaje = mensaje;
    errores.push_back(error);
}

vector<Token> Lexer::analizar()
{
    vector<Token> tokens;
    while (!esFinal())
    {
        ignorarEspacios();
        if (esFinal())
        {
            break;
        }
        char c = actual();

        // Literal entero o real
        if (isdigit(c))
        {
            tokens.push_back(reconocerNumero());
        }

        // Identificador o palabra clave
        else if (isalpha(c) || c == '_')
        {
            tokens.push_back(reconocerIdentificador());
        }

        // SOPORTE PARA STRING
        else if (c == '"')
        {
            tokens.push_back(reconocerString());
        }

        // Operadores aritméticos
        else if (c == '+' || c == '-' || c == '*' || c == '/')
        {
            tokens.push_back(reconocerOperador());
        }

        // Agregar prioridad paréntesis
        else if (c == '(' || c == ')')
        {
            int lineaInicio = linea;
            int columnaInicio = columna;
            string lexema = "";
            lexema += avanzar();
            tokens.push_back({TipoToken::OPERADOR, lexema, lineaInicio, columnaInicio});
        }

        // Asignación
        else if (c == '=')
        {
            int lineaInicio = linea;
            int columnaInicio = columna;

            string lexema = "";
            lexema += avanzar();

            tokens.push_back({TipoToken::ASIGNACION,
                              lexema,
                              lineaInicio,
                              columnaInicio});
        }

        // Delimitador
        else if (c == '=')
        {
            int lineaInicio = linea;
            int columnaInicio = columna;

            string lexema = "";
            lexema += avanzar();

            tokens.push_back({TipoToken::ASIGNACION,
                              lexema,
                              lineaInicio,
                              columnaInicio});
        }

        else if (c == ';')
        {
            int lineaInicio = linea;
            int columnaInicio = columna;

            string lexema = "";
            lexema += avanzar();

            tokens.push_back({TipoToken::DELIMITADOR,
                              lexema,
                              lineaInicio,
                              columnaInicio});
        }

        // Carácter desconocido - error lexico
        else
        {
            int lineaError = linea;
            int columnaError = columna;

            string lexema = "";
            lexema += avanzar();

            registrarError(lexema, lineaError, columnaError, "Caracter no reconocido");
            tokens.push_back({TipoToken::DESCONOCIDO,
                              lexema,
                              lineaError,
                              columnaError});
        }
    }
    return tokens;
}