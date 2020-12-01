/*
 * ICT1002 (C Language) Group Project.
 *
 * This file implements the behaviour of the chatbot. The main entry point to
 * this module is the chatbot_main() function, which identifies the intent
 * using the chatbot_is_*() functions then invokes the matching chatbot_do_*()
 * function to carry out the intent.
 *
 * chatbot_main() and chatbot_do_*() have the same method signature, which
 * works as described here.
 *
 * Input parameters:
 *   inc      - the number of words in the question
 *   inv      - an array of pointers to each word in the question
 *   response - a buffer to receive the response
 *   n        - the size of the response buffer
 *
 * The first word indicates the intent. If the intent is not recognised, the
 * chatbot should respond with "I do not understand [intent]." or similar, and
 * ignore the rest of the input.
 *
 * If the second word may be a part of speech that makes sense for the intent.
 *    - for WHAT, WHERE and WHO, it may be "is" or "are".
 *    - for SAVE, it may be "as" or "to".
 *    - for LOAD, it may be "from".
 * The word is otherwise ignored and may be omitted.
 *
 * The remainder of the input (including the second word, if it is not one of the
 * above) is the entity.
 *
 * The chatbot's answer should be stored in the output buffer, and be no longer
 * than n characters long (you can use snprintf() to do this). The contents of
 * this buffer will be printed by the main loop.
 *
 * The behaviour of the other functions is described individually in a comment
 * immediately before the function declaration.
 *
 * You can rename the chatbot and the user by changing chatbot_botname() and
 * chatbot_username(), respectively. The main loop will print the strings
 * returned by these functions at the start of each line.
 */

#include <stdio.h>
#include <string.h>
#include "chat1002.h"

int awaiting_knowledge = 0; //set this to wait for knowledge
Knowledge *awaiting = NULL;

/*
 * Get the name of the chatbot.
 *
 * Returns: the name of the chatbot as a null-terminated string
 */
const char *chatbot_botname() {

	return "Chatbot";

}


/*
 * Get the name of the user.
 *
 * Returns: the name of the user as a null-terminated string
 */
const char *chatbot_username() {

	return "User";

}


/*
 * Get a response to user input.
 *
 * See the comment at the top of the file for a description of how this
 * function is used.
 *
 * Returns:
 *   0, if the chatbot should continue chatting
 *   1, if the chatbot should stop (i.e. it detected the EXIT intent)
 */
int chatbot_main(int inc, char *inv[], char *response, int n) {

	/* check for empty input */
	if (inc < 1) {
		snprintf(response, n, "");
		return 0;
	}

	if (awaiting_knowledge && awaiting != NULL) { //consume next input as knowledge, unless the user doesnt know
		if (strcmp(inv[0], "no") != 0) {
			char learn[MAX_ENTITY];
			strcpy(learn, inv[0]);
				
			//concat all the rest of the data with space
			for (int i = 1; i < inc; i++) {
				strcat(learn, " ");
				strcat(learn, inv[i]);
			}

			if (knowledge_put(awaiting->intent, awaiting->entity, learn) != 0) {
				snprintf(response, n, "I had trouble learning the knowledge.");
			}
			else {
				snprintf(response, n, "I have learned new things!");
			}
			
		}
		else {
			snprintf(response, n, "u bad bad");
		}

		//clear temp learner
		free(awaiting);
		awaiting = NULL;
		awaiting_knowledge = 0;

		return 0;
	}

	/* look for an intent and invoke the corresponding do_* function */
	if (chatbot_is_exit(inv[0]))
		return chatbot_do_exit(inc, inv, response, n);
	else if (chatbot_is_smalltalk(inv[0]))
		return chatbot_do_smalltalk(inc, inv, response, n);
	else if (chatbot_is_load(inv[0]))
		return chatbot_do_load(inc, inv, response, n);
	else if (chatbot_is_question(inv[0]))
		return chatbot_do_question(inc, inv, response, n);
	else if (chatbot_is_reset(inv[0]))
		return chatbot_do_reset(inc, inv, response, n);
	else if (chatbot_is_save(inv[0]))
		return chatbot_do_save(inc, inv, response, n);
	else {
		snprintf(response, n, "I don't understand \"%s\".", inv[0]);
		return 0;
	}

}


/*
 * Determine whether an intent is EXIT.
 *
 * Input:
 *  intent - the intent
 *
 * Returns:
 *  1, if the intent is "exit" or "quit"
 *  0, otherwise
 */
int chatbot_is_exit(const char *intent) {

	return compare_token(intent, "exit") == 0 || compare_token(intent, "quit") == 0;

}


/*
 * Perform the EXIT intent.
 *
 * See the comment at the top of the file for a description of how this
 * function is used.
 *
 * Returns:
 *   0 (the chatbot always continues chatting after a question)
 */
int chatbot_do_exit(int inc, char *inv[], char *response, int n) {

	snprintf(response, n, "Goodbye!");

	return 1;

}


/*
 * Determine whether an intent is LOAD.
 *
 * Input:
 *  intent - the intent
 *
 * Returns:
 *  1, if the intent is "load"
 *  0, otherwise
 */
int chatbot_is_load(const char *intent) {

	/* to be implemented */
	if(compare_token(intent, "load") == 0){
        return 1;
    }
    else
        return 0;

}


/*
 * Load a chatbot's knowledge base from a file.
 *
 * See the comment at the top of the file for a description of how this
 * function is used.
 *
 * Returns:
 *   0 (the chatbot always continues chatting after loading knowledge)
 */
int chatbot_do_load(int inc, char *inv[], char *response, int n) {

	/* to be implemented */

		FILE *f;
        int fileline = 0;
        char* userinput = inv[1];
        if (! userinput){
            strcpy(response,"Please indicate a file to load!");
        }
        else
        {
        if (f = fopen(inv[1], "r")){

        	fileline = knowledge_read(f);
            fclose(f);
           	snprintf(response, n,"File consists of %d lines", fileline);
        }
        else{
            snprintf(response, n, "File not found!");
        }
    }
    return 0;

}


/*
 * Determine whether an intent is a question.
 *
 * Input:
 *  intent - the intent
 *
 * Returns:
 *  1, if the intent is "what", "where", or "who"
 *  0, otherwise
 */
int chatbot_is_question(const char *intent) {

 	if(compare_token(intent, "what") == 0 || compare_token(intent, "where") == 0 || compare_token(intent, "who") == 0){
		 return 1;
 	}
 	return 0;

}


/*
 * Answer a question.
 *
 * inv[0] contains the the question word.
 * inv[1] may contain "is" or "are"; if so, it is skipped.
 * The remainder of the words form the entity.
 *
 * See the comment at the top of the file for a description of how this
 * function is used.
 *
 * Returns:
 *   0 (the chatbot always continues chatting after a question)
 */
int chatbot_do_question(int inc, char *inv[], char *response, int n) {

	/* to be implemented */
	char qnsIntent[MAX_INTENT];
	strncpy(qnsIntent, inv[0], sizeof(qnsIntent)/sizeof(qnsIntent[0])); //Define char array to store qns intent

	char qnsEntity[MAX_ENTITY]; //Define char array to store entity
	qnsEntity[0] = '\0';

	//Checks if question is less than 2 words (Ex. only stating "where is")
	if (inc <= 2)
	{
		if(compare_token(qnsIntent,"what") == 0){
		snprintf(response,n,"Do you mean %s is ICT1002?",qnsIntent);
		return 0;
		}
		else if(compare_token(qnsIntent,"where") == 0){
		snprintf(response,n,"Do you mean %s is SIT?",qnsIntent);
		return 0;
		}
		else if(compare_token(qnsIntent,"who") == 0){
		snprintf(response,n,"Do you mean %s is Frank Guan?",qnsIntent);
		return 0;
		}
	}

	//check if inv[1] contain "is" or "are"
		else if(compare_token(inv[1],"is") == 0  || compare_token(inv[1],"are") == 0) {
			strcpy(qnsEntity, inv[2]);
			
			//concat all the rest of the data with space
			for (int i = 3; i < inc; i++) {
				strcat(qnsEntity, " ");
				strcat(qnsEntity, inv[i]);
			}
		}

	if (knowledge_get(qnsIntent,qnsEntity,response,n) == -1) {
		awaiting = (Knowledge *) malloc(sizeof(Knowledge));
		//store the data to learn
		awaiting->intent = malloc(strlen(qnsIntent) + 1); //alloc and copy
		strcpy(awaiting->intent, qnsIntent);

		awaiting->entity = malloc(strlen(qnsEntity) + 1);;
		strcpy(awaiting->entity, qnsEntity);

		awaiting->response = malloc(strlen(response) + 1);;
		strcpy(awaiting->response, response);
		
		//ignore next anyways
		//awaiting->next = NULL;

		awaiting_knowledge = 1;
	}

	return 0;

}


/*
 * Determine whether an intent is RESET.
 *
 * Input:
 *  intent - the intent
 *
 * Returns:
 *  1, if the intent is "reset"
 *  0, otherwise
 */
int chatbot_is_reset(const char *intent) {

	/* to be implemented */
	if(compare_token(intent, "reset") == 0){
        return 1;
    }
    else
        return 0;

}


/*
 * Reset the chatbot.
 *
 * See the comment at the top of the file for a description of how this
 * function is used.
 *
 * Returns:
 *   0 (the chatbot always continues chatting after beign reset)
 */
int chatbot_do_reset(int inc, char *inv[], char *response, int n) {

	/* to be implemented */
	knowledge_reset();
    system("CLS");

    snprintf(response, n, "Hello, I'm %s.\n", chatbot_botname(), chatbot_botname());

    return 0;

}


/*
 * Determine whether an intent is SAVE.
 *
 * Input:
 *  intent - the intent
 *
 * Returns:
 *  1, if the intent is "what", "where", or "who"
 *  0, otherwise
 */
int chatbot_is_save(const char *intent) {

	/* to be implemented */

	return 0;

}


/*
 * Save the chatbot's knowledge to a file.
 *
 * See the comment at the top of the file for a description of how this
 * function is used.
 *
 * Returns:
 *   0 (the chatbot always continues chatting after saving knowledge)
 */
int chatbot_do_save(int inc, char *inv[], char *response, int n) {

	/* to be implemented */

	return 0;

}


/*
 * Determine which an intent is smalltalk.
 *
 *
 * Input:
 *  intent - the intent
 *
 * Returns:
 *  1, if the intent is the first word of one of the smalltalk phrases
 *  0, otherwise
 */
int chatbot_is_smalltalk(const char *intent) {

 /* to be implemented */

    const char* smalltalk[] = {
        "hello", "hi", "thanks", "done", "alright"
    };

	size_t smalltalkLength = sizeof(smalltalk)/sizeof(smalltalk[0]);

  for (int i = 0; i < smalltalkLength; i++) {
    if (compare_token(intent, smalltalk[i]) == 0) {
      return 1;
    }
  }
  return 0;
}

/*
 * Respond to smalltalk.
 *
 * See the comment at the top of the file for a description of how this
 * function is used.
 *
 * Returns:
 *   0, if the chatbot should continue chatting
 *   1, if the chatbot should stop chatting (e.g. the smalltalk was "goodbye" etc.)
 */
int chatbot_do_smalltalk(int inc, char *inv[], char *response, int n) {

	/* to be implemented */
    if (compare_token("hello", inv[0]) == 0 ||  compare_token("hi", inv[0]) == 0) {
//snprintf used as there is buffer, n % output
        snprintf(response, n, "Hi %s! How are you doing?", chatbot_username());
    }
    else if (compare_token("thanks", inv[0]) == 0) {

        snprintf(response, n, "You're welcome %s!", chatbot_username());
    }
    else if (compare_token("done", inv[0]) == 0) {

        snprintf(response, n, "Can i help you with anything else?");
    }
  else if (compare_token("alright", inv[0]) == 0) {

    snprintf(response, n, "Alright then.");
  }
	return 0;
}
