/**
 * @file tritree.cpp
 * @author CPSC 221
 * @description TriTree class implementation, CPSC 221 PA3
 *
 * This file must be submitted for grading.
 *
**/

#include "tritree.h"
#include "tritree-private.h"

TriTree::TriTree(PNG& imIn) {
	// REPLACE THE LINEs BELOW WITH YOUR CODE
	width = imIn.width();
	height = imIn.height();
	root = BuildNode(imIn, make_pair(0, 0), width, height);
}

void TriTree::Clear() {
	this->Clear(root);
    root = nullptr;
}

void TriTree::Copy(const TriTree& other) {
	width = other.width;
    height = other.height;
    root = Copy(other.root);
}

PNG TriTree::Render() const {
	PNG img(width, height);
    const_cast<TriTree*>(this)->Render(root, img);
    return img;
}

void TriTree::Transpose() {
	Transpose(root);
    int temp = width;
    width = height;
    height = temp;
}

void TriTree::Prune(double tol) {
	Prune(root, tol);
}

int TriTree::NumLeaves() const {
	return const_cast<TriTree*>(this)->NumLeaves(root);
}

Node* TriTree::BuildNode(PNG& im, pair<int, int> ul, int w, int h) {
    if (w == 0 || h == 0) return nullptr;

    Node *node = new Node(ul, w, h);

    if (w == 1 && h == 1) {
        node->avg = *im.getPixel(ul.first, ul.second);
        return node;
    }

    bool splitWide = (w >= h);
    int longSide = splitWide ? w : h;

    int partA = longSide / 3;
    int partB = longSide / 3;
    int partC = longSide / 3;

    if (longSide % 3 == 1) {
        partB++;
    } else if (longSide % 3 == 2) {
        partA++;
        partC++;
    }

    if (splitWide) {
        node->A = BuildNode(im, ul, partA, h);
        if (partB > 0) {
            node->B = BuildNode(im, make_pair(ul.first + partA, ul.second), partB, h);
        }
        if (partC > 0) {
            node->C = BuildNode(im, make_pair(ul.first + partA + partB, ul.second), partC, h);
        }
    } else {
        node->A = BuildNode(im, ul, w, partA);
        if (partB > 0) {
            node->B = BuildNode(im, make_pair(ul.first, ul.second + partA), w, partB);
        }
        if (partC > 0) {
            node->C = BuildNode(im, make_pair(ul.first, ul.second + partA + partB), w, partC);
        }
    }

    long long r = 0, g = 0, b = 0, a = 0;
    long long totalArea = 0;

    auto addChildAverage = [&](Node* child) {
        if (child) {
            long long area = child->width * child->height;
            r += child->avg.r * area;
            g += child->avg.g * area;
            b += child->avg.b * area;
            a += child->avg.a * area;
            totalArea += area;
        }
    };

    addChildAverage(node->A);
    addChildAverage(node->B);
    addChildAverage(node->C);

    if (totalArea > 0) {
        node->avg.r = r / totalArea;
        node->avg.g = g / totalArea;
        node->avg.b = b / totalArea;
        node->avg.a = a / totalArea;
    }

    return node;
}

/*==== ALSO IMPLEMENT ANY PRIVATE FUNCTIONS YOU HAVE DECLARED ====*/

void TriTree::Clear(Node *node) {
    if (!node) return;
    Clear(node->A);
    Clear(node->B);
    Clear(node->C);
    delete node;
}

Node* TriTree::Copy(Node *node) {
    if (!node) return nullptr;
    Node* newNode = new Node(node->upperleft, node->width, node->height);
    newNode->avg = node->avg;
    newNode->A = Copy(node->A);
    newNode->B = Copy(node->B);
    newNode->C = Copy(node->C);
    return newNode;
}

void TriTree::Render(Node *node, PNG& img) {
    if (!node) return;
    if (!node->A && !node->B && !node->C) {
        for (int x = node->upperleft.first; x < node->upperleft.first + node->width; x++) {
            for (int y = node->upperleft.second; y < node->upperleft.second + node->height; y++) {
                *img.getPixel(x, y) = node->avg;
            }
        }
        return;
    }
    Render(node->A, img);
    Render(node->B, img);
    Render(node->C, img);
}

void TriTree::Transpose(Node *node) {
    if (!node) return;

    int tempX = node->upperleft.first;
    node->upperleft.first = node->upperleft.second;
    node->upperleft.second = tempX;

    int tempW = node->width;
    node->width = node->height;
    node->height = tempW;

    Transpose(node->A);
    Transpose(node->B);
    Transpose(node->C);
}

bool TriTree::Prunable(Node *node, RGBAPixel targetAvg, double tol) {
    if (!node) return true;
    
    if (!node->A && !node->B && !node->C) {
        
        double rDist = (double)node->avg.r - (double)targetAvg.r;
        double gDist = (double)node->avg.g - (double)targetAvg.g;
        double bDist = (double)node->avg.b - (double)targetAvg.b;
        
        double distSq = (rDist * rDist) + (gDist * gDist) + (bDist * bDist);
        return distSq <= (tol * tol);
    }
    
    return Prunable(node->A, targetAvg, tol) && 
           Prunable(node->B, targetAvg, tol) && 
           Prunable(node->C, targetAvg, tol);
}

void TriTree::Prune(Node *node, double tol) {
    if (!node) return;
    if (!node->A && !node->B && !node->C) return; 
    
    if (Prunable(node, node->avg, tol)) {
        Clear(node->A);
        node->A = nullptr;
        Clear(node->B);
        node->B = nullptr;
        Clear(node->C);
        node->C = nullptr;
    } else {
        Prune(node->A, tol);
        Prune(node->B, tol);
        Prune(node->C, tol);
    }
}

int TriTree::NumLeaves(Node *node) {
    if (!node) return 0;
    if (!node->A && !node->B && !node->C) return 1;
    return NumLeaves(node->A) + NumLeaves(node->B) + NumLeaves(node->C); 
}
