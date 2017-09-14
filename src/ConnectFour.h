#pragma once

#include "IConnectFour.h"
#include <ostream>
#include <array>

#define MAX_ROW 6
#define MAX_COLUMN 7

using Column = std::array<Player, MAX_ROW>;
using Grid = std::array<Column, MAX_COLUMN>;

/**
 * @brief Permet la construction et l'utilisation des attributs et fonctions membres du Puissance 4
 *
 * @author @author Damien Carnal, Matthieu Chatelan, Loan Lassalle
 * @since 13.04.2016
 */
class ConnectFour : public IConnectFour {
public:
    ConnectFour();

    /**
     *  @brief Remet à zèro les attributs pour jouer une nouvelle partie
     */
    void reset();

    /**
     *  @brief Joue pour le joueur p dans la colonne c
     *
     *  @param c numero de colonne entre 0 et 6
     *  @param p joueur X ou O
     */
    void playInColumn(size_t c, Player p);

    /**
     *  @brief Verifie si le joueur p gagne
     *
     *  @param p joueur X ou O
     *  @return true si 4 a la suite pour p, false sinon
     */
    bool isWinner(Player p) const;

    /**
     *  @brief Verifie si jouer dans la colonne c est valide
     *
     *  @param c numero de colonne entre 0 et 6
     *  @return true si c est entre 0 et 6 et que la colonne n'est pas pleine, false sinon
     */
    bool isValidMove(size_t c) const;

    /**
     *  @brief Choisit la prochain mouvement pour le joueur p
     *
     *  @param p joueur X ou O
     *  @param depth profondeur de recherche de l'algorithme minimax
     *  @return un numero de colonne entre 0 et 6 sans modifier l'etat de la grille du jeu
     *          typiquement calcule avec une variante a choix de l'algorithme minimax
     */
    size_t chooseNextMove(Player p, unsigned depth);

    /**
     *  @brief Renvoie le nom de votre classe pour le tournoi
     *
     *  @return un nom qui sera typiquement tronque a l'affichage pour ne garder que les 4 premieres
     *          lettres
     */
    std::string getName() const;

private:

    /**
     * @brief Grille contenant les mouvements des joueurs
     */
    Grid grid;

    /**
     * @brief Dernière case jouée
     */
    unsigned lastBoxColumn,
            lastBoxRow;

    /**
     * @brief Tableau des alignements de pions de chaque joueur
     */
    std::array<unsigned, 3> alignmentsO;
    std::array<unsigned, 3> alignmentsX;

    /**
     * @brief Recense les alignements de 2, 3 et 4 pions du joueurs
     *
     * @param score nombre de pions alignés
     * @param playerAlignments tableau des alignements du joueur
     * @return le meilleur score d'un mouvement
     */
    void alignmentsCount(unsigned score, std::array<unsigned, 3> &playerAlignments);

    /**
     * @brief Evalue le score d'un noeud de l'arbre de jeu
     *
     * @param bestColumn meilleure colonne à jouer
     * @param depht profondeur de recherche de l'arbre de jeu
     * @param alpha nombre de référence pour l'élagage de l'arbre de jeu du joueur
     * @param beta nombre de référence pour l'élagage de l'arbre de jeu de l'adversaire
     * @param p joueur
     * @return le meilleur score d'un mouvement.
     */
    int alphabeta(unsigned &bestColumn, unsigned depth, int alpha, int beta, Player p);

    /**
     * @brief Efface un pion joué dans une colonne
     *
     * @param c colonne dans laquelle effacer le dernier coup joué
     */
    void eraseMove(unsigned c);

    /**
     * @brief Determine si la grille est pleine.
     *
     * @return true si la grille est pleine, false sinon
     */
    bool isGameOver() const;

    /**
     * @brief Evalue la possibilité de gagner la partie pour un joueur
     *
     * @param p joueur jouant le dernier coup et dont on veut determiner la possibilité de gagner
     *          la partie
     * @return la possibilité que le joueur à de gagner ou de perdre
     */
    int nodeValue(Player p);

    /**
     * @brief Calcule le nombre de pions alignés par un joueur sur la colonne jouée au dernier coup
     *
     * @param p joueur jouant le dernier coup et dont on veut determiner le nombre de pions alignés
     * @return nombre de pions alignés sur la colonne jouée au dernier coup
     */
    void scoreColumn(Player p);

    /**
     * @brief Calcule le nombre de pions alignés par un joueur sur la diagonale
     * 	     du bas à gauche vers le haut à droite selon la position
     *        du dernier coup joué.
     *
     * @param p  Joueur jouant le dernier coup et dont on veut determiner
                 le nombre de pions alignés
     * @return Nombre de pions alignés sur la diagonale ascendente.
     */
    void scoreDiagAsc(Player p);

    /**
     * @brief Calcule le nombre de pions alignés par un joueur sur la diagonale
     *        du haut à gauche vers le bas à droite selon la position
              du dernier coup joué.
     *
     * @param p  Joueur jouant le dernier coup et dont on veut determiner
                 le nombre de pions alignés
     * @return Nombre de pions alignés sur la diagonale descendente.
     */
    void scoreDiagDesc(Player p);

    /**
     * @brief Calcule le nombre de pions alignés par un joueur sur la ligne jouée
              au dernier coup.
     *
     * @param p  Joueur jouant le dernier coup et dont on veut determiner
                 le nombre de pions alignés
     * @return Nombre de pions alignés sur la ligne jouée au dernier coup.
     */
    void scoreRow(Player p);

    /**
     *  @brief Surcharge de << pour afficher la grille du jeu
     *
     *  @param flux   Flux utilisé pour l'affichage
     *  @param P4Type Objet de votre classe P4
     *  @return Le flux original modifié.
     */
    friend std::ostream &operator<<(std::ostream &flux, const ConnectFour &P4Type);
};
