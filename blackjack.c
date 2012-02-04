/******************************************************************************
 * bj.c
 *
 * RW
 * Blackjack Game
 *
 * Simple console based blackjack game.
 *
 *****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>


// function prototypes
int Deal();
int GetCardValue(int card);
char GetCardSuit(int card);
int GetHandValue(int hand[], int length);
void InitializeHand(int hand[], int length);
void ShowHand(int hand[], int length);
int AddToHand(int hand[], int length, int card);
int CheckCardInHand(int hand[], int length, int card);
int ValidateCard(int hand[], int length, int card);
int AIPlayTurn(int aiHand[], int aiHandSize, int playerHand[], int playerHandSize);
void AIFinishGame(int aiHand[], int aiHandSize, int playerHand[], int playerHandSize);
void PlayerPlayTurn(int playerHand[], int playerHandSize, int aiHand[], int aiHandSize);
int GameDecideWinner(int playerHand[], int playerHandSize, int aiHand[], int aiHandSize);

// main
int main(void) {
    // seed the random number generator
    srand(time(NULL));

    // declare some variables for our use
    int score = 0; 
    int answer = 1;

    // declare a constant size for the players hand
    const int kHandSize = 8;
    
    // array to store cards in a players hand.
    int hand[kHandSize];
    InitializeHand(hand, kHandSize);

    // create and initialize the AI hand
    int AIHand[kHandSize];
    InitializeHand(AIHand, kHandSize);

    // main game loop
    while(answer != 2 && score <= 21) {
        // player turn
        PlayerPlayTurn(hand, kHandSize, AIHand, kHandSize);

        // AI play turn
        AIPlayTurn(AIHand, kHandSize, hand, kHandSize);

        // show player hand
        printf("In Hand:  ");
        ShowHand(hand, kHandSize);

        // update player score
        score = GetHandValue(hand, kHandSize);

        // show player score
        printf("  Score:  %d\n", score);

        // ask if the player wants to deal another card if they haven't bust
        if(score < 21) {
            printf("Would you like to hit (1: Yes  |  2: No)? ");
            scanf("%d", &answer);
        }

        // blank lines for nicer output
        printf("\n");
    }

    // let the AI keep playing if it needs to
    AIFinishGame(AIHand, kHandSize, hand, kHandSize);

    // display AI hand and score
    printf("Computer:  ");
    ShowHand(AIHand, kHandSize);
    printf("   Score:  %d\n", GetHandValue(AIHand, kHandSize));

    // display player hand
    printf("  Player:  ");
    ShowHand(hand, kHandSize);
    printf("   Score:  %d\n", GetHandValue(hand, kHandSize));

    // print the winner of the game
    int winner = GameDecideWinner(hand, kHandSize, AIHand, kHandSize);
    if(winner == 1)
        printf("You Won! Congratulations.\n");
    else if(winner == 0)
        printf("You Lost.\n");
    else
        printf("The game was a tie.\n");
    
    return 0;
}

// function implementations

// Deals a card to a player.
// returns a random number between 1 and 52
int Deal() {
    return rand() % 52 + 1;
}

// Gets the face value of a card.
// Input: card is any number that needs to be converted to a value 1 through 10
// Return: returns a value between 1 and 10
int GetCardValue(int card) {
    if(card % 13 == 0 || card % 13 == 12 || card % 13 == 11)
        return 10;

    if(card % 13 == 1)
        return 1;

    return card % 13;
}

// Gets the suit of a card
// Input: card is a number which between 1 and 52 to be interpreted as the cards suit
// Return: H, D, C, S for correct inputs, ? is returned for invalid inputs
char GetCardSuit(int card) {
    if(card >= 1 && card <= 13)
        return 'H';
    else if(card >= 14 && card <= 26)
        return 'D';
    else if(card >= 27 && card <= 39)
        return 'C';
    else if(card >= 40 && card <= 52)
        return 'S';
    return '?';
}

// Gets the value or score of a players hand
// Input: card1 and card2 are face values of cards
// Return: the sum of both cards
int GetHandValue(int hand[], int length) {
    int tempScore = 0;
    for(int i = 0; i < length; i++) {
        if(hand[i] != -1)
            if(GetCardValue(hand[i]) == 1) 
                tempScore += (tempScore + 11 <= 21) ? 11 : 1;
            else
                tempScore += GetCardValue(hand[i]);
    }
    return tempScore;
}

// Initialize an array representing the players hand
// All values are set to '-1'
void InitializeHand(int hand[], int length) {
    for(int i = 0; i < length; i++)
        hand[i] = -1;
}

// Display the users hand on the scren
// Input: hand is an array containing the players hand
// Input: length is the size of the array
void ShowHand(int hand[], int length) {
    for(int i = 0; i < length; i++) {
        if(hand[i] != -1) {
            if(hand[i] % 13 == 1)
                printf("A%c, ", GetCardSuit(hand[i]));
            else if(hand[i] % 13 == 0)
                printf("K%c, ", GetCardSuit(hand[i]));
            else if(hand[i] % 13 == 12)
                printf("Q%c, ", GetCardSuit(hand[i]));
            else if(hand[i] % 13 == 11)
                printf("J%c, ", GetCardSuit(hand[i]));
            else
                printf("%d%c, ", GetCardValue(hand[i]), GetCardSuit(hand[i]));
        }
    }
    printf("\n");
}

// Adds a card to the players hand
// Input: hand is an array representing the players hand
// Input: length is the size of the array
// Input: card is the value of the card to be added to the players hand
// Return: returns -1 if the hand is full otherwise 0
int AddToHand(int hand[], int length, int card) {
    for(int i = 0; i < length; i++) {
        if(hand[i] == -1) {
            hand[i] = card;
            return 0;
        }
    }
    return -1;
}

// Checks that a card is valid and hasn't been used
// Input: hand is an array of cards in a hand
// Input: length is the size of the array
// Input: card is the value to be checked if it is valid
// Return: 1 if the card is in the hand, 0 if not
int CheckCardInHand(int hand[], int length, int card) {
    int inHand = 0;

    for(int i = 0; i < length; i++) {
        if(hand[i] == card)
            inHand = 1;
    }
    return inHand;
}

// Makes sure that a card is valid and hasn't been used already
// Input: hand is an array of cards in a hand
// Input: length is the size of the array
// card is the card being checked
// Return: if the card is valid it is returned otherwise a new valid card is returned
int ValidateCard(int hand[], int length, int card) {
    while(CheckCardInHand(hand, length, card)) {
        card = Deal();
    }
    return card;
}

// Plays one turn for an AI opponent
// Input: aiHand is an array representing the AI's hand
// Input: aiHandSize is the length of the AI hand array
// Input: playerHand is an array representing the players hand
// Input: playerHandSize is the length of the player hand array
int AIPlayTurn(int aiHand[], int aiHandSize, int playerHand[], int playerHandSize) {
    // check whether or not the AI should draw another card
    if(GetHandValue(aiHand, aiHandSize) <= 16) {  // Draw
        // deal a card to the AI
        int tempCard = Deal();

        // make sure the card hasn't already been dealt to the ai or player
        tempCard = ValidateCard(aiHand, aiHandSize, tempCard);
        tempCard = ValidateCard(playerHand, playerHandSize, tempCard);

        // add the card to the AI hand
        AddToHand(aiHand, aiHandSize, tempCard);

        // the AI finished his turn and is waiting
        return 1;
    }
    // AI cannot draw another card
    return 0;
}

// Allows the AI to finish the game if the player ends his turns
// Input: aiHand is an array representing the AI hand
// Input: aiHandSize is the length of the AI hand array
// Input: playerHand is an array representing the player's hand
// Input: playerHandSize is the length of the players hand array
void AIFinishGame(int aiHand[], int aiHandSize, int playerHand[], int playerHandSize) {
    // keep playing the game until the AI cannot draw another card
    while(AIPlayTurn(aiHand, aiHandSize, playerHand, playerHandSize)) {}
}

// Plays one turn for the player
// Input: playerHand is an array representing the player's hand
// Input: playerHandSize is the length of the player hand array
// Input: aiHand is an array representing the AI's hand
// Input: aiHandSize is the length of the AI hand array
void PlayerPlayTurn(int playerHand[], int playerHandSize, int aiHand[], int aiHandSize) {
    // first deal a new card to the player
    int tempCard = Deal();

    // make sure the card hasn't already been dealt to the player or AI
    tempCard = ValidateCard(aiHand, aiHandSize, tempCard);
    tempCard = ValidateCard(playerHand, playerHandSize, tempCard);

    // add the card to the Player's hand
    AddToHand(playerHand, playerHandSize, tempCard);
}

// Determines the winner of a single game or tie
// Input: playerHand is an array representing the player's hand
// Input: playerHandSize is the length of the player's hand array
// Input: aiHand is an array representing the AI's hand
// Input: aiHandSize is the lenth of the AI hand array
// Returns: 0 if the computer won, 1 if the player won, 2 if the game is a tie
int GameDecideWinner(int playerHand[], int playerHandSize, int aiHand[], int aiHandSize) {
    // get the score for the player and AI
    int playerScore = GetHandValue(playerHand, playerHandSize);
    int aiScore = GetHandValue(aiHand, aiHandSize);

    // check if the player and ai bust
    if(playerScore > 21 && aiScore > 21)
        return 2;

    // check if the player bust
    if(playerScore > 21)
        return 0;

    // check if the AI bust
    if(aiScore > 21)
        return 1;

    // determine the winner of the game
    if(playerScore > aiScore)
        return 1;  // player wins
    else if(playerScore == aiScore)
        return 2;  // tie game

    // AI wins
    return 0;
}
