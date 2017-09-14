/**
 *  @brief Détails des outils pour la construction et l'utilisation du jeu Puissance 4
 *  @author Damien Carnal, Matthieu Chatelan, Loan Lassalle
 *  @date 13.04.2016
 */

#include "ConnectFour.h"
#include <algorithm>

#define WIN_SCORE 4

ConnectFour::ConnectFour() {
    reset();
}

void ConnectFour::alignmentsCount(unsigned score, std::array<unsigned, 3> &playerAlignments) {
    const unsigned USEFUL_INDEX = score - 2;

    if (USEFUL_INDEX > 2) {
        return;
    }

    const unsigned PREVIOUS_INDEX = score - 3;

    if (PREVIOUS_INDEX < 2 && playerAlignments[PREVIOUS_INDEX]) {
        --playerAlignments[PREVIOUS_INDEX];
    }

    ++playerAlignments[USEFUL_INDEX];
}

int ConnectFour::alphabeta(unsigned &bestColumn, unsigned depth, int alpha, int beta, Player p) {
    int bestScore = -std::numeric_limits<int>::max();

    if (isWinner(Player(-p))) {
        bestScore = -100 * int(depth + 1);
    } else if (!depth) {
        // Evaluation de la possibilité de gagner du joueur.
        bestScore = nodeValue(p);
    } else if (isGameOver()) {
        bestScore = 0;
    } else {
        int score;
        unsigned nextBestColumn;

        std::array<unsigned, MAX_COLUMN> columns = {3, 4, 5, 0, 1, 2, 6};
        std::random_shuffle(columns.begin(), columns.end());

        // Sauvegarde des tableaux des alignements des joueurs.
        std::array<unsigned, 3> previousAlignmentsO = alignmentsO;
        std::array<unsigned, 3> previousAlignmentsX = alignmentsX;

        // Détermination du meilleur score pour le noeud de l'arbre à jeu.
        for (unsigned column : columns) {
            if (isValidMove(column)) {
                playInColumn(column, Player(p));
                score = -alphabeta(nextBestColumn, depth - 1, -beta, -alpha, Player(-p));
                eraseMove(column);

                // Récupération des anciens tableaux des alignements des joueurs.
                alignmentsO = previousAlignmentsO;
                alignmentsX = previousAlignmentsX;

                if (score > bestScore) {
                    bestScore = score;
                    bestColumn = column;
                }

                if (score > alpha) {
                    alpha = bestScore;
                    bestColumn = column;

                    if (alpha > beta) {
                        break;
                    }
                }
            }
        }
    }

    return bestScore;
}

size_t ConnectFour::chooseNextMove(Player p, unsigned depth) {
    unsigned bestColumn = MAX_COLUMN / 2;

    if (grid[bestColumn][MAX_ROW - 1] == Player::EMPTY) {
        return bestColumn;
    }

    alphabeta(bestColumn, depth, -std::numeric_limits<int>::max(), std::numeric_limits<int>::max(),
              p);

    return bestColumn;
}

void ConnectFour::eraseMove(unsigned c) {
    playInColumn(c, Player::EMPTY);
}

std::string ConnectFour::getName() const {
    return std::string("A_Carnal_Chatelan_Lassalle");
}

bool ConnectFour::isGameOver() const {
    // Contrôle si une colonne possède un emplacement vide.
    for (unsigned column = 0; column < MAX_COLUMN; ++column) {
        // Une colonne, au minimum, à une case vide.
        if (isValidMove(column)) {
            return false;
        }
    }

    // Plus aucune colonne ne possède d'emplacement vide.
    return true;
}

bool ConnectFour::isValidMove(size_t c) const {
    return c < MAX_COLUMN && grid[c][0] == Player::EMPTY;
}

bool ConnectFour::isWinner(Player p) const {
    if (p == Player::O) {
        return alignmentsO[2];
    } else {
        return alignmentsX[2];
    }
}

int ConnectFour::nodeValue(Player p) {
    // Calcul du score de chaque joueur en fonction des alignements de pions effectués et
    // de leurs coefficients.
    const int scoreO =
            int(alignmentsO[0]) * 1 + int(alignmentsO[1]) * 10 + int(alignmentsO[2]) * 100,
            scoreX = int(alignmentsX[0]) * 1 + int(alignmentsX[1]) * 10 + int(alignmentsX[2]) * 100;

    if (p == Player::O) {
        return scoreO - scoreX;
    } else {
        return scoreX - scoreO;
    }
}

void ConnectFour::playInColumn(size_t c, Player p) {
    // Première case pleine (du haut vers le bas).
    unsigned firstFullBox = unsigned(std::count(grid[c].begin(), grid[c].end(), Player::EMPTY));

    if (p == Player::EMPTY) {
        // Suppresion du dernier coup joué dans la colonne.
        grid[c][firstFullBox] = p;
    } else {
        grid[c][--firstFullBox] = p;

        //Enregistre la colonne et la ligne du dernier pion joué.
        lastBoxColumn = c;
        lastBoxRow = firstFullBox;

        // Recensement des alignements de pions.
        scoreColumn(p);
        scoreDiagAsc(p);
        scoreDiagDesc(p);
        scoreRow(p);
    }
}

void ConnectFour::reset() {
    // Initialisation des tableaux des alignements des joueurs à 0.
    alignmentsO.fill(0);
    alignmentsX.fill(0);

    // Initialisation de la grille de jeu.
    for (Column &column : grid) {
        for (Player &box : column) {
            box = Player::EMPTY;
        }
    }
}

void ConnectFour::scoreColumn(Player p) {
    unsigned score = 1;

    // Calcul du score à partir de la dernière case jouée jusqu'à la dernière case de la colonne (vers le bas).
    for (unsigned row = lastBoxRow + 1;
         row < MAX_ROW && grid[lastBoxColumn][row] == p && score < WIN_SCORE; ++row) {
        ++score;
    }

    if (p == Player::O) {
        alignmentsCount(score, alignmentsO);
    } else {
        alignmentsCount(score, alignmentsX);
    }
}

void ConnectFour::scoreDiagAsc(Player p) {
    unsigned score = 1;

    // Calcul du score à partir de la dernière case jouée jusqu'à la dernière case de la diagnonale (vers le haut à droite).
    for (unsigned column = lastBoxColumn + 1, row = lastBoxRow - 1;
         column < MAX_COLUMN && row < MAX_ROW && grid[column][row] == p &&
         score < WIN_SCORE; ++column, --row) {
        ++score;
    }

    if (score < WIN_SCORE) {
        // Calcul du score à partir de la case à gauche de la dernière case jouée jusqu'à la première case de la diagnonale (vers le bas à gauche).
        for (unsigned column = lastBoxColumn - 1, row = lastBoxRow + 1;
             column < MAX_COLUMN && row < MAX_ROW && grid[column][row] == p &&
             score < WIN_SCORE; --column, ++row) {
            ++score;
        }
    }

    if (p == Player::O) {
        alignmentsCount(score, alignmentsO);
    } else {
        alignmentsCount(score, alignmentsX);
    }
}

void ConnectFour::scoreDiagDesc(Player p) {
    unsigned score = 1;

    // Calcul du score à partir de la dernière case jouée jusqu'à la dernière case de la diagnonale (vers le bas à droite).
    for (unsigned column = lastBoxColumn + 1, row = lastBoxRow + 1;
         column < MAX_COLUMN && row < MAX_ROW && grid[column][row] == p &&
         score < WIN_SCORE; ++column, ++row) {
        ++score;
    }

    if (score < WIN_SCORE) {
        // Calcul du score à partir de la case à gauche de la dernière case jouée jusqu'à la première case de la diagnonale (vers le haut à gauche).
        for (unsigned column = lastBoxColumn - 1, row = lastBoxRow - 1;
             column < MAX_COLUMN && row < MAX_ROW && grid[column][row] == p &&
             score < WIN_SCORE; --column, --row) {
            ++score;
        }
    }

    if (p == Player::O) {
        alignmentsCount(score, alignmentsO);
    } else {
        alignmentsCount(score, alignmentsX);
    }
}

void ConnectFour::scoreRow(Player p) {
    unsigned score = 1;

    // Calcul du score à partir de la dernière case jouée jusqu'à la dernière case de la ligne (vers la droite).
    for (unsigned column = lastBoxColumn + 1;
         column < MAX_COLUMN && grid[column][lastBoxRow] == p && score < WIN_SCORE; ++column) {
        ++score;
    }

    if (score < WIN_SCORE) {
        // Calcul du score à partir de la case à gauche de la dernière case jouée jusqu'à la première case de la ligne (vers la gauche).
        for (unsigned column = lastBoxColumn - 1;
             column < MAX_COLUMN && grid[column][lastBoxRow] == p && score < WIN_SCORE; --column) {
            ++score;
        }
    }

    if (p == Player::O) {
        alignmentsCount(score, alignmentsO);
    } else {
        alignmentsCount(score, alignmentsX);
    }
}

std::ostream &operator<<(std::ostream &flux, const ConnectFour &P4Type) {
    const char SYMBOL_O = 'O',
            SYMBOL_X = 'X',
            EMPTY_BOX = ' ';

    const std::string SEPARATOR = " | ";

    flux << std::endl;

    for (size_t row = 0; row < MAX_ROW; ++row) {
        flux << SEPARATOR;

        for (size_t column = 0; column < MAX_COLUMN; ++column) {
            flux << (P4Type.grid[column][row] == Player::EMPTY ? EMPTY_BOX :
                     (P4Type.grid[column][row] == Player::O ? SYMBOL_O : SYMBOL_X));

            flux << SEPARATOR;
        }
        flux << std::endl;
    }

    flux << std::endl << SEPARATOR;

    for (unsigned column = 0; column < MAX_COLUMN; ++column) {
        flux << column << SEPARATOR;
    }

    flux << std::endl << std::endl;

    return flux;
}
