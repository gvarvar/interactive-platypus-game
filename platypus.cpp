#include <iostream>
#include <map>
#include <utility>

enum Animal
{
    KANGAROO,
    EMU,
    WOMBAT,
    PLATYPUS
};

enum Tree
{
    WATTLE=-1,
    GHOST_GUM=1
};

enum Colour
{
    YELLOW='y',
    GREEN='G'
};

typedef std::pair<Animal,Colour> Transition;
typedef std::tuple<Animal,Colour,Tree> Action;
typedef std::map<Transition,Action> TransitionTable;

class PlatypusGame
{
    char board[21] = {YELLOW};
};

int main()
{
    std::cout << "Hello World\n";

    TransitionTable machine0;
    machine0[Transition(KANGAROO,YELLOW)] = Action(EMU,GREEN,WATTLE);

    return 0;
}