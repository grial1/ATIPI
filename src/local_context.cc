/** @file local_context.cc
 * Determinación del contexto local para 
 * hallar los estadísticos A (suma de valores
 * absolutos de los errores) y n (cantidad de 
 * muestras).
 */

#include "local_context.h"

/// @brief Class constructor
ContextTable::ContextTable()
{};

/**
 * @brief Class constructor
 * @param height [in] - alto
 * @param width [in] - ancho
*/
ContextTable::ContextTable(int height, int width):
height{height},width{width}{};

/// @brief Class destructor
ContextTable::~ContextTable()
{};

/**
 * @brief Función local que determina la mascara de los pixeles que pertenecen al contexto
 * @param N [in] - Tamaño del contexto
 * @returns oMask, lista de pixeles en la mascara del contexto local
*/
static ContextMask& createPixelMask(const size_t N)
{

    bool find_all = false;
    int distance = 0, total_found = 0;
    ContextMask* oMask = new ContextMask();
    do
    {

        distance++;
        int found = 0;
        int secDistance = 0;

        while ( secDistance <= distance && total_found+found < N )
        {

            if( distance > secDistance && total_found+found < N )
            {
                oMask->push_back(PixelPos{ 0 - secDistance, 0 - distance });
                found++;
            }
            if( total_found+found < N )
            {
                oMask->push_back(PixelPos{ 0 - distance, 0 - secDistance });
                found++;
            }
            if( secDistance > 0 && total_found+found < N )
            {
                oMask->push_back(PixelPos{ 0 - distance, 0 + secDistance });
                found++;
            }

        secDistance++; 
        }

        total_found += found;
        if(total_found == N || found == 0)
            find_all = true;

    } while(!find_all);
    return *oMask;

}

/**
 * @brief Función local para aplicar la mascara del contexto local a cada uno de los pixeles
 * @param oPosition [in] - Ubicación del pixel (x)
 * @param oMask [in] - Mascara del contexto
 * @param width [in] - Cantidad de columnas de pixeles de la imagen
 * @param height [in] - Cantidad de filas de pixeles de la imagen
 * @param oRet [out] - Vector con pixeles del contexto
 * @returns oRet, lista de pixeles en el contexto local de oPosition
*/
static void applyMask(const PixelPos& oPosition, const ContextMask& oMask, const int width, const int height, vector<PixelPos>& oRet)
{
    /// Tomar cada pixel de la mascara
    for(const auto& dot : oMask )
    {

        PixelPos oCandidate{ oPosition.first + dot.first, oPosition.second + dot.second };
        if( oCandidate.first >= 0       &&
            oCandidate.first < height   &&
            oCandidate.second >=0       &&
            oCandidate.second < width   )
            oRet.push_back(oCandidate);
        else
            continue;

    }
}

/**
 * @brief Función que calcula la tabla de contextos
 * @param N [in] - Tamaño del contexto
 * @param width [in] - Cantidad de columnas de pixeles de la imagen
 * @param height [in] - Cantidad de filas de pixeles de la imagen
 * @returns oTable, o tabla con pares de desplazamientos
*/
const ContextTable& getLocalContext(const size_t N, const int width, const int height)
{

    ContextTable* oTable = new ContextTable(height, width);
    ContextMask oMask{ createPixelMask(N) };

    for(int row = 0; row < height; ++row)
    {
        for(int col = 0; col < width; ++col)
        {

            PixelPos oMappedPosition{ row * (height - (height-width)), col };
            if( oTable->find(oMappedPosition) == oTable->end() )
            {
                vector<PixelPos> oNeighbours{};
                applyMask(PixelPos{row, col}, oMask, width, height, oNeighbours);
                oTable->insert(make_pair(oMappedPosition, oNeighbours));
            }

        }
    }

    return (*oTable);

}