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
    Node *curr = NW;
    int scaledDim = curr->data.Dimension() * scale;
    PNG img = PNG(columns_ * scaledDim, rows_ * scaledDim);

    if (roworder) {
        for (int row = 0; row < rows_; row++) {
            for (int col = 0; col < columns_; col++) {
                curr->data.Render(img, col * scaledDim, row * scaledDim, scale);
                curr = curr->next;
            }
        }
    } else {
        for (int col = 0; col < columns_; col++) {
            for (int row = 0; row < rows_; row++) {
                curr->data.Render(img, col * scaledDim, row * scaledDim, scale);
                curr = curr->next;
            }
        }
    }
    return img;
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

        for (int row = 0; row < v.size() - 1; row++) {
            if (row + rows_ >= v.size()) {
                int back = row + rows_ - v.size() + 1;
                v[row]->next = v[back];
            } else {
                v[row]->next = v[row+rows_];
            }
        }

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

        for (int col = 0; col < v.size() - 1; col++) {
            if (col + columns_ >= v.size()) {
                int back = col + columns_ - v.size() + 1;
                v[col]->next = v[back];
            } else {
                v[col]->next = v[col+columns_];
            }
        }

        roworder = false;
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
    int oldRows = rows_;
    rows_ = columns_;
    columns_ = oldRows;

    Node *curr = NW;
    while(curr != nullptr) {
        curr->data.Transpose();
        curr = curr->next;
    }

    roworder = !roworder;
}

/**************************************************
* IF YOU HAVE DECLARED PRIVATE FUNCTIONS IN       *
* chain-private.h, COMPLETE THEIR IMPLEMENTATIONS *
* HERE                                            *
**************************************************/

