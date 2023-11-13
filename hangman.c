/*====================================================================================================================
 *
 *                                              University College Dublin
 *                                          EEEN20010 - Computer Engineering I
 *
 * File Name	:	hangman.c
 *
 * Description	:   Program for a game of hangman. First, the player selects a text file from the working directory.
 *                  This text file consists of one word: the word to be guessed. This file is opened and read,
 *                  and the word is stored as a string. Another string, of equal length, is generated. This string
 *                  contains only asterisks and is what will be displayed throughout the game. The player is then
 *                  asked to guess either the word or a single letter. Depending on their choice, the guess_word or
 *                  guess_letter function gets called. If the guess is incorrect, the player loses a turn. If it is
 *                  correct, the guessed letter replaces any asterisks in its place in the display string. A correct
 *                  guess of the entire word replaces the entire display string. The player wins if the original
 *                  string and the display string become equal. The player loses if they run out of turns. At the
 *                  end, the player is asked if they want to play again.
 *
 * Author		:	Donal C. Monahan
 *
 * Date			:	10-11-22
 *
 *==================================================================================================================*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAXLENGTH 50    /* maximum length for all strings in program */
#define STARTTURNS 10   /* number of turns the player starts with */

/* function prototypes */
int guess_letter(char [], char [], int, int);
int guess_word(char [], char [], int, int);

int main(void) {

    char word_or_letter;                /* character to represent whether player wants to guess the word or a letter */
    char yes_or_no;                     /* character to represent whether player wants to play again */

    /* do-while loop executes again if yes_or_no == 'y' */
    do {

        char file_name[MAXLENGTH + 1] = {};     /* string which will store the name of the file we want to open; reset to empty array in each game */
        FILE *word_file;                        /* pointer to file we want to open */
        char original_word[MAXLENGTH + 1] = {}; /* string we want to read from the file; reset to empty array in each game */
        char *word;                             /* pointer to string that we read from the file */
        int turns = STARTTURNS;                 /* number of turns player has remaining; reset to STARTTURNS in each game */
        char display_word[MAXLENGTH + 1] = {};  /* string displayed to player throughout game to represent the word they are guessing; reset to empty array in each game */

        /* select file */
        printf("Give the filename with the unknown word: ");
        scanf("%s", file_name);

        /* open file */
        word_file = fopen(file_name, "r");

        /* error if file not found; error message printed to stderr */
        if(word_file == NULL){

            fprintf(stderr, "Error! Cannot open file.\n");
            exit(EXIT_FAILURE);

        }

        /* read string */
        word = fgets(original_word, MAXLENGTH, word_file);

        /* error if file is empty; error message printed to stderr */
        if (word == NULL) {

            fprintf(stderr, "Error! The file is empty.\n");
            exit(EXIT_FAILURE);

        }

        /* find length of word */
        int length = strlen(word);

        /* populate display_word string with asterisks */
        for(int i = 0; i < length; i++)

            display_word[i] = '*';

        /* start game */
        printf("\nReady to start!\n");
        printf("The word is %s\n\n", display_word);

        /* loop while player has turns left, and the word has not been found */
        while (turns > 0 && strcmp(word, display_word) != 0) {

            /* ask player if they want to guess the word or a letter */
            printf("Number of turns remaining: %d\n", turns);
            printf("Would you like to guess the word [w] or guess a letter [l]: ");
            scanf(" %c", &word_or_letter);

            /* if player is guessing the word */
            if(word_or_letter == 'w') {

                /* call on guess_word function */
                turns = guess_word(word, display_word, length, turns);

                /* if word is correctly guessed, break loop */
                if(strcmp(word, display_word) == 0)

                    break;

            }

            /* if player is guessing a letter */
            else if(word_or_letter == 'l')

                /* call on guess_letter function */
                turns = guess_letter(word, display_word, length, turns);

            /* print updated display_word, with any correctly guessed letters revealed, if game is not over */
            if(strcmp(word, display_word) != 0 && turns != 0)

                printf("The word is %s\n", display_word);

        }

        /* if word and display_word are equal, player has won */
        if(strcmp(word, display_word) == 0)

            printf("Congratulations!\n");

        /* if word and display_word are not equal, player has lost */
        else

            printf("You lose!\n");


        /* close file */
        fclose(word_file);

        /* ask player if they want to play again */
        printf("\n***********************************************\n");
        printf("Do you want to play again [y/n]: ");
        scanf(" %c", &yes_or_no);
        printf("\n***********************************************\n");

    } while(yes_or_no == 'y');

    return 0;

}

/*====================================================================================================================
 *  This function takes the correct word, the display word, the length of the word, and the player's remaining turns
 *  as arguments. The player is then prompted to guess a word. If guess and word are not equal, the player loses a
 *  turn. "Bad choice!" is printed unless the player's last turn is used up (in that case "You lose!" is printed by
 *  the main function). If the guess and word are equal, display_word is updated to also be equal to them. This
 *  function returns a value for turns, which is updated only if the guess is incorrect.
 *==================================================================================================================*/
int guess_word(char word[], char display_word[], int length, int turns) {

    char guess[MAXLENGTH + 1];

    /* ask player for their guess */
    printf("What is your chosen word?: ");
    scanf(" %s", guess);

    printf("\n***********************************************\n");

    /* if guess is wrong, remove a turn */
    if(strcmp(word, guess) != 0) {

        turns--;

        /* only print "Bad choice!" if game is not over */
        if(turns != 0)

            printf("Bad choice!\n");

    }

    /* if guess is right, display_word becomes equal to word */
    else {

        for(int i = 0; i < length; i++) {

            display_word[i] = word[i];

        }

    }

    /* return new value of turns to main function */
    return turns;

}

/*====================================================================================================================
 *  This function takes the correct word, the display word, the length of the word, and the player's remaining turns
 *  as arguments. The variable correct starts off equal to 0. The player is then prompted to guess a letter. If the
 *  guess does not occur within the word, correct remains equal to 0, a turn is taken from the player, and "Bad
 *  choice!" is printed (if the player has just lost their last turn, nothing is printed here but "You lose!" is
 *  printed by the main function). If the guess does occur within the word, correct will become equal to 1, and no
 *  turn is taken. "Good choice!" is printed unless the user has guessed all letters correctly, in which case the main
 *  function will print "Congratulations!". This function returns a value for turns, which is only updated if the
 *  guess is incorrect.
 *==================================================================================================================*/
int guess_letter(char word[], char display_word[], int length, int turns) {

    char guess;
    int correct = 0; /* if correct remains equal to 0, player has guessed incorrectly */

    /* ask player for their guess */
    printf("What letter have you chosen?: ");
    scanf(" %c", &guess);

    /* loop through each letter of word */
    for(int i = 0; i < length; i++) {

        /* if guessed letter matches any letter in the word, the letter is revealed in display_word */
        if(guess == word[i]) {

            display_word[i] = word[i];
            correct = 1; /* correct = 1 means player has guessed correctly */

        }

    }

    printf("\n***********************************************\n");

    /* if correct has remained 0 since it was initialised, guess is incorrect and a turn is removed */
    if(correct != 1){

        turns--;

        /* only print "Bad choice!" if game is not over */
        if(turns != 0)

            printf("Bad choice!\n");

    }

    /* if correct = 1, guess is correct. if word and display_word are equal (i.e. game is won), do not print "Good choice!" */
    else if(strcmp(word, display_word) != 0)

        printf("Good choice!\n");

    return turns;

}
