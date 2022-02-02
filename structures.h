/*
 * FILE: structures.h
 * AUTHOR: Sean Tronsen
 * PURPOSE: contains program structures and headers for the associated source file
 * DATE: December 3rd, 2021
 *
 */

#ifndef TRONSEN_SCHEDULER_STRUCTURES_H
#define TRONSEN_SCHEDULER_STRUCTURES_H

// structure used to imitate a running process - member vars used for both simulation and calculations
struct Process {
    int id;
    int arrival_time;
    int cpu_time;
    int wait_time;
    int init_response_time;
    int is_running; // boolean style
    int has_run; // boolean style
};


typedef struct Process *process;


/* note this is reuse of the stack element assignment with a general rename */
struct ListElement {
    void *item;
    struct ListElement *next;
};

typedef struct ListElement list_element;

// this is another idea taken from the original stack element assignment
struct SinglyLinkedList {
    list_element *head;
    int length;
};


typedef struct SinglyLinkedList *singly_linked_list; // the list is a pointer


singly_linked_list new_list();

int is_empty(singly_linked_list l);

void append(singly_linked_list l, void *item);

void *pop(singly_linked_list l);


void sort_list_of_processes(singly_linked_list l);

void print_list_of_processes(singly_linked_list l);

int check_for_existing_id(singly_linked_list l, int requested_id);

void print_process_status(process p, char status[], int current_time);

void output_arrivals(singly_linked_list l, int time);

void increment_wait_timers(singly_linked_list l);


#endif //TRONSEN_SCHEDULER_STRUCTURES_H
