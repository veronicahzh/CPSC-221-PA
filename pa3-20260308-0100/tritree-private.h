/**
 * @file tritree-private.h
 * @author (Your CWLs here)
 * @description TriTree private function declarations, CPSC 221 PA3
 *
 * This file must be submitted for grading,
 * even if you do not add anything to it.
 *
 * Declare your functions here as if they are written in-line
 * starting from line 220 of tritree.h
**/

void Clear(Node* node);
Node* Copy(Node* node);
void Render(Node* node, PNG& img);
void Transpose(Node* node);
bool Prunable(Node* node, RGBAPixel targetAvg, double tol);
void Prune(Node* node, double tol);
int NumLeaves(Node* node);

