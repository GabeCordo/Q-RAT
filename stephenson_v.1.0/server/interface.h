#ifndef HEADER_INTERFACE
#define HEADER_INTERFACE

	#include <stdio.h>
	#include <stdlib.h>
	#include <string.h>
	
	/* Type Declarations */
	
	typedef struct machine_connection_ machine_connection_t;//forward declaration of machine_connection_t for the ll next
	
	typedef enum connected_ {
		CON_FALSE,
		CON_TRUE
	} connected_t;
	
	struct machine_connection_ {
		char *name;
		void *client_socket;
		connected_t is_active;
		machine_connection_t *next;
	};
	
	typedef struct machine_library_ {
		machine_connection_t *head;
		int num_of_machines;
	} machine_library_t;
	
	/* Macro Declarations */
	
	#define MACHINE_CONNECTION_INIT(machine_library, machine_connection, socket)	\
		machine_connection->name = #machine_connection;								\
		machine_connection->client_socket = socket;									\
		machine_connection->is_active = CON_TRUE;									\
		machine_connection->next = NULL;											\
		machine_library_add(machine_library, machine_connection);					\
	
	#define MACHINE_LIBRARY_INIT(machine_library)		\
		machine_library->head = NULL;					\
		machine_library->num_of_machines = 0;			\
	
	#define ITERATE_MACHINE_BEGIN(machine_library, ptr)			\
	{															\
		for (ptr = machine_library->head; ptr; ptr->next) {		\
		
	#define ITERATE_MACHINE_END }}
	
	/* Function Deffinitions */
	
	void machine_library_print(machine_library_t * library);
	machine_connection_t *machine_library_active_list(machine_library_t *library, char *name);
	
	int machine_library_add(machine_library_t *library, machine_connection_t* socket);
	int machine_library_remove(machine_library_t *library, machine_connection_t* machine_connection);
	
#endif //HEADER_INTERFACE