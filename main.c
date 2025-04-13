#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "pisti.h"

int main() {
    // Initialize variables
    Card deck[52];
    Card table[52];
    Card player1hand[4];
    Card player2hand[4];
    Card taken1cards[52];
    Card taken2cards[52];
    int deckSize = 52;
    int tableSize = 0;
    int player1HandSize = 0;
    int player2HandSize = 0;
    int taken1CardsSize = 0;
    int taken2CardsSize = 0;
    int player1Points = 0;
    int player2Points = 0;
    int player1PishtiPoints = 0;
    int player2PishtiPoints = 0;
    int currentPlayer = 1; // Player 1 starts the first round
    int roundCounter = 1;
    int distributionTour = 0;
    int lastTaker = 0; // To track who took cards last (1 for player 1, 2 for player 2)
    int lastPlayer = 0; // To track who played the very last card in the round

    printf("=======================================\n");
    printf("       WELCOME TO PİŞTİ GAME         \n");
    printf("=======================================\n\n");

    while (1) { // Game loop
        printf("\n========== ROUND %d ==========\n", roundCounter);
        
        // Initialize the deck for new round
        int index = 0;
        for (int s = HEARTS; s <= SPADES; s++) {
            for (int r = ACE; r <= KING; r++) {
                deck[index].suit = (Suit)s;
                deck[index].rank = (Rank)r;
                index++;
            }
        }
        
        // Reset taken cards and points for the new round
        taken1CardsSize = 0;
        taken2CardsSize = 0;
        player1PishtiPoints = 0;
        player2PishtiPoints = 0;
        lastPlayer = 0;

        // Shuffle the deck
        shuffle(deck, deckSize);
        shuffle(deck, deckSize);
        shuffle(deck, deckSize); // Multiple shuffles for better randomness
        shuffle(deck, deckSize);
        shuffle(deck, deckSize);
        
        // Place initial 4 cards on the table
        tableSize = 0;
        placeCardsOnTable(table, deck, &deckSize, &tableSize, 1, currentPlayer);
                
        // Reset distribution tour counter
        distributionTour = 0;
        
        // Main game loop for this round
        while (deckSize > 0) {
            distributionTour++;
            printf("\n--- Distribution Tour %d ---\n", distributionTour);
            
            // Distribute 4 cards to each player
            player1HandSize = 0;
            player2HandSize = 0;
            distribute(player1hand, deck, &deckSize);
            player1HandSize = 4;
            distribute(player2hand, deck, &deckSize);
            player2HandSize = 4;

            
            // Check if this is the final tour of the round
            int finalTour = (deckSize == 0);
            
            // Players take turns playing cards until both are out of cards
            while (player1HandSize > 0 || player2HandSize > 0) {
                // Track if this is the last play of the round
                if (finalTour && player1HandSize + player2HandSize == 1) {
                    lastPlayer = (player1HandSize == 1) ? 1 : 2;
                }
                
                if (currentPlayer == 1 && player1HandSize > 0) {
                    printf("\nPlayer 1's turn:\n");
                    move(player1hand, &player1HandSize, table, &tableSize, taken1cards, &taken1CardsSize, &player1PishtiPoints);
                    if (tableSize == 0) {
                        lastTaker = 1; // Player 1 was the last to take cards
                    }
                    currentPlayer = 2; // Switch to Player 2
                } else if (currentPlayer == 2 && player2HandSize > 0) {
                    printf("\nPlayer 2's turn:\n");
                    move(player2hand, &player2HandSize, table, &tableSize, taken2cards, &taken2CardsSize, &player2PishtiPoints);
                    if (tableSize == 0) {
                        lastTaker = 2; // Player 2 was the last to take cards
                    }
                    currentPlayer = 1; // Switch to Player 1
                } else {
                    // If current player has no cards but the other does, switch players
                    if (currentPlayer == 1 && player2HandSize > 0) {
                        currentPlayer = 2;
                    } else if (currentPlayer == 2 && player1HandSize > 0) {
                        currentPlayer = 1;
                    } else {
                        break; // Both players are out of cards
                    }
                }
            }
            
            // Show status after this distribution tour
            printf("\nEnd of Distribution Tour %d\n", distributionTour);
            printf("Player 1 has taken %d cards\n", taken1CardsSize);
            printf("Player 2 has taken %d cards\n", taken2CardsSize);
            printf("Cards on the table: %d\n", tableSize);
        }
        
        // Handle remaining cards on the table at the end of round
        if (tableSize > 0) {
            // The player who played the last card takes all remaining cards on the table
            if (lastPlayer > 0) {
                printf("\nPlayer %d played the last card of the round and takes all remaining cards on the table!\n", lastPlayer);
                if (lastPlayer == 1) {
                    takefromTable(taken1cards, &taken1CardsSize, table, &tableSize);
                } else {
                    takefromTable(taken2cards, &taken2CardsSize, table, &tableSize);
                }
            }
            // If somehow we couldn't determine who played last (shouldn't happen with correct logic)
            else if (lastTaker > 0) {
                printf("\nRemaining cards on the table are taken by the last player who took cards (Player %d).\n", lastTaker);
                if (lastTaker == 1) {
                    takefromTable(taken1cards, &taken1CardsSize, table, &tableSize);
                } else {
                    takefromTable(taken2cards, &taken2CardsSize, table, &tableSize);
                }
            } else {
                // Fallback if no one took cards during the round
                printf("No one took cards in this round, giving to Player %d.\n", 3 - currentPlayer);
                if (3 - currentPlayer == 1) {
                    takefromTable(taken1cards, &taken1CardsSize, table, &tableSize);
                } else {
                    takefromTable(taken2cards, &taken2CardsSize, table, &tableSize);
                }
            }
        }
        
        // Calculate points for the round
        int round1Points = calculatePoints(taken1cards, taken1CardsSize, player1PishtiPoints);
        int round2Points = calculatePoints(taken2cards, taken2CardsSize, player2PishtiPoints);
        
        // Add 3 points to the player with more cards
        if (taken1CardsSize > taken2CardsSize) {
            round1Points += 3;
            printf("Player 1 gets 3 extra points for having more cards (%d vs %d).\n", 
                   taken1CardsSize, taken2CardsSize);
        } else if (taken2CardsSize > taken1CardsSize) {
            round2Points += 3;
            printf("Player 2 gets 3 extra points for having more cards (%d vs %d).\n", 
                   taken2CardsSize, taken1CardsSize);
        } else {
            printf("Players have the same number of cards, no extra points awarded.\n");
        }
        
        // Update total points
        player1Points += round1Points;
        player2Points += round2Points;
        
        // Display round results
        printf("\n=== END OF ROUND %d ===\n", roundCounter);
        printf("Player 1's points this round: %d (Total: %d)\n", round1Points, player1Points);
        printf("Player 2's points this round: %d (Total: %d)\n", round2Points, player2Points);
        
        // Check for game-ending conditions
        int gameResult = checkWin(player1Points, player2Points);
        if (gameResult == 1) {
            printf("\n********************************************\n");
            printf("*  Player 1 wins the game with %d points! *\n", player1Points);
            printf("********************************************\n");
            break;
        } else if (gameResult == 2) {
            printf("\n********************************************\n");
            printf("*  Player 2 wins the game with %d points! *\n", player2Points);
            printf("********************************************\n");
            break;
        } else if (gameResult == 0) {
            printf("\nScores are tied at or above 100. Playing another round...\n");
        }
        
        // Switch starting player for the next round
        currentPlayer = (roundCounter % 2 == 0) ? 1 : 2;
        printf("\nPlayer %d will start the next round.\n", currentPlayer);
        
        // Prepare for next round
        roundCounter++;
        deckSize = 52; // Reset deck size
    }

    return 0;
}
