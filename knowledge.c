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
int knowledge_get(const char *intent, const char *entity, char *response, int n) {

	/* to be implemented */
	char *intentKB[] = {"Where", "What", "Who"};
	char *entityKB[] = {"SIT","ICT1001","ICT1002","ICT1003","ICT1004","ICT1005"};
	size_t entityKBLength = sizeof(entityKB) / sizeof(entityKB[0]);
	size_t intentKBLength = sizeof(intentKB) / sizeof(intentKB[0]);
	
		for (int i = 0; i<intentKBLength; i++){
		//where
		if(compare_token(intent, intentKB[0]) == 0){
			for(int i = 0; i< entityKBLength; i++){
				if (compare_token(entity,entityKB[i]) == 0){
					snprintf(response,n, "found in KB (where)");
					return KB_OK;
				}else{
					return KB_NOTFOUND;
				}
			}
		} //what
		else if (compare_token(intent, intentKB[1]) == 0){
					for(int i = 0; i< entityKBLength; i++){
				if (compare_token(entity,entityKB[i]) == 0){
					snprintf(response,n, "found in KB (what)");
					return KB_OK;
				}else{
					return KB_NOTFOUND;
				}
			}
		}//who
		else if(compare_token(intent, intentKB[2]) == 0){
					for(int i = 0; i< entityKBLength; i++){
				if (compare_token(entity,entityKB[i]) == 0){
					snprintf(response,n, "found in KB (who)");
					return KB_OK;
				}else{
					return KB_NOTFOUND;
				}
			}
		}else{
			snprintf(response,n,"Intent is not a recognised question word");
			return KB_INVALID;
		}
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

	/* to be implemented */

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

	/* to be implemented */

	return 0;
}


/*
 * Reset the knowledge base, removing all know entitities from all intents.
 */
void knowledge_reset() {

	/* to be implemented */

}


/*
 * Write the knowledge base to a file.
 *
 * Input:
 *   f - the file
 */
void knowledge_write(FILE *f) {

	/* to be implemented */

}
