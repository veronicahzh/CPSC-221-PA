/**
 * @file deque.cpp
 * @description Implementations for PA2, Deque class
 * @author (vhzh)
**/

#include "deque.h"

template <class T>
Deque<T>::Deque() {
    /* YOUR CODE HERE! */
    n1 = 0;
    n2 = 0;
}

/**
 * Adds the parameter object to the right of the Deque.
 *
 * @param newItem object to be added to the Deque.
**/
template <class T>
void Deque<T>::PushR(T newItem) {
    /* YOUR CODE HERE! */
    data.push_back(newItem);
    n2++; // increase data size
}

/**
 * Removes the object at the left of the Deque, and returns it to the
 * caller.
 *
 * See .h file for implementation notes. 
 *
 * @return The item that used to be at the left of the Deque.
**/
template <class T>
T Deque<T>::PopL() {
    /* YOUR CODE HERE! */
    T removed = data[n1];
    n1++; // new start of data vector
    n2--; // decrease count if data size
    if (IsEmpty()) n1 = 0; // reset start
    
    else if (n2 <= n1) {
        vector<T> newData;
        for (int i = n1; i < n1 + n2; i++) {
            newData.push_back(data[i]); // copy elements from data to newData
        }
        data = newData;
        n1 = 0; // reset start
    }

    return removed;
}
/**
 * Removes the object at the right of the Deque, and returns it to the
 * caller.
 *
 * @return The item that used to be at the right of the Deque.
**/
template <class T>
T Deque<T>::PopR() {
    /* YOUR CODE HERE! */
    T removed = data[n1 + n2 - 1];
    n2--;
    data.pop_back();
    if (IsEmpty()) n1 = 0;

    else if (n2 <= n1) {
        vector<T> newData;
        for (int i = n1; i < n1 + n2; i++) {
            newData.push_back(data[i]); // copy elements from data to newData
        }
        data = newData;
        n1 = 0; // reset start
    }
    
    return removed;
}

/**
 * Finds the object at the left of the Deque, and returns it to the
 * caller. Unlike PopL(), this operation does not alter the deque.
 *
 * @return The item at the front of the deque.
**/
template <class T>
T Deque<T>::PeekL() {
    /* YOUR CODE HERE! */
    T peeked = data[n1];

    return peeked;
}

/**
 * Finds the object at the right of the Deque, and returns it to the
 * caller. Unlike PopR(), this operation does not alter the deque.
 *
 * @return the value of The item at the right of the deque.
**/
template <class T>
T Deque<T>::PeekR() {
    /* YOUR CODE HERE! */
    T peeked = data[n1 + n2 - 1];

    return peeked;
}

/**
 * Determines if the Deque is empty.
 *
 * @return bool which is true if the Deque is empty, false otherwise.
**/
template <class T>
bool Deque<T>::IsEmpty() const {
    return n2 == 0;

}
