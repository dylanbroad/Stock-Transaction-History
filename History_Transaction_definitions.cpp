#include <cassert>
#include <iomanip>
#include <iostream>

#include "project4.hpp"
#include "History.hpp"
#include "Transaction.hpp"

////////////////////////////////////////////////////////////////////////////////
// Definitions for Transaction class
////////////////////////////////////////////////////////////////////////////////
//
//

// Constructor
// TASK 1
//
Transaction::Transaction(std::string ticker_symbol, unsigned int day_date, unsigned int month_date, unsigned year_date, bool buy_sell_trans, unsigned int number_shares, double trans_amount){
symbol = ticker_symbol;
day = day_date;
month = month_date;
year = year_date;
if (buy_sell_trans) {
    trans_type = "Buy";
  }
  else {
    trans_type = "Sell";

  }
shares = number_shares;
amount = trans_amount;
trans_id = assigned_trans_id;
assigned_trans_id = assigned_trans_id + 1;
p_next = p_next;

}


// Destructor
// TASK 1
//
Transaction::~Transaction() {
}

// Overloaded < operator.
// TASK 2
//
bool Transaction::operator<(Transaction const &other) {
  if (Transaction::year < other.year) {
    return true;
  }
  else if (Transaction::year > other.year) {
    return false;
  }
  else if (Transaction::month < other.month) {
      return true;
  }
  else if (Transaction::month > other.month) {
      return false;
  }
  
  else if (Transaction::day < other.day) {
       return true;
  }
  else if (Transaction::day > other.day) {
      return false;
  }
  else if (Transaction::trans_id > other.trans_id) {
      return true;
  }
   else {
   return false;
        }
      }


// GIVEN
// Member functions to get values.
//
std::string Transaction::get_symbol() const { return symbol; }
unsigned int Transaction::get_day() const { return day; }
unsigned int Transaction::get_month() const { return month; }
unsigned int Transaction::get_year() const { return year; }
unsigned int Transaction::get_shares() const { return shares; }
double Transaction::get_amount() const { return amount; }
double Transaction::get_acb() const { return acb; }
double Transaction::get_acb_per_share() const { return acb_per_share; }
unsigned int Transaction::get_share_balance() const { return share_balance; }
double Transaction::get_cgl() const { return cgl; }
bool Transaction::get_trans_type() const { return (trans_type == "Buy") ? true: false ; }
unsigned int Transaction::get_trans_id() const { return trans_id; }
Transaction *Transaction::get_next() { return p_next; }

// GIVEN
// Member functions to set values.
//
void Transaction::set_acb( double acb_value ) { acb = acb_value; }
void Transaction::set_acb_per_share( double acb_share_value ) { acb_per_share = acb_share_value; }
void Transaction::set_share_balance( unsigned int bal ) { share_balance = bal ; }
void Transaction::set_cgl( double value ) { cgl = value; }
void Transaction::set_next( Transaction *p_new_next ) { p_next = p_new_next; }

// GIVEN
// Print the transaction.
//
void Transaction::print() {
  std::cout << std::fixed << std::setprecision(2);
  std::cout << std::setw(4) << get_trans_id() << " "
    << std::setw(4) << get_symbol() << " "
    << std::setw(4) << get_day() << " "
    << std::setw(4) << get_month() << " "
    << std::setw(4) << get_year() << " ";


  if ( get_trans_type() ) {
    std::cout << "  Buy  ";
  } else { std::cout << "  Sell "; }

  std::cout << std::setw(4) << get_shares() << " "
    << std::setw(10) << get_amount() << " "
    << std::setw(10) << get_acb() << " " << std::setw(4) << get_share_balance() << " "
    << std::setw(10) << std::setprecision(3) << get_acb_per_share() << " "
    << std::setw(10) << std::setprecision(3) << get_cgl()
    << std::endl;
}


////////////////////////////////////////////////////////////////////////////////
// Definitions for the History class
////////////////////////////////////////////////////////////////////////////////
//
//


// Constructor
// TASK 3
//

History::History() {
    p_head = nullptr;
    
}

// Destructor
// TASK 3
//
    History::~History() { 
      //set the head node as the current node
        Transaction *currNode {p_head};
      // create a loop to test if the current node is equal to null pointer
        while(currNode != nullptr){
      // if save the address of the next node 
          Transaction *nextNode {currNode->get_next()};
      //delete the current node
          delete currNode;
      // set the current node to nullptr
          currNode = nullptr;
      // set current node to nextNode so the while loop can repeat
          currNode = nextNode;
        }
        
    }



// read_history(...): Read the transaction history from file.
// TASK 4
//
void History::read_history() {
  //open file
  ece150::open_file();
  //loop through transactions and create linked list
  while (ece150::next_trans_entry() == true) {
     Transaction *current_trans = new Transaction{ece150::get_trans_symbol(), ece150::get_trans_day(), ece150::get_trans_month(), ece150::get_trans_year(), ece150::get_trans_type(), ece150::get_trans_shares(), ece150::get_trans_amount()};
    //call insert function
     insert(current_trans);
  }

  ece150::close_file();

}
// insert(...): Insert transaction into linked list.
// TASK 5
//
void History::insert( Transaction *p_new_trans ) {
  //check if p_head is empty
  if (p_head == nullptr) {
      p_head = p_new_trans;
  }

  else {
      //track id of argument
      unsigned int new_trans_id {p_new_trans->get_trans_id()};
      // Create a transaction object to alter the last node
      Transaction *l_node{p_head};
      

      //loop through linked list to reach last node
      for (int i = p_head->get_trans_id(); i < new_trans_id - 1; ++i) {
        l_node = l_node->get_next();
      }
      //set pointer of l_node to argument
      l_node->set_next(p_new_trans);
  }
}

// sort_by_date(): Sort the linked list by trade date.
// TASK 6
//
void History::sort_by_date() {
  Transaction *sorted_list = nullptr;
        Transaction *cur = p_head;
        while (!(cur == nullptr)) {
            Transaction *next = cur->get_next();
            if (sorted_list == nullptr || *cur < *sorted_list) {
              cur->set_next(sorted_list);
              sorted_list = cur;
            } else {
              Transaction *temp = sorted_list;
              while (temp->get_next() != nullptr && *temp->get_next() < *cur) {
                  temp = temp->get_next();
              }
              cur->set_next(temp->get_next());
              temp->set_next(cur);
          }
          cur = next;
        }
        p_head = sorted_list;

 
}


// update_acb_cgl(): Updates the ACB and CGL values.
// TASK 7
//
void History::update_acb_cgl() {
   Transaction *head_node = p_head;
   double acb{};
   double acb_per_share{};
   unsigned int share_bal{};
   double new_cgl{};
   while ( head_node != nullptr ) {
        if (head_node->get_trans_type()) 
        {
            acb += head_node->get_amount();
        }
        else
        {
            acb -= head_node->get_shares()*acb_per_share;
        }
        if (head_node->get_trans_type())
        {
            share_bal += head_node->get_shares();
        }
        else
        {
            share_bal -= head_node->get_shares();
        }
        head_node->set_share_balance(share_bal);

        head_node->set_acb(acb);
        if (head_node->get_trans_type()) 
        {
            if (head_node->get_share_balance() != 0)
            {
                acb_per_share = head_node->get_acb() / head_node->get_share_balance();
            }
            else
            {
                acb_per_share = 0;
            }
        }
        head_node->set_acb_per_share(acb_per_share);


        // Calculating CGL
        if (!(head_node->get_trans_type()))
        {
           new_cgl = (head_node->get_amount()-(head_node->get_shares()*head_node->get_acb_per_share()));
           head_node->set_cgl(new_cgl);
        }
        head_node = head_node->get_next(); 
    }

    head_node = nullptr;
    return;

    }

   


// compute_cgl(): )Compute the ACB, and CGL.
// TASK 8
double History::compute_cgl( unsigned int year ) {
  Transaction *new_node = p_head;
  double compute_cgl = 0.0;
  while (new_node != nullptr){
    if (new_node->get_year() == year){
      compute_cgl += new_node->get_cgl();
    }
    new_node = new_node->get_next();
  }
  return compute_cgl;
} 



// print() Print the transaction history.
//TASK 9
//
void History::print() {
  std::cout << "========== BEGIN TRANSACTION HISTORY ============" << std::endl;
  Transaction *new_node = p_head;
  
   while (new_node != nullptr)
    {
        new_node->print();
        new_node = new_node->get_next();
    }
  std::cout <<"========== END TRANSACTION HISTORY ============" << std::endl;

}


// GIVEN
// get_p_head(): Full access to the linked list.
//
Transaction *History::get_p_head() { return p_head; }
