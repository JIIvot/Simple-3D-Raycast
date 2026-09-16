#pragma once

#include <cstdint>

constexpr float c_cellSizeMeters = 4.0f;

constexpr int32_t c_mapWidthCells = 32;
constexpr int32_t c_mapHeightCells = 18;

constexpr float c_editorPixelsPerMeter = 5.0f;

constexpr int32_t c_editorWidthPixels = c_mapWidthCells * static_cast<int32_t>( c_cellSizeMeters * c_editorPixelsPerMeter );
constexpr int32_t c_editorHeightPixels = c_mapHeightCells * static_cast<int32_t>( c_cellSizeMeters * c_editorPixelsPerMeter );

constexpr int32_t c_gameWidthPixels = 320;
constexpr int32_t c_gameHeightPixels = 180;

constexpr int32_t c_cellSizePixels = static_cast<int32_t>( c_editorPixelsPerMeter * c_cellSizeMeters );

/*================================================================================*/
/* c_halfVerticalFovTan works like that only if c_horizontalFov is 90 degrees!    */
/* General formula, where a - horizontal FOV, b - vertical FOV, x - aspect ratio: */
/* tan(b / 2) = tan(a / 2) / x                                                    */
/*================================================================================*/
constexpr float c_horizontalFov = 90.0f;
constexpr float c_halfVerticalFovTan = static_cast<float>( c_gameHeightPixels ) / c_gameWidthPixels;

constexpr float c_angleBetweenRays = c_horizontalFov / ( c_gameWidthPixels - 1 );
