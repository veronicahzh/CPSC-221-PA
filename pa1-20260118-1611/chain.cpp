/**
 * @file chain.cpp
 * @description Student implementation of Chain functions, CPSC 221 PA1
 * @author (your CWLs here)
**/

#include "chain.h"
#include <cmath>
#include <iostream>


// PA1 functions
// Complete all of the missing implementation
// and submit this file for grading.

/* Most useful constructor. Most easily implemented using your
 * implementation of Block.Build and Chain.InsertAfter.
 * Builds a chain out of the blocks in the
 * given image in rows. The blocks we create
 * have width equal to imIn.width()/numCols
 * and height equal to imIn.height() / block width.
 *
 * @param imIn     The image providing the blocks
 * @param numCols  The number of block columns into which the image will be divided
 * @pre   imIn's width is exactly divisible by numCols
 * @pre   imIn's height will be exactly divisible by the resulting block dimension
**/
Chain::Chain(PNG& imIn, int numCols) {
    NW = nullptr;
    Node* tail = nullptr;

    columns_ = numCols;
    int blockDim = imIn.width() / columns_;
    rows_ = imIn.height() / blockDim;

    roworder = true;

    // build row by row
    for (int row = 0; row < rows_; row++) {
        for (int col = 0; col < columns_; col++) {
            int x = col * blockDim;
            int y = row * blockDim;

            Block b;
            b.Build(imIn, x, y, blockDim);

            tail = InsertAfter(tail, b);
        }
    }

}

/**
 * Destroys the current Chain. This function should ensure that
 * memory does not leak on destruction of a chain.
**/
Chain::~Chain() {
	Clear();
}

/**
 * Inserts a new node after the node pointed to by p in the
 * chain (so p->next is the new node) and returns a pointer to
 * the newly created node.
 * If p is NULL, inserts a new head node to the chain.
 *
 * @param p = The new node should be pointed to by p->next.
 *            If p is NULL, the new node becomes the head of the chain.
 * @param ndata = The data to be inserted.
**/
Node* Chain::InsertAfter(Node* p, const Block &ndata) {
    Node *newNode = new Node(ndata);

	if (p == nullptr) {
        newNode->next = NW;
        NW = newNode;
    } else {
        newNode->next = p->next;
        p->next = newNode;
    }
	return newNode;
}

/**
 * Destroys all dynamically allocated memory associated with the
 * current Chain class.
**/
void Chain::Clear() {
	Node *curr = NW;
    while (curr != nullptr) {
        Node *next = curr->next;
        delete curr;
        curr = next;
    }

    NW = nullptr;
    rows_ = 0;
    columns_ = 0;
}

/**
 * Makes the current object into a copy of the parameter:
 * All member variables should have the same value as
 * those of other, but the memory should be completely
 * independent. This function is used in both the copy
 * constructor and the assignment operator for Chains.
**/
void Chain::Copy(Chain const &other) {
	rows_ = other.rows_;
    columns_ = other.columns_;
    roworder = other.roworder;

    if (other.NW == nullptr) {
        NW = nullptr;
        return;
    }

    NW = new Node(other.NW->data);

    Node *currThis = NW;
    Node *currOther = other.NW->next;

    while (currOther != nullptr) {
        currThis->next = new Node(currOther->data);
        currThis = currThis->next;
        currOther = currOther->next;
    }
}

/**
 * Takes the current chain and renders it into a
 * correctly sized PNG. The blocks in the chain
 * are placed in the image according to their row/column positions in the chain.
 * The output image will be enlarged using nearest-neighbour scaling
 * (i.e. no pixel blending).
 * @pre scale >= 1
**/
PNG Chain::Render(int scale) {
    if (NW == nullptr) {
        return PNG();
    }
    int blockDim = NW->data.Dimension();
    int scaleW = columns_ * blockDim * scale;
    int scaleH = rows_ * blockDim * scale;

    PNG scalePNG(scaleW, scaleH);

    Node *curr = NW;
    int index = 0; // which node are we on

    while (curr != nullptr) {
        int rowIndex;
        int colIndex;

        if (roworder) {
            rowIndex = index / columns_;
            colIndex = index % columns_;
        } else {
            rowIndex = index % rows_;
            colIndex = index / rows_;
        }

        int x = colIndex * blockDim * scale;
        int y = rowIndex * blockDim * scale;

        curr->data.Render(scalePNG, x, y, scale);
        curr = curr->next;
        index++;
    }

    return scalePNG;
}

/**
 * Rearranges the links in the list so that each node's
 * next pointer moves horizontally across an image, respecting
 * the image's number of columns and rows.
 * 
 * Has no effect on a list which is already in row order.
**/
void Chain::ToRowOrder() {
    if (!roworder) {
        Node *curr = NW;
        vector<Node*> v;

        while(curr != nullptr) {
            v.push_back(curr);
            curr = curr->next;
        }

        vector<Node*> rowV(v.size());

        for (int i = 0; i < v.size(); i++) {
            int row = i % rows_;
            int col = i / rows_;
            int rowIndex = row * columns_ + col;
            rowV[rowIndex] = v[i];
        }

        for (int i = 0; i < rowV.size() - 1; i++) {
            rowV[i]->next = rowV[i + 1];
        }
        rowV.back()->next = nullptr;
        NW= rowV[0];
        roworder = true;
    }

}

/**
 * Rearranges the links in the list so that each node's
 * next pointer moves vertically across an image, respecting
 * the image's number of columns and rows.
 *
 * Has no effect on a list which is already in column order.
**/
void Chain::ToColumnOrder() {
     if (roworder) {
        Node *curr = NW;
        vector<Node*> v;

        while(curr != nullptr) {
            v.push_back(curr);
            curr = curr->next;
        }

        vector<Node*> colV(v.size());

        for (int i = 0; i < v.size(); i++) {
            int col = i % columns_;
            int row = i / columns_;
            int colIndex = col * rows_ + row;
            colV[colIndex] = v[i];
        }

        for (int i = 0; i < colV.size() - 1; i++) {
            colV[i]->next = colV[i + 1];
        }
        colV.back()->next = nullptr;
        NW= colV[0];
        roworder = true;
    }
}

/**
 * Rearranges the links in the list and each node's Block data
 * so that the rendered image appears transposed over the NW-SE diagonal.
 * 
 * e.g.
 * 
 *  A -> B                     A     C  (individual blocks must also be transposed)
 *      /    --Transpose-->    |   / |
 *    /                        v /   v
 *  C -> D                     B     D
**/
void Chain::Transpose() {
    /* your code here */

}

/**************************************************
* IF YOU HAVE DECLARED PRIVATE FUNCTIONS IN       *
* chain-private.h, COMPLETE THEIR IMPLEMENTATIONS *
* HERE                                            *
**************************************************/

