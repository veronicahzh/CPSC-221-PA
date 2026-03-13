/**
 * @file decoder.cpp
 * @description Implementations for PA2, Decoder class
 * @author (your CWLs)
**/

#include "decoder.h"
#include "queue.h"
#include "stack.h"
using namespace std;

Decoder::Decoder(const PNG & tm, pair<int, int> s) : start(s), mapImg(tm) {
    /* YOUR CODE HERE */
    
}

PNG Decoder::RenderSolution(){
    /* REPLACE THE LINE BELOW WITH YOUR CODE */
    return PNG();
}

PNG Decoder::RenderMaze(){
    /* REPLACE THE LINE BELOW WITH YOUR CODE */
    return PNG();
}

void Decoder::SetGrey(PNG& im, pair<int, int> loc){
    /* YOUR CODE HERE */
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
    /* REPLACE THE LINE BELOW WITH YOUR CODE */
    return -1;
}

bool Decoder::Good(vector<vector<bool>>& v, vector<vector<int>>& d, pair<int, int> curr, pair<int, int> next){
    /* REPLACE THE LINE BELOW WITH YOUR CODE */
    return false;
}

vector<pair<int, int>> Decoder::Neighbours(pair<int, int> curr) {
    /* REPLACE THE LINES BELOW WITH YOUR CODE */
    int dx = 1; 
	int dy = 1; 
	vector<pair<int,int>> neighbours = { {curr.first - dx, curr.second }, 
										 {curr.first, curr.second + dy },
										 {curr.first + dx, curr.second },
										 {curr.first, curr.second - dy }};
	return neighbours; 
}

bool Decoder::Compare(RGBAPixel p, int d){
    /* REPLACE THE LINE BELOW WITH YOUR CODE */
    return false;
}

/*******************************************
* IF YOU DECLARED ANY ADDITIONAL PRIVATE   *
* FUNCTIONS IN decoder-private.h, COMPLETE *
* THEIR IMPLEMENTATIONS BELOW              *
*******************************************/

