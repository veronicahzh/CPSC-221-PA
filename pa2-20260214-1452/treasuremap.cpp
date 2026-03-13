/**
 * @file treasuremap.cpp
 * @description Implementations for PA2, TreasureMap class
 * @author (vhzh)
 * @author (aguha01)
**/
#include "treasuremap-private.h"
#include "treasuremap.h"
#include "queue.h"

#define BLUEMASK 0b00000011
#define GREENMASK 0b00001100
#define REDMASK 0b00110000
using namespace std;

TreasureMap::TreasureMap(const PNG& baseim, const PNG& mazeim, pair<int, int> s) {
	base = baseim;
	maze = mazeim; 
	start = s; 
}

void TreasureMap::SetGrey(PNG& im, pair<int, int> loc) {
	RGBAPixel *pixel = im.getPixel(loc.first, loc.second);
	pixel -> r = 2 * (pixel -> r / 4); 
	pixel -> g = 2 * (pixel -> g / 4);
	pixel -> b = 2 * (pixel -> b / 4);
}

void TreasureMap::SetLOB(PNG& im, pair<int, int> loc, int d) {
	int bitd = d % 64;

    int rVal = (bitd & REDMASK) >> 4;
    int gVal = (bitd & GREENMASK) >> 2;
    int bVal = (bitd & BLUEMASK) >> 0;

    RGBAPixel *pixel = im.getPixel(loc.first, loc.second);

    pixel->r = (pixel->r & 0b11111100) | rVal;  
    pixel->g = (pixel->g & 0b11111100) | gVal;
    pixel->b = (pixel->b & 0b11111100) | bVal;
}

PNG TreasureMap::RenderMap() {
	/* REPLACE THE LINE BELOW WITH YOUR CODE */
	return PNG();
}


PNG TreasureMap::RenderMaze() {
	/* REPLACE THE LINE BELOW WITH YOUR CODE */
	return PNG();
}

bool TreasureMap::Good(vector<vector<bool>>& v, pair<int, int> curr, pair<int, int> next) {
	/* REPLACE THE LINE BELOW WITH YOUR CODE */
	return false;
}

vector<pair<int, int>> TreasureMap::Neighbours(pair<int, int> curr) {
	/* REPLACE THE LINES BELOW WITH YOUR CODE */
	vector<pair<int, int>> v;
	return v;
}

/***********************************************
* IF YOU DECLARED ANY ADDITIONAL PRIVATE       *
* FUNCTIONS IN treasuremap-private.h, COMPLETE *
* THEIR IMPLEMENTATIONS BELOW                  *
***********************************************/
