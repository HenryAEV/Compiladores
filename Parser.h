#ifndef PARSER_H
#define PARSER_H

#include <vector>
#include <unordered_map>
#include "Token.h"

using namespace std;

// Estructura triada
struct TAC {
    string op, arg1, arg2, res;
};
class Parser
{
private:
    vector<Token> tokens;
    int posicion;

    // Pregunta 3: Análisis Semántico
    unordered_map<string, string> tablaSimbolos; // ID -> Tipo

    // PRegunta 4: Código Intermedio
    int tempCount;
    vector<TAC> codigoTAC;

    // Devuelve true si ya no quedan tokens
    bool esFinal();

    // Devuelve el token que estamos analizando
    Token actual();

    // Comprueba si el token actual es del tipo esperado
    bool coincidir(TipoToken tipo);

    // No terminales de nuestra gramática
    bool tipo();
    bool valor();
    bool declaracion();
    bool instruccion();    //Pregunta 4

    //Pregunta 4:
    string expresion();
    string termino();
    string factor();

public:
    //Pregunta 4:
    vector<string> erroresSintacticos;
    vector<string> erroresSemanticos;

    Parser(vector<Token> tokens);

    // Inicia el análisis sintáctico
    bool analizar();
    void error(string esperado);
    
    // Pregunta 4: Código intermedio
    void mostrarTablaSimbolos();
    void mostrarTAC();
    void optimizarTAC();
};

#endif