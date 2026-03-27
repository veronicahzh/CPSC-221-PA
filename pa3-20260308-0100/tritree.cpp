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
	Clear(root);
    root = nullptr;
}

void TriTree::Copy(const TriTree& other) {
	width = other.width;
    height = other.height;
    root = Copy(other.root);
}

PNG TriTree::Render() const {
	PNG img(width, height);
    Render(root, img);
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
	return NumLeaves(root);
}

Node* TriTree::BuildNode(PNG& im, pair<int, int> ul, int w, int h) {
    if (w == 0 || h == 0) return nullptr;

    Node *node = new Node(ul, w, h);

    // Leaf node base case
    if (w == 1 && h == 1) {
        node->avg = *im.getPixel(ul.first, ul.second);
        return node;
    }

    if (w >= h) {
        int wA = w / 3;
        int wB = w / 3;
        int wC = w / 3;
        
        if (w % 3 == 1) {
            wB++;
        } else if (w % 3 == 2) {
            wA++;
            wC++;
        }

        node->A = BuildNode(im, ul, wA, h);
        if (wB > 0) node->B = BuildNode(im, make_pair(ul.first + wA, ul.second), wB, h);
        if (wC > 0) node->C = BuildNode(im, make_pair(ul.first + wA + wB, ul.second), wC, h);
    } else {
        int hA = h / 3;
        int hB = h / 3;
        int hC = h / 3;
        
        if (h % 3 == 1) {
            hB++;
        } else if (h % 3 == 2) {
            hA++;
            hC++;
        }
        
        node->A = BuildNode(im, ul, w, hA);
        if (hB > 0) node->B = BuildNode(im, make_pair(ul.first, ul.second + hA), w, hB);
        if (hC > 0) node->C = BuildNode(im, make_pair(ul.first, ul.second + hA + hB), w, hC);
    }

    // Fix 2: Calculate the average color for this internal node based on its children's areas
    long long r = 0, g = 0, b = 0, a = 0;
    long long totalArea = 0;

    auto accumulateChild = [&](Node* child) {
        if (child) {
            long long area = child->width * child->height;
            r += child->avg.r * area;
            g += child->avg.g * area;
            b += child->avg.b * area;
            a += child->avg.a * area;
            totalArea += area;
        }
    };

    accumulateChild(node->A);
    accumulateChild(node->B);
    accumulateChild(node->C);

    if (totalArea > 0) {
        node->avg.r = r / totalArea;
        node->avg.g = g / totalArea;
        node->avg.b = b / totalArea;
        node->avg.a = a / totalArea;
    }

    return node;
}

/*==== ALSO IMPLEMENT ANY PRIVATE FUNCTIONS YOU HAVE DECLARED ====*/

void Clear(Node *node) {
    if (!node) return;
    Clear(node->A);
    Clear(node->B);
    Clear(node->C);
    delete node;
}

Node* Copy(Node *node) {
    if (!node) return nullptr;
    Node* newNode = new Node(node->upperleft, node->width, node->height);
    newNode->avg = node->avg;
    newNode->A = Copy(node->A);
    newNode->B = Copy(node->B);
    newNode->C = Copy(node->C);
    return newNode;
}

void Render(Node *node, PNG& img) {
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

void Transpose(Node *node) {
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

// Fix 3: The second recursive helper for checking if a subtree is prunable
bool Prunable(Node* node, RGBAPixel targetAvg, double tol) {
    if (!node) return true;
    
    // If it's a leaf node, check if its color distance is within tolerance
    if (!node->A && !node->B && !node->C) {
        // Cast to double to prevent unsigned char underflow when subtracting
        double rDist = (double)node->avg.r - (double)targetAvg.r;
        double gDist = (double)node->avg.g - (double)targetAvg.g;
        double bDist = (double)node->avg.b - (double)targetAvg.b;
        
        double distSq = (rDist * rDist) + (gDist * gDist) + (bDist * bDist);
        return distSq <= (tol * tol);
    }
    
    // Otherwise, all children must be prunable
    return Prunable(node->A, targetAvg, tol) && 
           Prunable(node->B, targetAvg, tol) && 
           Prunable(node->C, targetAvg, tol);
}

void Prune(Node *node, double tol) {
    if (!node) return;
    if (!node->A && !node->B && !node->C) return; // leaf

    // Use our new helper to check if this entire subtree can be pruned
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

int NumLeaves(Node *node) {
    if (!node) return 0;
    if (!node->A && !node->B && !node->C) return 1;
    return NumLeaves(node->A) + NumLeaves(node->B) + NumLeaves(node->C);
}