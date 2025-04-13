# Pişti Game Implementation in C

## Overview
This repository contains a traditional Turkish card game "Pişti" implementation in C. The game is a two-player card game that requires strategy and memory skills.

## Project Structure
- **pisti.c**: Contains all game functions and core logic
- **main.c**: Manages the main flow and execution of the game

## Game Rules

### Game Setup
1. Four cards are placed face-up on the table
2. Four cards are distributed to each player
3. Player 1 starts the first round

### Special Initial Condition
- If a Jack appears on top of the table at the start, it will be given to Player 2 during the last distribution phase

### Gameplay Flow
- Players take turns placing one card at a time (Player 1, Player 2, Player 1, etc.)
- When players run out of cards, new sets of four cards are distributed to each player
- A round continues until the deck is finished
- At the start of each new round, cards are placed on the table from the deck
- Starting player alternates between rounds

### Capturing Cards
- If a player places a card that matches the rank of the top card on the table, they capture all cards on the table
- If a player places Jack, they capture all the cards
- If a player places a card that matches the rank of the top card on the table, and there is only 1 card, it is called a "Pişti" and player earns 10 points.

### Scoring System
- Aces: 1 point each
- Jacks: 1 point each
- Two of Clubs: 2 points (beautiful 2)
- Ten of Diamonds: 3 points (beautiful 10)
- Each "Pişti": 10 points
- Player with more captured cards: 3 bonus points

### Game End
- Game continues until one player scores 100 points or more
- If both players exceed 100 points with equal scores, an additional round is played

## How to Run
- Make sure that main.c and pisti.h are in the same folder
- Run main.c file for game

## For questions and suggestions
- Mail: [nadirabulfazazizov@gmail.com](mailto:nadirabulfazazizov@gmail.com)
- Instagram: [@n1azizov](https://www.instagram.com/n1azizov/)
- LinkedIn: [Nadir Azizov](https://www.linkedin.com/in/n1azizov/)
- Facebook: [Nadir Azizov](https://www.facebook.com/n1azizov)
