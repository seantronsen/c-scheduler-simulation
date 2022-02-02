/*
 * FILE: structures.c
 * AUTHOR: Sean Tronsen
 * PURPOSE: contains useful data structure functions for the associated executable
 * DATE: December 3rd, 2021
 */
#include <stdlib.h>
#include <stdio.h>
#include "structures.h"

// pre-definition to calm the compiler
typedef struct SinglyLinkedList SinglyLinkedList;
typedef struct ListElementL ListElementL;

// this is a variation of the new_stack function originally created for Programming Assignment 2
singly_linked_list new_list() {
    singly_linked_list l = (singly_linked_list) malloc(sizeof(SinglyLinkedList));
    if (l == NULL) {
        perror("Memory could not be allocated. \n");
        exit(EXIT_FAILURE);
    }
    l->head = NULL;
    l->length = 0;
    return l;
}

// variation - see above comments
int is_empty(singly_linked_list l) {
    if (!l->head) return 1;
    return 0;
}

// used to append an element to the end of the specified list
void append(singly_linked_list l, void *item) {
    // update the length
    l->length = l->length + 1;
    list_element *element = (list_element *) malloc(sizeof(list_element)); // allocate space on heap

    // create a new element
    element->next = NULL;
    element->item = item;

    // add element to the list
    if (is_empty(l)) {
        l->head = element;
        return;
    }
    // if not empty, add to the end of the list
    list_element *temp_element = l->head;
    while (temp_element->next != NULL) {
        temp_element = temp_element->next;
    }
    temp_element->next = element;
}


// variation - see above comments
void *pop(singly_linked_list l) {
    if (is_empty(l)) return NULL;
    list_element *element = (l->head);
    l->head = element->next;
    void *item = element->item;
    free(element);
    l->length = l->length - 1;
    return item;
}






// PROCESS - COMMON

// SORT ARRIVAL TIME ASCENDING - BUBBLE SORT
// ASSUMES THAT A LIST OF PROCESSES HAS BEEN PASSED IN
void sort_list_of_processes(singly_linked_list l) {
    // check for empty list
    if (is_empty(l)) return;

    // outer flag for sort occurred during iteration
    int flag;
    do {
        flag = 0;
        // inner loop runs for length of entire list
        list_element *element = l->head;
        while (element->next != NULL) {

            // CHECK ELEMENT AND ELEMENT_NEXT
            process a = element->item;
            process b = element->next->item;

            if (a->arrival_time > b->arrival_time) {
                void *temp = element->next->item;
                element->next->item = element->item;
                element->item = temp;
                flag = 1;
            }
            element = element->next;
        }
    } while (flag);

}

// PRINT LIST CONTENTS
// ASSUMES LIST OF PROCESSES
void print_list_of_processes(singly_linked_list l) {
    printf("PRINTING LIST CONTENTS \n");
    if (is_empty(l)) {
        printf("EMPTY LIST\n");
        return;
    }

    // get first element and loop till next NULL is found
    list_element *element = l->head;
    do {
        process a = element->item;
        printf("ID: %5d \tARRIVAL TIME: %5d\tCPU TIME: %5d\n", a->id, a->arrival_time, a->cpu_time);
        element = element->next;
    } while (element != NULL);
}

// returns 1 for existing and zero for legal value
int check_for_existing_id(singly_linked_list l, int requested_id) {
    int flag = 0;
    list_element *element = l->head;
    while (element != NULL) {
        process p = element->item;
        if (p->id == requested_id) {
            flag = 1;
        }
        element = element->next;
    }
    return flag;
}

// outputs to the console if processes have an arrival time that matches the current time
void output_arrivals(singly_linked_list l, int time) {
    list_element *element = l->head;
    while (element != NULL) {
        process p = element->item;
        if (p->arrival_time == time && (p->is_running == 0)) {
            print_process_status(p, "ARRIVED", time);
        }
        element = element->next;
    }
}

// function used to update the wait values on processes
// updates are only done for processes that do not have the running status
void increment_wait_timers(singly_linked_list l) {
    list_element *element = l->head;
    while (element != NULL) {
        process p = element->item;
        if (!p->is_running) {
            p->wait_time = p->wait_time + 1;
        }
        element = element->next;
    }
}

// simple status printing function
void print_process_status(process p, char status[], int current_time) {
    printf("CURRENT TIME: %5d\tID: %5d\tCPU TIME: %5d\tSTATUS: %s\n", current_time, p->id, p->cpu_time, status);
}
