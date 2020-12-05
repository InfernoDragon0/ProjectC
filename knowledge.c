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
#include <ctype.h>

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

	/*check if we loaded things already*/
	if (head != NULL) { 
		Knowledge *finder = head;
		while (finder != NULL) {
			if (compare_token(finder->intent,intent) != 0 || compare_token(finder->entity, entity) != 0) {
				finder = finder->next;
				continue;
			}
			snprintf(response, n, "i know! %s", finder->response);
			return KB_OK; 																/*stop when found*/
		}
		snprintf(response, n, "Do you have an answer to %s is%s?", intent, entity);
		return KB_NOTFOUND; 															/*if whole loop succeeds without returning, then it is not found.*/
	}
	else {
		snprintf(response, n, "Do you have an answer to %s is%s?", intent, entity);
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

	/*Create our knowledge piece*/
	Knowledge *knowledge_entity = (Knowledge *)malloc(sizeof(Knowledge)); //might wanna check for no mem

	/*Alloc and copy*/
	knowledge_entity->intent = malloc(strlen(intent) + 1); 
	strcpy(knowledge_entity->intent, intent);

	knowledge_entity->entity = malloc(strlen(entity) + 1);
	strcpy(knowledge_entity->entity, entity);

	knowledge_entity->response = malloc(strlen(response) + 1);
	strcpy(knowledge_entity->response, response);

	/*Next is empty first*/
	knowledge_entity->next = NULL;

	/*Check if initial head is created*/
	if (head != NULL) {
		
		/*Iterate the list to the last in list*/
		Knowledge *temp = head;
		while (temp->next != NULL) {
			temp = temp->next;
		}
		temp->next = knowledge_entity;
		printf("ADDED NEXT %s -> %s\n", temp->next->response, temp->response);
		return KB_OK;
	}
	else {
		/*Create first head*/
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
    int lines = 0;											/*Lines counter*/
    char *node = malloc(MAX_INTENT * sizeof(char));
    char *splitstr;
    char entity [MAX_ENTITY]; 								
    char intent[MAX_INTENT];
    char response[MAX_RESPONSE];

    while(read_line(node, f) != KB_NOTFOUND) {
		
		/*Set Intent to WHAT until Next Intent Found*/
        if (strstr(node, "what")) {
            strcpy(intent, "what");
        }

		/*Set Intent to WHERE until Next Intent Found*/
        else if (strstr(node, "where")) {
            strcpy(intent, "where");
        }

		/*Set Intent to WHO*/
        else if (strstr(node, "who")) {
            strcpy(intent, "who");
        }

        if (strchr(node,ch)) {

			/*Split first token Obtain the Entity from line of file*/
            splitstr = strtok(node, "=");
            strcpy(entity, splitstr);
          	
			/*Find 2nd token Obtain the Response from line of file*/
            splitstr = strtok(NULL, "=");
			strcpy(response, splitstr);

            splitstr[strcspn(splitstr, "\n")] = 0;

			/*Send to Knowledge_Put to insert into List*/
            knowledge_put(intent, entity, response);
            
			/*Add counter*/
			lines++;
        }
    }

	/*Flush any unecessary remaining input*/
    fflush(stdout);
	if (node) {
		free(node);
	}
	
	/*This is a debug printing the brain*/
	Knowledge *temp = head;
	while (temp != NULL) {
		printf("the brain stored intent: %s found: %s ||| %s\n", temp->intent, temp->entity, temp->response);
		temp = temp->next;
	}

    return lines;
}

int read_line(char *node, FILE *f) {
	/*MAX_INTENT*/
	if (fscanf(f, "%[^\n]", node) != EOF) {
		fgetc(f); /*Bypass \n to read next line*/
		return KB_OK;
	}
	return KB_NOTFOUND;
}

/*
 * Reset the knowledge base, removing all know entitities from all intents.
 */
void knowledge_reset() {	
	Knowledge *nextTemp;									/*Pointer to store next node*/
	
	/*Loop through and free node*/
	for (Knowledge *temp = head; temp; temp = nextTemp) {
		nextTemp = temp->next;
		printf("CLEARING: %s\n", temp->response);
		free(temp->response);
		free(temp->entity);
		free(temp->intent);
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

    char currentIntent[MAX_INTENT];

	Knowledge *temp = head;
	
	/*Initial start with what*/	
	strcpy(currentIntent, "what");
	
	/*Initial print*/
	if(compare_token(temp->intent,currentIntent)==0){
		fprintf(f,"[what]\n");
	}

	while (temp != NULL) {
		
		/*Still same intent*/
		if (compare_token(temp->intent, currentIntent) == 0) {
			
			/*Change intent to lowercase*/
			for(int i=0; i<strlen(currentIntent);i++){
			currentIntent[i]=tolower(currentIntent[i]);
			}	

			fprintf(f,"%s=%s\n", temp->entity, temp->response);		/*Write to file entity and response*/
		}
		else {
			strcpy(currentIntent, temp->intent);

			/*Change intent to lowercase*/
			for(int i=0; i<strlen(currentIntent);i++){
			currentIntent[i]=tolower(currentIntent[i]);
			}	

			fprintf(f, "[%s]\n", currentIntent);					/*Write to file [intent]*/
			fprintf(f,"%s=%s\n", temp->entity, temp->response);		/*Write to file entity and response*/
			/*Continue; Skip*/
		}
		temp = temp->next;
	}
}
