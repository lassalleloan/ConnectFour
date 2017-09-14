#pragma once

#include <cstdlib>
#include <string>

enum Player {
    O = -1, EMPTY = 0, X = 1
};

/**
 * @brief
 *
 * @author Damien Carnal, Matthieu Chatelan, Loan Lassalle
 * @since 13.04.2016
 */
class IConnectFour {
public:
    virtual std::string getName() const = 0;

    virtual size_t chooseNextMove(Player p, unsigned depth) = 0;

    virtual void playInColumn(size_t c, Player p) = 0;

    virtual void reset() = 0;

    virtual ~IConnectFour() {

    }
};
