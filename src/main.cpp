/**
 *  @brief
 *  @author Damien Carnal, Matthieu Chatelan, Loan Lassalle
 *  @date 13.04.2016
 */

#include "ConnectFour.h"
#include <iostream>
#include <chrono>

using namespace std;
using P4Type = ConnectFour;
using Clock = std::chrono::high_resolution_clock;

const int INITIAL_DEPTH = 5;
const int INCREASE_DEPTH_IF_FASTER_THAN = 200; // millisecondes

/**
 * @brief Choisit interactivement le prochain mouvement en le demandant au joueur et en verifiant
 *        que le choix est licite. Cette fonction est recursive si le choix est illicite.
 *
 * @param game
 * @param player
 * @return
 */
int interactive(P4Type &game, Player player) {
    cout << endl << "Prochain movement du joueur "
         << (player == X ? "X" : "O") << ": "
         << flush;

    int n;
    cin >> n;
    cout << endl;

    if (!cin.good() || !game.isValidMove(n)) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Movement non valide. Essayez encore." << endl;

        return interactive(game, player);
    } else {
        return n;
    }
}

/**
 * @brief Choisit interactivement le nombre de joueurs humains en le demandant au joueur et en
 *        verifiant que le choix est licite. Cette fonction est recursive si le choix est illicite.
 *
 * @return
 */
int askNumberOfPlayers() {
    cout << "Combien de joueurs humains? (0, 1 ou 2) " << flush;

    int n;
    cin >> n;
    cout << endl;

    if (!cin.good() || n < 0 || n > 2) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Nombre de joueur invalide." << endl;

        return askNumberOfPlayers();
    } else {
        return n;
    }
}

/**
 * @brief Demande le nombre de joueurs humains, choisit aleatoirement qui commence en cas de match
 *        humain/ordinateur. Puis boucle tant qu'il n'y a pas de vainqueur et que le tableau n'est
 *        pas plein. Affiche le resultat du match.
 *
 * @return
 */
int main() {
    srand((int) time(0));

    int np = askNumberOfPlayers();
    bool human[2];

    for (size_t i = 0; i < 2; i++) {
        human[i] = i + 1 <= np;
    }

    if (rand() % 2) {
        swap(human[0], human[1]);
    }

    P4Type game;
    Player currentPlayer = X;

    int depth = INITIAL_DEPTH;

    unsigned piecesJouees = 0;

    for (; piecesJouees < 42; ++piecesJouees) {
        cout << game;

        size_t move;

        if (human[(currentPlayer + 1) / 2]) {
            move = interactive(game, currentPlayer);
        } else {
            Clock::time_point t1 = Clock::now();
            move = game.chooseNextMove(currentPlayer, depth);
            Clock::time_point t2 = Clock::now();

            if (chrono::duration_cast<chrono::milliseconds>(t2 - t1).count()
                < INCREASE_DEPTH_IF_FASTER_THAN and depth + piecesJouees <= 42) {
                ++depth;
                cout << "Nouvelle profondeur minimax: " << depth << endl;
            }
        }

        game.playInColumn(move, currentPlayer);

        if (game.isWinner(currentPlayer)) {
            break;
        }

        currentPlayer = Player(-currentPlayer);
    }

    cout << game;

    if (piecesJouees >= 42) {
        cout << " \n Egalité! \n";
    } else {
        cout << " \n" << (currentPlayer == X ? 'X' : 'O') << " gagne! \n";
    }
}
