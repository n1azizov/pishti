#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

typedef enum{
    HEARTS,
    DIAMONDS,
    CLUBS,
    SPADES
} Suit;

typedef enum{
    ACE = 1, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE, TEN, JACK, QUEEN, KING
} Rank;

typedef struct{
    Rank rank;
    Suit suit;
} Card;

/**
 * Get the name of a suit.
 * @param suit The suit to get the name of.
 * @return A string representing the name of the suit.
 */
const char* getSuitName(Suit suit){
    switch(suit){
        case HEARTS: return "♥Hearts";
        case DIAMONDS: return "♦Diamonds";
        case CLUBS: return "♣Clubs";
        case SPADES: return "♠Spades";
        default: return "Unknown";
    }
}

/**
 * Get the name of a rank.
 * @param rank The rank to get the name of.
 * @return A string representing the name of the rank.
 */
const char* getRankName(Rank rank) {
    switch (rank) {
        case TWO:   return "2";
        case THREE: return "3";
        case FOUR:  return "4";
        case FIVE:  return "5";
        case SIX:   return "6";
        case SEVEN: return "7";
        case EIGHT: return "8";
        case NINE:  return "9";
        case TEN:   return "10";
        case JACK:  return "Jack";
        case QUEEN: return "Queen";
        case KING:  return "King";
        case ACE:   return "Ace";
        default:    return "Unknown";
    }
}

/**
 * Get the full name of a card.
 * @param card The card to get the name of.
 * @return A string representing the full name of the card.
 */
const char* CardName(Card card) {
    static char cardName[40];
    snprintf(cardName, sizeof(cardName), "%s of %s",
             getRankName(card.rank), getSuitName(card.suit));
    return cardName;
}

/**
 * Shuffle a deck of cards.
 * @param deck The deck of cards to shuffle.
 * @param size The size of the deck.
 */
void shuffle(Card deck[], int size) {
    srand(time(NULL));
    for (int i = size - 1; i > 0; i--) {
        int j = rand() % (i + 1); 

        Card temp = deck[i];
        deck[i] = deck[j];
        deck[j] = temp;
    }
}

/**
 * Distribute cards to a player's hand.
 * @param playerhand The player's hand to distribute cards to.
 * @param deck The deck of cards to distribute from.
 * @param deckSize Pointer to the size of the deck.
 */
void distribute(Card* playerhand, Card* deck, int* deckSize) {
    for (int i = 0; i < 4; i++) {
        if (*deckSize > 0) {
            playerhand[i] = deck[*deckSize - 1]; // Take the top card from the deck
            (*deckSize)--;                  // Decrease the deck size
        }
    }
}


/**
 * Show the top card on the table.
 * @param table The table of cards.
 * @param tableSize The size of the table.
 */
void showTopCard(Card* table, int tableSize) {
    if (tableSize > 0) {
        printf("Card on the top of the table: %s\n", CardName(table[0]));
    } else {
        printf("No cards on the table.\n");
    }
    printf("Cards on the table: %d\n\n", tableSize);
}

/**
 * Check if a card can take the top card on the table.
 * @param played The card played.
 * @param top The top card on the table.
 * @return 1 if taking is possible, 0 otherwise.
 */
int checkTake(Card played, Card top) {
    if (played.rank == top.rank || played.rank == JACK) {
        return 1; // Taking is possible
    }
    return 0; // Taking is not possible
}

/**
 * Check if a Pişti occurs.
 * @param played The card played.
 * @param table The table of cards.
 * @param tableSize The size of the table.
 * @return 1 if Pişti occurs, 0 otherwise.
 */
int checkPishti(Card played, Card* table, int tableSize) {
    if (tableSize == 1 && played.rank == table[0].rank && played.rank != JACK) {
        return 1; // Pişti is true
    }
    return 0; // Pişti is not true
}

/**
 * Check if a player has won the game.
 * @param player1point Player 1's points.
 * @param player2point Player 2's points.
 * @return 1 if Player 1 wins, 2 if Player 2 wins, 0 if tied, -1 if no winner yet.
 */
int checkWin(int player1point, int player2point){
    if (player1point >= 100 && player1point > player2point){
        return 1; // Player 1 wins
    }
    else if (player2point >= 100 && player2point > player1point){
        return 2; // Player 2 wins
    }
    else if (player1point >= 100 && player2point >= 100 && player2point == player1point){
        return 0; // Game didn't end, it should continue
    }
    return -1; // No winner yet
}

/**
 * Play a card onto the table.
 * @param card The card to play.
 * @param table The table of cards.
 * @param tableSize Pointer to the size of the table.
 */
void playCard(Card card, Card* table, int* tableSize) {
    for (int i = *tableSize; i > 0; i--) {
        table[i] = table[i - 1]; // Shift cards to make room for the new card
    }
    table[0] = card; // Place the new card at the top
    (*tableSize)++;  // Increment the table size
}

/**
 * Place cards from the deck onto the table.
 * @param table The table of cards.
 * @param deck The deck of cards.
 * @param deckSize Pointer to the size of the deck.
 * @param tableSize Pointer to the size of the table.
 * @param distributionTour The current distribution tour.
 * @param startingPlayer The player who starts the round.
 * @return 1 if Jack was moved, 0 otherwise.
 */
int placeCardsOnTable(Card* table, Card* deck, int* deckSize, int* tableSize, int distributionTour, int startingPlayer) {
    if (*deckSize < 4) {
        printf("Error: Not enough cards in the deck to place on the table.\n");
        return 0;
    }

    int jackMoved = 0;
    
    // Check if this is the first distribution tour of the round
    if (distributionTour == 1) {
        // Check if the first card is a Jack
        if (deck[*deckSize - 1].rank == JACK) {
            printf("A Jack appeared as the first card! Moving it to the bottom of the deck.\n");
            
            // Save the Jack card
            Card jackCard = deck[*deckSize - 1];
            
            // Move the Jack to the bottom of the deck (index 0)
            for (int i = *deckSize - 1; i > 0; i--) {
                deck[i] = deck[i - 1];
            }
            deck[0] = jackCard;
            
            jackMoved = 1;
        }
    }

    // Place 4 cards on the table normally
    for (int i = 0; i < 4; i++) {
        if (*deckSize > 0) {
            table[*tableSize] = deck[*deckSize - 1]; // Take the top card from the deck
            (*deckSize)--;                           // Decrease the deck size
            (*tableSize)++;                          // Increment table size
        }
    }
    
    return jackMoved;
}


/**
 * Take all cards from the table.
 * @param taker The taker's pile of cards.
 * @param takerCount Pointer to the taker's card count.
 * @param table The table of cards.
 * @param tableSize Pointer to the size of the table.
 */
void takefromTable(Card* taker, int* takerCount, Card* table, int* tableSize) {
    for (int i = 0; i < *tableSize; i++) {
        taker[*takerCount + i] = table[i];
    }
    (*takerCount) += (*tableSize);  // Update taker's card count
    *tableSize = 0;                // Clear the table
}

/**
 * Recommend the best card to play.
 * @param playerHand The player's hand.
 * @param playerHandSize The size of the player's hand.
 * @param table The table of cards.
 * @param tableSize The size of the table.
 * @return The index of the recommended card to play (1-based).
 */
int recommendMove(Card* playerHand, int playerHandSize, Card* table, int tableSize) {
    int recommendation = -1;
    
    if (tableSize == 0) {
        return rand() % playerHandSize + 1;
    }
    
    if (tableSize == 1) {
        for (int i = 0; i < playerHandSize; i++) {
            if (playerHand[i].rank == table[0].rank && playerHand[i].rank != JACK) {
                return i + 1; // Recommend this card
            }
        }
    }
    
    for (int i = 0; i < playerHandSize; i++) {
        if (playerHand[i].rank == table[0].rank || playerHand[i].rank == JACK) {
            return i + 1; // Recommend this card
        }
    }
    
    return rand() % playerHandSize + 1;
}

/**
 * Perform a player's move.
 * @param playerHand The player's hand.
 * @param playerHandSize Pointer to the size of the player's hand.
 * @param table The table of cards.
 * @param tableSize Pointer to the size of the table.
 * @param takenCards The player's pile of taken cards.
 * @param takenCardsSize Pointer to the size of the taken cards pile.
 * @param pishtiPoints Pointer to the player's Pişti points.
 */
void move(Card* playerHand, int* playerHandSize, Card* table, int* tableSize, Card* takenCards, int* takenCardsSize, int* pishtiPoints) {
    int choice;

    printf("Player's cards:\n");
    for (int i = 0; i < *playerHandSize; i++) {
        printf("%d. %s\n", i + 1, CardName(playerHand[i]));
    }

    printf("\nCards on the table:\n");
    if (*tableSize > 0) {
        printf("Top card: %s\n", CardName(table[0]));
        printf("Total cards on the table: %d\n", *tableSize);
    } else {
        printf("The table is empty.\n");
    }
    
    int recommendation = recommendMove(playerHand, *playerHandSize, table, *tableSize);
    printf("\nRecommended move: %d. %s\n", recommendation, CardName(playerHand[recommendation - 1]));

    printf("\nChoose a card to play (1-%d): ", *playerHandSize);
    scanf("%d", &choice);

    while (choice < 1 || choice > *playerHandSize) {
        printf("Invalid choice. Please choose a card between 1 and %d: ", *playerHandSize);
        scanf("%d", &choice);
    }

    Card playedCard = playerHand[choice - 1];
    printf("Played: %s\n", CardName(playedCard));
    
    if (*tableSize == 0) {
        playCard(playedCard, table, tableSize);
    } else {
        if (*tableSize == 1 && playedCard.rank == table[0].rank && playedCard.rank != JACK) {
            printf("\nPİŞTİ! Player takes the card and earns 10 points.\n");
            playCard(playedCard, table, tableSize);
            takefromTable(takenCards, takenCardsSize, table, tableSize);
            (*pishtiPoints) += 10;
        }
        else if (playedCard.rank == table[0].rank || playedCard.rank == JACK) {
            if (playedCard.rank == JACK) {
                printf("\nJack played! Player takes all cards from the table.\n");
            } else {
                printf("\nMatch! Player takes all cards from the table.\n");
            }
            playCard(playedCard, table, tableSize);
            takefromTable(takenCards, takenCardsSize, table, tableSize);
        }
        else {
            playCard(playedCard, table, tableSize);
        }
    }

    for (int i = choice - 1; i < *playerHandSize - 1; i++) {
        playerHand[i] = playerHand[i + 1];
    }
    (*playerHandSize)--;
}

/**
 * Calculate points based on taken cards and Pişti points.
 * @param takenCards The player's pile of taken cards.
 * @param takenCardsSize The size of the taken cards pile.
 * @param pishtiPoints The player's Pişti points.
 * @return The total points.
 */
int calculatePoints(Card* takenCards, int takenCardsSize, int pishtiPoints) {
    int points = pishtiPoints;

    for (int i = 0; i < takenCardsSize; i++) {
        if (takenCards[i].rank == ACE) {
            points += 1;
        } else if (takenCards[i].rank == JACK) {
            points += 1;
        } else if (takenCards[i].rank == TWO && takenCards[i].suit == CLUBS) {
            points += 2;
        } else if (takenCards[i].rank == TEN && takenCards[i].suit == DIAMONDS) {
            points += 3;
        }
    }

    return points;
}

#endif
