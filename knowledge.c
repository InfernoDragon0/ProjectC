/*
 * ICT1002 (C Language) Group Project.
 *
 * This file implements the chatbot's knowledge base.
 *
 * knowledge_get() retrieves the response to a question.
 * knowledge_put() inserts a new response to a question.
 * knowledge_read() reads the knowledge base from a file.
 * knowledge_reset() erases all of the knowledge.
 * knowledge_write() saves the knowledge base in a file.
 *
 * You may add helper functions as necessary.
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "chat1002.h"

/*
 * Get the response to a question.
 *
 * Input:
 *   intent   - the question word
 *   entity   - the entity
 *   response - a buffer to receive the response
 *   n        - the maximum number of characters to write to the response buffer
 *
 * Returns:
 *   KB_OK, if a response was found for the intent and entity (the response is copied to the response buffer)
 *   KB_NOTFOUND, if no response could be found
 *   KB_INVALID, if 'intent' is not a recognised question word
 */

Knowledge *head = NULL;

int knowledge_get(const char *intent, const char *entity, char *response, int n) {

	if (head != NULL) { //check if we loaded things already
		Knowledge *finder = head;
		while (finder != NULL) {
			if (compare_token(finder->intent,intent) != 0 || compare_token(finder->entity, entity) != 0) {
				finder = finder->next;
				continue;
			}
			snprintf(response, n, "i know! %s", finder->response);
			return KB_OK; //stop when found
		}
		snprintf(response, n, "Do you have an answer to %s is %s?", intent, entity);
		return KB_NOTFOUND; //if whole loop succeeds without returning, then it is not found.
	}
	else {
		snprintf(response, n, "Do you have an answer to %s is %s?", intent, entity);
		return KB_NOTFOUND;
	}

}


/*
 * Insert a new response to a question. If a response already exists for the
 * given intent and entity, it will be overwritten. Otherwise, it will be added
 * to the knowledge base.
 *
 * Input:
 *   intent    - the question word
 *   entity    - the entity
 *   response  - the response for this question and entity
 *
 * Returns:
 *   KB_FOUND, if successful
 *   KB_NOMEM, if there was a memory allocation failure
 *   KB_INVALID, if the intent is not a valid question word
 */
int knowledge_put(const char *intent, const char *entity, const char *response) {

	//create our knowledge piece
	Knowledge *knowledge_entity = (Knowledge *)malloc(sizeof(Knowledge)); //might wanna check for no mem

	knowledge_entity->intent = malloc(strlen(intent) + 1); //alloc and copy
	strcpy(knowledge_entity->intent, intent);

	knowledge_entity->entity = malloc(strlen(entity) + 1);;
	strcpy(knowledge_entity->entity, entity);

	knowledge_entity->response = malloc(strlen(response) + 1);;
	strcpy(knowledge_entity->response, response);
	//my next is empty first
	knowledge_entity->next = NULL;

	//check if initial head is created
	if (head != NULL) {

		Knowledge *temp = head; //iterate the list to the last in list
		while (temp->next != NULL) {
			temp = temp->next;
		}
		temp->next = knowledge_entity;
		printf("ADDED NEXT %s -> %s\n", temp->next->response, temp->response);
		return KB_OK;
	}
	else {
		//create first head
		head = knowledge_entity;
		printf("CREATED HEAD FOR %s\n", head->response);
		return KB_OK;
	}

	return KB_INVALID;

}

/*
 * Read a knowledge base from a file.
 *
 * Input:
 *   f - the file
 *
 * Returns: the number of entity/response pairs successful read from the file
 */
int knowledge_read(FILE *f) {
    int lines = 0;

    char *node = malloc(MAX_INTENT * sizeof(char));
    char *splitstr;

    char entity [MAX_ENTITY]; //should create function for dis 3
    char intent[MAX_INTENT];
    char response[MAX_RESPONSE];

    while(read_line(node, f) != KB_NOTFOUND) {

        if (strstr(node, "what")) {
            strcpy(intent, "WHAT");                     // Set Intent to WHAT until Next Intent Found
        }
        else if (strstr(node, "where")) {
            strcpy(intent, "WHERE");                    // Set Intent to WHERE until Next Intent Found
        }
        else if (strstr(node, "who")) {
            strcpy(intent, "WHO");
        }
        if (strchr(node,ch)) {
            splitstr = strtok(node, "=");          // Split first token Obtain the Entity from line of file
            strcpy(entity, splitstr);

            splitstr = strtok(NULL, "=");          // Find 2nd token Obtain the Response from line of file
			strcpy(response, splitstr);

            splitstr[strcspn(splitstr, "\n")] = 0;

            knowledge_put(intent, entity, response);    // Send to Knowledge_Put to insert into List
            lines++;
        }
    }

    fflush(stdout);                                     // Flush any unecessary remaining input
	if (node) {
		free(node);
	}
    //free(node); //this crashes some reason

	//this is a debug printing the brain
	Knowledge *temp = head;
	while (temp != NULL) {
		printf("the brain stored intent: %s found: %s ||| %s\n", temp->intent, temp->entity, temp->response);
		temp = temp->next;
	}

    return lines;

//    for (node = getc(f); node != EOF; node = getc(f))
//        if (node == '\n')
//            lines = lines + 1;
//
//
//    fclose(f);
//    free(node);
//	return lines;
}

int read_line(char *node, FILE *f) {
	//MAX_INTENT
	if (fscanf(f, "%[^\n]", node) != EOF) {
		fgetc(f); //bypass \n to read next line
    	//printf("Data:%s \n", node);
		return KB_OK;
	}
	return KB_NOTFOUND;
}

/*
 * Reset the knowledge base, removing all know entitities from all intents.
 */
void knowledge_reset() {

	/* to be implemented */
	Knowledge *nextTemp;
	for (Knowledge *temp = head; temp; temp = nextTemp) {
		nextTemp = temp->next;
		printf("CLEARING: %s\n", temp->response);
		// free(temp->response);
		// free(temp->entity);
		// free(temp->intent);
		free(temp);
		temp = NULL;
	}
	head = NULL;
}


/*
 * Write the knowledge base to a file.
 *
 * Input:
 *   f - the file
 */
void knowledge_write(FILE *f) {

	/* to be implemented */
    if (startWHAT!=NULL){
            Knowledge*temp = startWHAT;
            fprintf(f,"[%s]\n", "what");
            while (temp != NULL){
                fprintf(f,"%s=", temp->entity);
                fprintf(f,"%s\n", temp->response);
                temp = temp->next;
            }
        }

        fprintf(f,"%s", "\n");

        if (startWHO!=NULL){
            Knowledge*temp = startWHO;
            fprintf(f,"[%s]\n", "who");
            while (temp != NULL){
                fprintf(f,"%s=", temp->entity);
                fprintf(f,"%s\n", temp->response);
                temp = temp->next;
            }
        }

        fprintf(f,"%s", "\n");


        if (startWHERE!=NULL){
            Knowledge*temp = startWHERE;
            fprintf(f,"[%s]\n", "where");
            while (temp != NULL){
                fprintf(f,"%s=", temp->entity);
                fprintf(f,"%s\n", temp->response);
                temp = temp->next;
            }
        }

        fprintf(f,"%s", "\n");
}
