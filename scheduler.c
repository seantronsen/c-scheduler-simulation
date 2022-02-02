/*
 * FILE: scheduler.c
 * AUTHOR: Sean Tronsen
 * PURPOSE: Simulate a simple CPU scheduler
 * DATE: December 3rd, 2021
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "structures.h"


/* PRE-DEFINITIONS FOR THE COMPILER */
typedef struct Process Process;

void sim_fcfs(singly_linked_list list_of_processes);

void sim_rr(singly_linked_list list_of_processes, int quantum);


// main program entry point
int main(int argc, char *argv[]) {

    // output and arg variables defined.
    printf("CPU SCHEDULER SIMULATOR\n");
    char algorithm_choice[4];
    int time_quantum;


    // testing for errors in the user args
    if (argc < 2 || argc > 3) {
        printf("INVALID NUMBER OF ARGUMENTS. \n");
        printf("Please use the program as follows...\n");
        printf("%s <scheduler_type [FCFS | RR]> <[optional_quantum_as_int]>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    // ibid
    if (strcmp("RR", argv[1]) == 0) {
        if (argc != 3) {
            printf("INVALID ARGUMENTS. \n");
            printf("RR ALGORITHM REQUIRES TIME QUANTUM...\n");
            printf("%s <scheduler_type [FCFS | RR]> <[optional_quantum_as_int]>\n", argv[0]);
            exit(EXIT_FAILURE);
        }
        time_quantum = atoi(argv[2]);
        if (!time_quantum) {
            printf("INVALID ARGUMENTS. \n");
            printf("TIME QUANTUM MUST BE AN UNSIGNED INT TYPE GREATER THAN 0...\n");
            printf("%s <scheduler_type [FCFS | RR]> <[optional_quantum_as_int]>\n", argv[0]);
            exit(EXIT_FAILURE);
        }
        printf("ROUND ROBIN SELECTED WITH QUANTUM OF %d\n", time_quantum);
        strcpy(algorithm_choice, "RR");
    } else if (strcmp("FCFS", argv[1]) == 0) {
        printf("FIRST COME FIRST SERVE SELECTED\n");
        strcpy(algorithm_choice, "FCFS");
    } else {
        printf("INVALID ARGUMENTS. \n");
        printf("%s <scheduler_type [FCFS | RR]> <[optional_quantum_as_int]>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // user input helper message
    printf("Enter processes in the format: ID ARRIVAL_TIME CPU_TIME\n");

    // creation/allocation of the storage structure
    singly_linked_list list_of_processes = new_list();

    // input holder vars
    int input_id, input_arrival_time, input_cpu_time;

    // looping flag for when to stop gathering input
    int input_flag = 1;
    do {

        // get the three ints specified by the user
        if (scanf("%d %d %d", &input_id, &input_arrival_time, &input_cpu_time) != 3) {
            printf("Invalid input detected. Please enter three integer values using the specified format.\n");
            continue;
        }
        getchar(); // remove trailing newline

        // test for user requesting simulation start
        if (input_id == 0 && input_arrival_time == 0 && input_cpu_time == 0) input_flag = 0;

            // test for improper input
        else if (input_id < 1 || input_arrival_time < 0 || input_cpu_time < 1) {
            printf("Invalid input detected. Please enter three integer values using the specified format.\n");
            printf("Also note that the ID and cpu_time must be greater than 0 while the arrival_time can be"
                   "\ngreater than or equal to zero\n");
            continue;
        }
            // ibid
        else if (check_for_existing_id(list_of_processes, input_id)) {
            printf("The requested ID value has already been used. Please enter unique values only.\n");
            continue;
        }
            // ibid
        else {
            process input_process = (process) malloc(sizeof(Process));
            input_process->id = input_id;
            input_process->arrival_time = input_arrival_time;
            input_process->cpu_time = input_cpu_time;
            input_process->wait_time = 0;
            input_process->init_response_time = 0;
            input_process->is_running = 0;
            input_process->has_run = 0;
            append(list_of_processes, input_process);
        }
    } while (input_flag);


    // alert message and output of the entered processes
    printf("SUBMITTED PROCESSES\n");
    print_list_of_processes(list_of_processes);

    // branch based on algorithm selected and provide the necessary args
    if (!strcmp("FCFS", algorithm_choice)) {
        sim_fcfs(list_of_processes);
    }
    if (!strcmp("RR", algorithm_choice)) {
        sim_rr(list_of_processes, time_quantum);
    }

    // successful execution exit
    exit(EXIT_SUCCESS);

}

// fcfs simulator
// assumes that the passed in list has at least one element - must be of the process type
void sim_fcfs(singly_linked_list list_of_processes) {
    printf("SIMULATING FCFS\n");
    printf("--------------------------------------------------------------------\n");

    // counter variables simulate clock and used for later calculations
    int time_counter = 0;
    int cpu_running_time_counter = 0;
    int total_turnaround_time = 0;
    int total_waiting_time = 0;
    int total_response_time = 0;
    const int total_processes = list_of_processes->length; // init set - not used again until calculations

    // sort the list by arrival time to enact first come first serve
    sort_list_of_processes(list_of_processes);
    // output initial arrivals if any exist
    output_arrivals(list_of_processes, time_counter);

    // loop until the structure is empty
    while (!is_empty(list_of_processes)) {

        // obtain a process to run
        process current_process = pop(list_of_processes);

        // idle if the arrival time exceeds the current time
        while (current_process->arrival_time > time_counter) {
            time_counter++;
            printf("CURRENT TIME: %5d\t CPU IDLING\n", time_counter);
            output_arrivals(list_of_processes, time_counter);

        }

        // set that process to running and store the first response time
        current_process->is_running = 1;
        current_process->init_response_time = time_counter;

        // update values while the process runs for its total cpu time
        while (current_process->cpu_time != 0) {
            // increment the counters and storage values
            time_counter++;
            cpu_running_time_counter++;
            current_process->cpu_time = current_process->cpu_time - 1;
            increment_wait_timers(list_of_processes);
            // check for new arrivals
            output_arrivals(list_of_processes, time_counter);
            // output status
            print_process_status(current_process, "RUNNING", time_counter);
        }
        // alert the user that the process has completed
        print_process_status(current_process, "COMPLETED", time_counter);
        // update the totals for calculations
        total_turnaround_time += time_counter; // add process turn around time to counter
        total_response_time += current_process->init_response_time;
        total_waiting_time += current_process->wait_time;
        free(current_process); // deallocate the process data
    }

    // output the calculation results
    printf("SIMULATION COMPLETED\n");
    printf("AVERAGE WAITING TIME: %f\n", ((double) total_waiting_time / total_processes));
    printf("AVERAGE RESPONSE TIME: %f\n", ((double) total_response_time / total_processes));
    printf("AVERAGE TURNAROUND TIME: %f\n", ((double) total_turnaround_time / total_processes));
    printf("AVERAGE CPU USAGE: %f PERCENT\n", (((double) cpu_running_time_counter / time_counter) * 100));
}


// function that simulates round robin with a quantum
// assumes a non-empty list is passed as an argument where the elements of which are of the process type

// the majority of the processing is the same as FCFS, KEY differences will be commented
void sim_rr(singly_linked_list list_of_processes, int quantum) {

    printf("SIMULATING RR\n");
    printf("--------------------------------------------------------------------\n");

    // init counters - same idea as above
    int time_counter = 0;
    int cpu_running_time_counter = 0;
    int total_turnaround_time = 0;
    int total_waiting_time = 0;
    int total_response_time = 0;
    const int total_processes = list_of_processes->length; // init set - not used again until calculations

    // sort because round-robin processing is similar to FCFS in terms of initial ordering requirements
    sort_list_of_processes(list_of_processes);
    output_arrivals(list_of_processes, time_counter);

    // ibid
    while (!is_empty(list_of_processes)) {
        process current_process = pop(list_of_processes);
        while (current_process->arrival_time > time_counter) {
            time_counter++;
            printf("CURRENT TIME: %5d\t CPU IDLING\n", time_counter);
            output_arrivals(list_of_processes, time_counter);

        }
        // set current process is running and has run if it's the first run
        // this is to mark whether init response time should be given a value
        current_process->is_running = 1;
        if (current_process->has_run == 0) {
            current_process->init_response_time = time_counter;
            current_process->has_run = 1;
        }

        // run until either the cpu time has completed or the quantum time has been reached
        // otherwise same processing as FCFS
        int quantum_counter = 0;
        while (current_process->cpu_time != 0 && quantum_counter < quantum) {
            quantum_counter++;
            time_counter++;
            cpu_running_time_counter++;
            current_process->cpu_time = current_process->cpu_time - 1;
            increment_wait_timers(list_of_processes);
            output_arrivals(list_of_processes, time_counter);
            print_process_status(current_process, "RUNNING", time_counter);
        }
        // if the process completed, add values to the calc variables and deallocate
        if (current_process->cpu_time == 0) {
            print_process_status(current_process, "COMPLETED", time_counter);
            total_turnaround_time += time_counter; // add process turn around time to counter
            total_waiting_time += current_process->wait_time;
            total_response_time += current_process->init_response_time;
            free(current_process);

            // otherwise set is_running back to false and append it to the end of the list (queue in this case)
        } else {
            current_process->is_running = 0;
            append(list_of_processes, current_process);
        }
    }

    // output final calculations
    printf("SIMULATION COMPLETED\n");
    printf("AVERAGE WAITING TIME: %f\n", ((double) total_waiting_time / total_processes));
    printf("AVERAGE RESPONSE TIME: %f\n", ((double) total_response_time / total_processes));
    printf("AVERAGE TURNAROUND TIME: %f\n", ((double) total_turnaround_time / total_processes));
    printf("AVERAGE CPU USAGE: %f PERCENT\n", (((double) cpu_running_time_counter / time_counter) * 100));
}
