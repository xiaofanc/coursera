/* note for linked-memory */

// linked list 
// - a head pointer stores the beginning of the linked list
// - a pointer to nullptr marks the end
// - runnnig time is the size of linked list O(n), while array is O(1)
// - no fixed capacity
// - store same type value

// create a templated class for ListNode
template <typename T> // in order to store integer, string, cube...
class ListNode {
  public:
    T & data;
    ListNode *next;  // next pointer
    // create a ListNode constructor that takes in data and 
    // it includes data data and a next to null
    ListNode(const T & data) : data(data), next(nullptr) { }
};

// list.h
#pragma once

template <typename T>
class List {
  public:
    // ampersand operatoe means we can access a list 
    // if we have a list L, L[0] will call this function
    const T & operator[](unsigned index);
    void insertAtFront(const T & data);

  // in the list class internally contains ListNode
  private:
    class ListNode {
      public:
        const T & data;
        ListNode *next;
        ListNode(const T & data) : data(data), next(nullptr) { }
    };
  
  // list class has the head pointer
  ListNode *head_;   /*< Head pointer for our List */
    
};

// list.cpp
#include "List.h"
// List::get
template <typename T>
// retrive an index until loop
const T & List<T>::operator[](unsigned index) {
  // Start a `thru` pointer to advance thru the list:
  // initialize thru pointer as head
  ListNode *thru = head_;

  // Loop until the end of the list (or until a `nullptr`):
  while (index > 0 && thru->next != nullptr) {
    thru = thru->next;
    //decrement index !!! 
    index--;
  }  

  // Return the data:
  // access the ListNode data element
  return thru->data;
}

// List::insert
// add an element into the front of the linked list
// head_ -> new data -> head_.next - >...
template <typename T>
void List<T>::insertAtFront(const T & data) {
  // Create a new ListNode on the heap:
  ListNode *node = new ListNode(data);

  // Set the new node’s next pointer point the current
  // head of the List:
  // new node's next pointer points to whatever the head points to!!!!
  // why not *head_??
  node->next = head_;

  // Set the List’s head pointer to be the new node:
  // head points to new element
  head_ = node;
}







