#include "Parser.h"
#include <iostream>
#include <cctype>

using namespace std;
// Constructor
Parser::Parser(vector<Token> tokens)
{

    this->tokens = tokens;
    posicion = 0;
    tempCount = 1; //Pregunta: 4
}

// Verifica si terminamos de analizar
bool Parser::esFinal()
{

    return posicion >= tokens.size();
}

// Obtiene el token actual
Token Parser::actual()
{

    return tokens[posicion];
}

// Comprueba si el token actual
// corresponde al tipo esperado
bool Parser::coincidir(TipoToken tipoEsperado)
{
    if (!esFinal() &&
        actual().tipo == tipoEsperado)
    {

        posicion++;

        return true;
    }

    return false;
}

// ---------------------------------
// Tipo → int | float
// ---------------------------------

bool Parser::tipo()
{
    if (esFinal())
    {
        return false;
    }

    if (actual().tipo == TipoToken::PALABRA_CLAVE)
    {

        if (actual().lexema == "int" ||
            actual().lexema == "float" || 
            actual().lexema == "string") // Soportar String
        {

            posicion++;

            return true;
        }
    }
    return false;
}

// ---------------------------------
// Valor → NUM | REAL | ID
// ---------------------------------

bool Parser::valor()
{

    if (esFinal())
    {
        return false;
    }

    if (coincidir(TipoToken::LITERAL_ENTERO))
    {
        return true;
    }

    if (coincidir(TipoToken::LITERAL_REAL))
    {
        return true;
    }
    // Literal String
     if (coincidir(TipoToken::LITERAL_STRING))
    {
        return true;
    }

    if (coincidir(TipoToken::IDENTIFICADOR))
    {
        return true;
    }

    return false;
}

// ---------------------------------
// Declaracion → Tipo ID = Valor ;
// ---------------------------------

bool Parser::declaracion()
{   
    string tipoVar = actual().lexema;  // Agregar String
    // Tipo
    if (!tipo())
    {
        error("int o float o string");
        return false;
    }

    // ID
    if (actual().tipo != TipoToken::IDENTIFICADOR)
    {
        error("IDENTIFICADOR");
        return false;
    }

    //Agregar String
    string idVar = actual().lexema;    
    int lin = actual().linea;
    posicion++;

    // SEMÁNTICO: Variable declarada dos veces
    if (tablaSimbolos.find(idVar) != tablaSimbolos.end()) {
        erroresSemanticos.push_back("Error Semantico en linea " + to_string(lin) + ": Variable '" + idVar + "' ya fue declarada.");
    } else {
        tablaSimbolos[idVar] = tipoVar;
    }

    // =
    if (!coincidir(TipoToken::ASIGNACION))
    {
        error("'='");
        return false;
    }

    Token valToken = actual();
    string tipoVal = "";
    if (valToken.tipo == TipoToken::LITERAL_ENTERO) tipoVal = "int";
    else if (valToken.tipo == TipoToken::LITERAL_REAL) tipoVal = "float";
    else if (valToken.tipo == TipoToken::LITERAL_STRING) tipoVal = "string";
    else if (valToken.tipo == TipoToken::IDENTIFICADOR) {
        // SEMÁNTICA: Identificador no declarado
        if (tablaSimbolos.find(valToken.lexema) == tablaSimbolos.end()) {
            erroresSemanticos.push_back("Error Semantico en linea " + to_string(valToken.linea) + ": Variable '" + valToken.lexema + "' no declarada.");
        } else {
            tipoVal = tablaSimbolos[valToken.lexema];
        }
    }

    // SEMÁNTICA: Incompatibilidad de tipos
    if (!tipoVal.empty() && tipoVar != tipoVal) {
        erroresSemanticos.push_back("Error Semantico en linea " + to_string(lin) + ": Incompatibilidad de tipo (" + tipoVal + " a " + tipoVar + ").");
    }

    if (!valor()) { error("NUM, REAL, STRING o IDENTIFICADOR"); return false; }
    if (!coincidir(TipoToken::DELIMITADOR)) { error("';'"); return false; }

    return true;
}
/*  // Valor
    if (!valor())
    {
        error("NUM, REAL o IDENTIFICADOR");
        return false;
    }

    // ;
    if (!coincidir(TipoToken::DELIMITADOR))
    {
        error("';'");
        return false;
    }

    return true;
}
*/

// ---------------------------------
// Iniciar análisis sintáctico
// ---------------------------------

/*bool Parser::analizar()
{
    if (!declaracion())
    {
        return false;
    }

    // Verificamos que no sobren tokens
    if (!esFinal())
    {

        cout << "Error sintactico: "
             << "existen tokens adicionales."
             << endl;

        return false;
    }
    return true;
}
*/

void Parser::error(string esperado)
{
    if (!esFinal())
    {
        erroresSintacticos.push_back(
            "Error sintactico en linea " + to_string(actual().linea) +
            ", columna " + to_string(actual().columna) +
            ": se esperaba '" + esperado + "', pero se encontro '" + actual().lexema + "'"
        );
    }
    else
    {
        erroresSintacticos.push_back(
            "Error sintactico: se esperaba '" + esperado + "', pero se llego al final de la entrada."
        );
    }
}

/*{
    if (!esFinal())
    {
        cout << "Error sintactico en linea "
             << actual().linea
             << ", columna "
             << actual().columna
             << ": se esperaba "
             << esperado
             << ", pero se encontro '"
             << actual().lexema
             << "'."
             << endl;
    }
    else
    {
        cout << "Error sintactico: "
             << "se esperaba "
             << esperado
             << ", pero se llego al final de la entrada."
             << endl;
    }
} */

string Parser::factor()
{
    if (actual().tipo == TipoToken::LITERAL_ENTERO || actual().tipo == TipoToken::LITERAL_REAL) {
        string val = actual().lexema;
        posicion++;
        return val;
    } else if (actual().tipo == TipoToken::IDENTIFICADOR) {
        string id = actual().lexema;
        if (tablaSimbolos.find(id) == tablaSimbolos.end()) {
            erroresSemanticos.push_back("Error Semantico en linea " + to_string(actual().linea) + ": Variable '" + id + "' no declarada.");
        }
        posicion++;
        return id;
    }

    // MANEJO DE PARÉNTESIS PARA PRIORIDAD 
    else if (actual().lexema == "(") {
        posicion++; // Consume '('
        string resInterno = expresion(); // Evalúa la suma/expresión interna primero
        if (actual().lexema == ")") {
            posicion++; // Consume ')'
        } else {
            error("')'");
        }
        return resInterno; // Devuelve la variable temporal generada adentro (ej. t1)
    }
    return "";
}


string Parser::termino()
{
    string izq = factor();
    while (!esFinal() && actual().tipo == TipoToken::OPERADOR && (actual().lexema == "*" || actual().lexema == "/")) {
        string op = actual().lexema;
        posicion++;
        string der = factor();
        string temp = "t" + to_string(tempCount++);
        codigoTAC.push_back({op, izq, der, temp});
        izq = temp;
    }
    return izq;
}

string Parser::expresion()
{
    string izq = termino();
    while (!esFinal() && actual().tipo == TipoToken::OPERADOR && (actual().lexema == "+" || actual().lexema == "-")) {
        string op = actual().lexema;
        posicion++;
        string der = termino();
        string temp = "t" + to_string(tempCount++);
        codigoTAC.push_back({op, izq, der, temp});
        izq = temp;
    }
    return izq;
}

bool Parser::instruccion()
{
    if (actual().tipo == TipoToken::PALABRA_CLAVE) {
        return declaracion();
    } else if (actual().tipo == TipoToken::IDENTIFICADOR) {
        string id = actual().lexema;
        int lin = actual().linea;
        posicion++;

        if (tablaSimbolos.find(id) == tablaSimbolos.end()) {
            erroresSemanticos.push_back("Error Semantico en linea " + to_string(lin) + ": Variable '" + id + "' no declarada.");
        }

        if (!coincidir(TipoToken::ASIGNACION)) { error("'='"); return false; }
        string resExp = expresion();
        codigoTAC.push_back({"=", resExp, "", id});
        if (!coincidir(TipoToken::DELIMITADOR)) { error("';'"); return false; }
        return true;
    }
    return false;
}

bool Parser::analizar()
{
    bool exito = true;
    while (!esFinal()) {
        if (!instruccion()) {
            exito = false;
            posicion++; // Permite continuar tras un error sintáctico
        }
    }
    return exito;
}

void Parser::mostrarTablaSimbolos()
{
    cout << "\n--- TABLA DE SIMBOLOS ---\n";
    for (auto par : tablaSimbolos) {
        cout << par.first << " -> " << par.second << endl;
    }
}

void Parser::mostrarTAC()
{
    cout << "\n--- CODIGO INTERMEDIO (TAC) ---\n";
    for (auto t : codigoTAC) {
        if (t.op == "=") cout << t.res << " = " << t.arg1 << endl;
        else cout << t.res << " = " << t.arg1 << " " << t.op << " " << t.arg2 << endl;
    }
}

void Parser::optimizarTAC()
{
    cout << "\n--- CODIGO INTERMEDIO OPTIMIZADO ---\n";
    for (auto t : codigoTAC) {
        if ((t.op == "+" || t.op == "-" || t.op == "*" || t.op == "/") && isdigit(t.arg1[0]) && isdigit(t.arg2[0])) {
            int n1 = stoi(t.arg1), n2 = stoi(t.arg2), r = 0;
            if (t.op == "+") r = n1 + n2;
            else if (t.op == "-") r = n1 - n2;
            else if (t.op == "*") r = n1 * n2;
            else if (t.op == "/") r = (n2 != 0) ? n1 / n2 : 0;
            cout << t.res << " = " << r << "  (Optimizado)" << endl;
        } else {
            if (t.op == "=") cout << t.res << " = " << t.arg1 << endl;
            else cout << t.res << " = " << t.arg1 << " " << t.op << " " << t.arg2 << endl;
        }
    }
}