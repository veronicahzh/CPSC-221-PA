/**
 * @file block.cpp
 * @description Student implementation of Block functions, CPSC 221 PA1
 * @author (your CWLs here)
**/

#include "block.h"
#include <cmath>
#include <iostream>

/**
 * Return the dimension(width or height) of the block.
**/
int Block::Dimension() const {
	/* your code here */
	return data.size();
}

/**
 * Renders the given block onto img with its upper
 * left corner at (x, y). Assumes the rendered block fits on the
 * image.
 * The block is enlarged using nearest-neighbour scaling
 * (i.e. no pixel blending).
 * @pre scale >= 1
**/
void Block::Render(PNG& img, int x, int y, int scale) const {
	int rowIndex = 0;
    for (vector<RGBAPixel> row : data) {
        
        int colIndex = 0;
        for (RGBAPixel pixel : row) {

            int startX = x + colIndex * scale;
            int startY = y + rowIndex * scale;

            for (int dx = 0; dx < scale; dx++) {
                for (int dy = 0; dy < scale; dy++) {
                    *img.getPixel(startX + dx, startY + dy) = pixel;
                }
            }

            colIndex++;
        }
        
        rowIndex++;
    }
}

/**
 * Creates a block that is dimension X dimension pixels in size
 * by copying the pixels from (x, y) to (x+dimension-1, y+dimension-1)
 * in img. Assumes img is large enough to supply these pixels.
**/
void Block::Build(PNG& img, int x, int y, int dimension) {
    data.clear();
	
    data.resize(dimension); // data has dimension rows

    for (int row = 0; row < dimension; row++) { // y direction
        data[row].resize(dimension); // each row has dimension pixels (row x col = dim x dim)

        for (int col = 0; col < dimension; col++) { // x direction
            
            int startX = x + col;
            int startY = y + row;

            RGBAPixel *pixel = img.getPixel(startX, startY);

            data[row][col] = *pixel; // store copy of pixel into block
        }
    }

}

/**
 * Rearranges the image data in this Block so that it is transposed
 * (flipped) over the diagonal line from upper-left to lower-right
 * e.g.  1 2 3      1 4 7
 *       4 5 6  ->  2 5 8
 *       7 8 9      3 6 9
**/
void Block::Transpose() {
	/* your code here */

}
