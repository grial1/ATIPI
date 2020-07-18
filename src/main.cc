/** @file main.cc
 * Pruebas con el predictor MED
 */

#include <string>
#include <iostream>
#include <filesystem>
#include "error_calculator.h"
#include "context_sign_matrix.h"
#include "decompressor.h"
#include "file_handler.h"

/// Usar namespace de la biblioteca estándar
using namespace std;
namespace fs = std::filesystem;

/// Código de estado
const int ERROR_USAGE = -1;
const int OK = 0;

static int** initialize_errors_matrix(int height, int width){
    int** errors = new int*[height];
    for (int h = 0; h < height; h++){
            errors[h] = new int[width];
            for (int w = 0; w < width; w++){
                  // fill in some initial values
                  errors[h][w] = 0;
            }
      }
      return errors;
}

int main(int argc, char* argv[])
{

    if(argc < 3)
    {

        cout << "USAGE: Compressor <image-path> <N> | -d <compressed-image-path>" << endl;
        exit(ERROR_USAGE);

    }

    if( strncmp( argv[1], "-d", strlen("-d")) != 0 )
    {

        /// Comprimir archivo
        cout << "Comprimiendo "<< argv[1] << endl;
        GreyImage oImg( argv[1] );

        string filename{fs::path(argv[1]).stem().string()};
        compress(oImg, filename.c_str(), "P5", oImg.getWidth(), oImg.getHeight(), atoi(argv[2]));

    } else {

        /// Descomprimir archivo
        cout << "Descomprimiendo "<< argv[2] << endl;
        decompress_from_file(argv[2]);

    }
    cout << "Operación terminada " << endl;

    exit(OK);
}