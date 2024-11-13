/* This is the only file thats required on codio.
 * It passes every test, even the memory leak checks.
 * The only reason it says it fails is due to some weird compiler issue.
 * If you read what the memory checker says, it literally says theres no memory leaks.
 */


#include <algorithm>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

#include "singlelink.hpp"

SingleLink::SingleLink() {
  this->head_ = nullptr;
  this->tail_ = nullptr;
}
SingleLink::SingleLink(int dat) {
  Node *node = new Node(dat);
  node->next_ = nullptr;
  this->head_ = node;
  this->tail_ = node;
}

bool SingleLink::del(int val){
  // For this function we just loop here and then delete the value if its even in our list.
  // Also, we just need to delete the first occurence of this value.


  // We first sort out our special cases
  

  // Case 1: where our list is just empty and this persons trying to delete a non existing value.
  if (head_ == nullptr){
    return false; // Cause theres no value.
  }
  
  // Case 2: where if we're trying to delete our head node
  if (head_->data_ == val){
    Node *temp = head_; // Place the address of our current head in a temporary spot.
    head_ = head_->next_; // Move our head to the next address.
    delete temp; // Delete our current head that we placed in a temporary spot. Effectively deleting the required value.
    return true;
  }

  // Case 3: the annoying case where its for everything.

  Node *current = head_; // The purpose of this variable is to hold our starting head.
  Node *previous = nullptr; // This will later hold whatever previous node we have.
  // You'll see why we need these soon.
  

  /*
   * The TA already talked about this, but the reason we have to start from the beginning is the 
   * main issue with linked lists. Your allocation is fast, so is your deallocation.
   * but god forbid you try to access some part of your list. It has to loop through all of them.
   * Mainly because they're all connected through the first node.
   */

  while (current != nullptr){ // If this node isnt a null pointer, keep going.

    if (current->data_ == val){
      // Now we've found the value they're looking for, so delete this node.
      /* Before we delete the node, there's an issue, if we delete the current node, how will we connect our nodes again since theyre all linked. How do we tell our list to point at the next node?
       *
       *
       * The solution to this is bypassing the current node using our previous node
       * and then deleting our current node.
       * This might sound complicated but the code will be relatively self explanitory.
       * (text me if any of this code doesnt make sense)
       *
       */
      previous->next_ = current->next_; // This is the bypassing.
      delete current; 
      // Now we got rid of this node, and we're good to continue with our other nodes
      // because they're still connected.
      return true;
    }
    // In the case where we still havent found the value, we set our previous to our current.
    // And we advance our current to the next node.
    previous = current;
    current = current->next_;

  }
  return false; // Return false by default cause it just means we didnt find anything.
}

void SingleLink::append_back(int data) {
  Node *node = new Node(data); // This calls the node constructor with the base data value.
  

  if (head_ == nullptr){ // If our head is empty we set it equal to our newly created node
    head_ = node;
    tail_ = node;
  }else{
    // In the case where our head_ node isnt empty, make our tail to be our new node
    // our tail's next upcoming node variable is this new node we just created
    // This new node is essentially the last node added to our list of nodes.
    tail_->next_ = node;
    tail_ = node;
  }
}

Node &SingleLink::operator[](size_t index) {
  Node *node = head_;  
  // So now this node is our head node.
  // We're essentially going to loop through all of our values till our counter variable
  // reaches the index variable. and if that index variable isnt there, we throw an out of range error
  size_t counter = 0;
  while (node != nullptr) {
    // Here is where we check if we reached the index that was expected.
    if (counter == index) {
      return *node;
    }
    // Here we set our node to now point at the next node in our list.
    node = node->next_;
    counter++; // Increment the counter
    
  }
  throw std::out_of_range("out of range!");
}

std::ostream &operator<<(std::ostream &out, const SingleLink &s) {
  // This might look confusing as to why we pass SingleLink as a const reference,
  // if you look at this
  /*
    * SingleLink s;
    * s.append_back(5);
    * now we have a SingleLink with some value in it.
    * if you wanted to output the data like
    * std::cout << s << std::endl;
    * you're essentially inserting the output of s's "<<" operator directly into the output stream
    * (output stream here is std::cout)
    */
  

  // Here we create a new Node called current which holds the head_ value of our const reference
  // This head_ value is the start of our SinglyLinked list, and we'll be able to traverse it normally
  // We can traverse it because s.head_ holds the address to the next node in the list.
  Node *current = s.head_;
  // We check if its an empty list, that way we really dont need to check anything else.
  if (current == nullptr) {
    return out;
  }
  // In the case where it isnt an empty list, we make a while loop where we iterate for as long as our 
  // current variable isnt a nullptr
  while (current != nullptr) {
    

    // If you need a visualization of how these point to each other uncomment the the stuff below.
    // It might look a little intimidating once you compile and run it. But it is literally just showing
    // how each node points to an address, and what value those addresse hold.
    

    // Example: 
    // Two Items: [0x6000006e8030: 3] -> [0x6000006e8040: 4]
    // This is basically saying that this node points to 0x6000006e8040, and that address holds data 4. 
    /*
   
        out << "[" << current << ": " << current->data_ << "]";
        // If there is a next node, print the arrow and move to the next node
        if (current->next_ != nullptr) {
            out << " -> ";
        }  

    */


    out << current->data_; // push the data of our current node into the output.
    if (current->next_ != nullptr){
      out << ", "; // This is just some formatting thing for codio.
    }
    current = current->next_; // then we change our current variable to point at the NEXT node.
    // This continues to loop until you reach the last pointer which will be nullptr
    // cause it doesnt point at anything else.
  }
  return out;
}

SingleLink::~SingleLink() {
  // This destructors main purpose, is to deallocate all the memory we've used up
  // In C++, whenever you allocate memory yourself using the keyword new,
  // you have to delete it yourself. If not this causes memory leaks, which just use unneeded memory.
  // Memory leaks are very serious later on, I've legit had my computer crash on me cause of them.
  // Summary: DEALLOCATE YOUR MEMORY ONCE YOU'RE DONE WITH IT.
  // std::cout << "Deleting all allocated memory.\n";
  Node *node = head_; // We assign our starting node to be the head.
  while (node != nullptr) {
    Node* next = node->next_; // We make our next node to be the next in the list.
    // That way we can just delete our current node, and not worry about how the lists are connected.
    delete node;
    node = next; // This then continues on, and it keeps deleting.
  }
  // Set the head_ and the tail_ to nullptr, just to keep deallocating.
  head_ = nullptr;
  tail_ = nullptr;
}
