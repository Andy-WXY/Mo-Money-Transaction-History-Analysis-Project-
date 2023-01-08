#include <cassert>
#include <iomanip>
#include <iostream>
#include <cstring>

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
Transaction::Transaction(std::string ticker_symbol, unsigned int day_date,
                         unsigned int month_date, unsigned year_date,
                         bool buy_sell_trans, unsigned int number_shares,
                         double trans_amount) {
    symbol = ticker_symbol;
    day = day_date;
    month = month_date;
    year = year_date;
    p_next = nullptr;
    if (buy_sell_trans) {
        trans_type = "Buy";
    } else {
        trans_type = "Sell";
    }
    shares = number_shares;
    amount = trans_amount;
    trans_id = assigned_trans_id;
    ++assigned_trans_id;
    acb = 0;
    acb_per_share = 0;
    share_balance = 0;
    cgl = 0;
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
    if (this->year < other.get_year()) {
        return true;
    } else if (this->year == other.get_year() &&
               this->month < other.get_month()) {
        return true;
    } else if (this->year == other.get_year() &&
               this->month == other.get_month() &&
               this->day < other.get_day()) {
        return true;
    } else if (this->year == other.get_year() &&
               this->month == other.get_month() &&
               this->day == other.get_day() &&
               this->trans_id > other.get_trans_id()) {
        return true;
    }
    return false;
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

bool Transaction::get_trans_type() const { return (trans_type == "Buy") ? true : false; }

unsigned int Transaction::get_trans_id() const { return trans_id; }

Transaction *Transaction::get_next() { return p_next; }

// GIVEN
// Member functions to set values.
//
void Transaction::set_acb(double acb_value) { acb = acb_value; }

void Transaction::set_acb_per_share(double acb_share_value) { acb_per_share = acb_share_value; }

void Transaction::set_share_balance(unsigned int bal) { share_balance = bal; }

void Transaction::set_cgl(double value) { cgl = value; }

void Transaction::set_next(Transaction *p_new_next) { p_next = p_new_next; }

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

    if (get_trans_type()) {
        std::cout << "  Buy  ";
    }
    else {
        std::cout << "  Sell ";
    }

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
    if (p_head != nullptr) {
        Transaction *temp = p_head;
        while (p_head != nullptr) {
            p_head = p_head->get_next();
            delete temp;
            temp = p_head;
        }
    }
}

// read_history(...): Read the transaction history from file.
// TASK 4
//
void History::read_history() {
    ece150::open_file();
    while (ece150::next_trans_entry()) {
        insert(new Transaction(ece150::get_trans_symbol(),
                               ece150::get_trans_day(),
                               ece150::get_trans_month(),
                               ece150::get_trans_year(),
                               ece150::get_trans_type(),
                               ece150::get_trans_shares(),
                               ece150::get_trans_amount()));
    }
    ece150::close_file();

}

// insert(...): Insert transaction into linked list.
// TASK 5
//
void History::insert(Transaction *p_new_trans) {
    if (p_head == nullptr) {
        p_head = p_new_trans;
    } else {
        Transaction *temp = p_head;
        while (temp->get_next() != nullptr) {
            temp = temp->get_next();
        }
        temp->set_next(p_new_trans);
    }
}

// sort_by_date(): Sort the linked list by trade date.
// TASK 6
//
void History::sort_by_date() {
    int num_trans{0};
    Transaction *count = p_head;
    while (count != nullptr) {
        count = count->get_next();
        ++num_trans;
    }
    delete count;

    while (num_trans != 0) {
        Transaction *temp = p_head;
        Transaction *pre = p_head;
        for (int i = 0; i < num_trans - 1; ++i) {
            if (*temp->get_next() < *temp) {
                if (i == 0) {
                    p_head = temp->get_next();
                    temp->set_next(pre->get_next()->get_next());
                    p_head->set_next(temp);
                    pre = p_head;
                } else {
                    pre->set_next(temp->get_next());
                    temp->set_next(pre->get_next()->get_next());
                    pre->get_next()->set_next(temp);
                    pre = pre->get_next();
                }
            } else {
                pre = temp;
                temp = temp->get_next();
            }
        }
        --num_trans;
    }

//    if(!(p_head == nullptr || p_head->get_next() == nullptr)){
//        Transaction *ref = nullptr;
//        bool swap = true;
//        while(ref != p_head->get_next() && swap){
//            Transaction *check_loc = p_head;
//            swap = false;
//            for(; check_loc->get_next() && check_loc->get_next() != ref; check_loc = check_loc->get_next()){
//                if(*check_loc->get_next() < *check_loc){
//                    Transaction *temp = check_loc->get_next();
//                    check_loc->set_next(check_loc);
//                    check_loc = temp;
//                    swap = true;
//                }
//            }
//            ref = check_loc;
//        }
//    }



//    Transaction *sorted_head = nullptr;
//    while(p_head != nullptr){
//        Transaction *temp = p_head;
//        Transaction *pre = p_head;
//        Transaction *min = p_head;
//        while(temp->get_next() != nullptr){
//            if(*temp->get_next() < *min){
//                pre = temp;
//                min = temp->get_next();
//            }
//            temp = temp->get_next();
//        }
//        if(min = p_head){
//            p_head = min->get_next();
//        }
//        else{
//            pre->set_next(min->get_next());
//        }
//        if(sorted_head == nullptr){
//            sorted_head == min;
//        }
//        else{
//            Transaction *set = sorted_head;
//            while(set->get_next() != nullptr){
//                set = set->get_next();
//            }
//            set->set_next(min);
//        }
//    }
//    p_head = sorted_head;
//    sorted_head = nullptr;
}

// update_acb_cgl(): Updates the ACB and CGL values.
// TASK 7
//
void History::update_acb_cgl() {
    Transaction *temp = p_head;
    while (temp != nullptr){
        if (temp->get_trans_type()) {
            temp->set_acb(temp->get_acb() + temp->get_amount());
            temp->set_share_balance(temp->get_share_balance() + temp->get_shares());
            temp->set_acb_per_share(temp->get_acb() / temp->get_share_balance());
        }
        else {
            temp->set_acb(temp->get_acb() - (temp->get_shares() * temp->get_acb_per_share()));
            temp->set_share_balance(temp->get_share_balance() - temp->get_shares());
            temp->set_cgl(temp->get_amount() - (temp->get_shares() * temp->get_acb_per_share()));
        }

        if (temp->get_next() != nullptr) {
            temp->get_next()->set_acb(temp->get_acb());
            temp->get_next()->set_share_balance(temp->get_share_balance());
            temp->get_next()->set_acb_per_share(temp->get_acb_per_share());
        }
        temp = temp->get_next();
    }
}

// compute_cgl(): )Compute the ACB, and CGL.
// TASK 8
double History::compute_cgl(unsigned int year) {
    double cgl{0};
    Transaction *temp = p_head;
    while (temp != nullptr) {
        if (temp->get_year() == year) {
            cgl += temp->get_cgl();
        }
        temp = temp->get_next();
    }
    delete temp;
    temp = nullptr;
    return cgl;
}

// print() Print the transaction history.
// TASK 9
//
void History::print() {
    std::cout << "========== BEGIN TRANSACTION HISTORY ===========" << std::endl;
    Transaction *temp = p_head;
    while (temp != nullptr) {
        temp->print();
        temp = temp->get_next();
    }
    std::cout << "========== END TRANSACTION HISTORY ===========" << std::endl;
}

// GIVEN
// get_p_head(): Full access to the linked list.
//
Transaction *History::get_p_head() { return p_head; }
