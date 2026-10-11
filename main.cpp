#include <iostream>
#include <vector>
#include "Lexer.h"
#include "Parser.h"

using namespace std;

string nombreToken(TipoToken tipo)
{
    switch (tipo)
    {
    case TipoToken::LITERAL_ENTERO:
        return "LITERAL_ENTERO";

    case TipoToken::LITERAL_REAL:
        return "LITERAL_REAL";

        case TipoToken::LITERAL_STRING:  // String
        return "LITERAL_STRING";

    case TipoToken::IDENTIFICADOR:
        return "IDENTIFICADOR";

    case TipoToken::PALABRA_CLAVE:
        return "PALABRA_CLAVE";

    case TipoToken::OPERADOR:
        return "OPERADOR";

    case TipoToken::ASIGNACION:
        return "ASIGNACION";

    case TipoToken::DELIMITADOR:
        return "DELIMITADOR";

    case TipoToken::DESCONOCIDO:
        return "DESCONOCIDO";
    }
    return "";
}

int main()
// {
//     string codigo;
//     cout << "Ingrese codigo: ";
//     getline(cin, codigo);
//     Lexer lexer(codigo);
//     vector<Token> tokens = lexer.analizar();
//     cout << "--------TOKENS ENCONTRADOS---------\n";
//     for (Token token : tokens)
//     {
//         cout
//             << "<"
//             << nombreToken(token.tipo)
//             << ", "
//             << token.lexema
//             << ">"
//             << endl;
//     }
{
    // Código de prueba multilínea listo para ejecutar
    string codigoFuente = 
        "int edad = 20;\n"
        "float precio = 10.5;\n"
        "string nombre = \"Ana\";\n"
        "string dato = \"10.5\";\n"
        "int edad = 30;\n"
        "a = @ 5;\n"
        "b = noDeclarada;\n"
        "int x 10;\n"
        "resultado = (a + b) * c;\n"
        "resultado = 2 + 3;\n";

    cout << "======================================\n";
    cout << "      CODIGO FUENTE DE ENTRADA        \n";
    cout << "======================================\n";
    cout << codigoFuente << "\n";

    // 1. Análisis Léxico
    Lexer lexer(codigoFuente);
    vector<Token> tokens = lexer.analizar();

    vector<ErrorLexico> errores = lexer.obtenerErrores();
    cout << "\n-------- ERRORES LEXICOS --------\n";

    if (errores.empty())
    {
        cout << "No se encontraron errores lexicos." << endl;
    }
    else
    {
        for (ErrorLexico error : errores)
        {
            cout << "Lexema: " << error.lexema << endl;
            cout << "Linea: " << error.linea << endl;
            cout << "Columna: " << error.columna << endl;
            cout << "Mensaje: " << error.mensaje << endl;
            cout << "----------------------" << endl;
        }
    }

   //cout << "\n===============================" << endl;
  // cout << "     ANALISIS SINTACTICO" << endl;
 // cout << "===============================" << endl;

    Parser parser(tokens);

    parser.analizar();

    // Errores Sintacticos vacio 
    cout << "\n-------- ERRORES SINTACTICOS --------\n";
    if (parser.erroresSintacticos.empty())
    {
        cout << "No se encontraron errores sintacticos." << endl;
    }
    else
    {
        for (string error : parser.erroresSintacticos)
        {
            cout << error << endl;
        }
    } 

    cout << "\n-------- ERRORES SEMANTICOS --------\n";
    if (parser.erroresSemanticos.empty())
    {
        cout << "No se encontraron errores semanticos." << endl;
    }
    else
    {
        for (string error : parser.erroresSemanticos)
        {
            cout << error << endl;
        }
    }

    parser.mostrarTablaSimbolos();
    parser.mostrarTAC();
    parser.optimizarTAC();

    return 0;
}
    
    
    /*if (parser.analizar())
    {

        cout << "\nResultado: "
             << "Instruccion sintacticamente correcta."
             << endl;
    }
    else
    {

        cout << "\nResultado: "
             << "Instruccion sintacticamente incorrecta."
             << endl;
    }

    return 0;
}
*/

// int edad = 20;
// float promedio = nota1 + 15.5;
// x = 10 @ 2;
// int resultado = nota1 * 2 + 10;