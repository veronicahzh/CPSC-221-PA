/**
 * @file decoder.cpp
 * @description Implementations for PA2, Decoder class
 * @author (your CWLs)
**/

#include "decoder.h"
#include "queue.h"
#include "stack.h"

#define BLUEMASK 0b00000011
#define GREENMASK 0b00001100
#define REDMASK 0b00110000
#define LOWEST2BITS 0b00000011
using namespace std;

Decoder::Decoder(const PNG & tm, pair<int, int> s) : start(s), mapImg(tm) {
    /* YOUR CODE HERE */
    
}

PNG Decoder::RenderSolution(){
    /* REPLACE THE LINE BELOW WITH YOUR CODE */
    return PNG();
}

PNG Decoder::RenderMaze(){
    PNG mapCopy = mapImg;

    vector<vector<bool>> isVisited(mapImg.width(), vector<bool>(mapImg.height(), false));
    vector<vector<int>> distance(mapImg.height(), vector<int>(mapImg.width(), 0));
    Queue<pair<int, int>> toSearch;

    isVisited[start.first][start.second] = true;
    distance[start.second][start.first] = 0;
    toSearch.Enqueue(start);

    while (!toSearch.IsEmpty()) {
        pair<int, int> curr = toSearch.Dequeue();
        SetGrey(mapCopy, curr);

        for (auto & neighbour : Neighbours(curr)) {
            if (Good(isVisited, distance, curr, neighbour)) {
                isVisited[neighbour.first][neighbour.second] = true;
                distance[neighbour.second][neighbour.first] = distance[curr.second][curr.first] + 1;
                toSearch.Enqueue(neighbour);
            }
        }
    }

    for (int x = start.first - 3; x <= start.first + 3; x++) {
        for (int y = start.second - 3; y <= start.second + 3; y++) {
            if (x >= 0 && y >= 0 && x < (int) mapImg.width() && y < (int) mapImg.height()) {
                RGBAPixel * pixel = mapCopy.getPixel(x, y);
                pixel->r = 255;
                pixel->g = 0;
                pixel->b = 0;
            }
        }
    }

    return mapCopy;
}

void Decoder::SetGrey(PNG& im, pair<int, int> loc){
    RGBAPixel *pixel = im.getPixel(loc.first, loc.second);
	pixel -> r = 2 * (pixel -> r / 4); 
	pixel -> g = 2 * (pixel -> g / 4);
	pixel -> b = 2 * (pixel -> b / 4);
}

pair<int, int> Decoder::FindSpot(){
    /* REPLACE THE LINES BELOW WITH YOUR CODE */
    pair<int, int> spot;
    return spot;
}

int Decoder::PathLength(){
    // d = |x1 - x2| + |y1 - y2|
    int x1 = start.first;
    int y1 = start.second;

    pair<int, int> secondCoord = FindSpot();
    int x2 = secondCoord.first;
    int y2 = secondCoord.second;

    return abs(x1 - x2) + abs(y1 - y2);
}

bool Decoder::Good(vector<vector<bool>>& v, vector<vector<int>>& d, pair<int, int> curr, pair<int, int> next){
    int height = (int) mapImg.height();
	int width = (int) mapImg.width();

	bool inBounds = next.first >= 0 && next.second >= 0 
				&& next.first < width 
				&& next.second < height;
    if (!inBounds) {
        return false;
    }

    if (v[next.first][next.second]) {
        return false;
    }

    RGBAPixel * nextPixel = mapImg.getPixel(next.first, next.second);
    int encoded = ((nextPixel->r & LOWEST2BITS) << 4)
                  | ((nextPixel->g & LOWEST2BITS) << 2)
                  | (nextPixel->b & LOWEST2BITS);
    int expected = (d[curr.second][curr.first] + 1) % 64;

    return encoded == expected;
    
}

vector<pair<int, int>> Decoder::Neighbours(pair<int, int> curr) {
    int dx = 1; 
	int dy = 1; 
	vector<pair<int,int>> neighbours = { {curr.first - dx, curr.second }, 
										 {curr.first, curr.second + dy },
										 {curr.first + dx, curr.second },
										 {curr.first, curr.second - dy }};
	return neighbours; 
}

bool Decoder::Compare(RGBAPixel p, int d){
    int bitd = (d + 1) % 64;

    int rVal = (bitd & REDMASK) >> 4;
    int gVal = (bitd & GREENMASK) >> 2;
    int bVal = (bitd & BLUEMASK) >> 0;

    int rActual = p.r & LOWEST2BITS;
    int gActual = p.g & LOWEST2BITS;
    int bActual = p.b & LOWEST2BITS;

    return (rVal == rActual) && (gVal == gActual) && (bVal == bActual);
}

/*******************************************
* IF YOU DECLARED ANY ADDITIONAL PRIVATE   *
* FUNCTIONS IN decoder-private.h, COMPLETE *
* THEIR IMPLEMENTATIONS BELOW              *
*******************************************/

