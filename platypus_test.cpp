#include <iostream>
#include <map>
#include <utility>
#include <vector>
#include <cstdio>
#include <functional>

enum Animal
{
    KANGAROO,
    EMU,
    WOMBAT,
    PLATYPUS,
    TERMINATED
};

enum Tree
{
    WATTLE=-1,
    GHOST_GUM=1
};

enum Colour
{
    YELLOW='Y',
    GREEN='G'
};

typedef std::pair<Animal,Colour> Transition;
struct Action 
{ 
    Animal animal; 
    Colour colour; 
    Tree tree; 
    Action(Animal a, Colour c, Tree t) : animal(a),colour(c),tree(t) {}
    Action() = default;
};
typedef std::map<Transition,Action> TransitionTable;

class PlatypusMachine
{
    Animal animal = KANGAROO;
    int position = 10; // The 'billabong'
    TransitionTable t;

    public:
    PlatypusMachine(TransitionTable& transitions) : t(transitions)
    {

    }

    Colour doTurn(Colour cellColour)
    {
        if (animal == TERMINATED || (animal == PLATYPUS && cellColour == GREEN))
        {
            animal = TERMINATED;
            return cellColour; // Do nothing, retain colour too.
        }
        // Look in the table to decide what to do.
        Transition trans = Transition(animal,cellColour);
        if (t.find(trans) == t.end()) {animal = TERMINATED;return cellColour;}
        Action a = t[trans];

        // Change position
        position += a.tree;

        // Change state
        animal = a.animal;

        // Tell the game what colour we want to change.
        return a.colour;
    }

    int getPosition() const 
    {
        return position;
    }

    Animal getAnimal() const 
    {
        return animal;
    }
};

class PlatypusGame
{
    static constexpr int boardSize = 21;
    Colour board[21];
    PlatypusMachine machine;
    int points = 0;

    std::map<Animal,std::string> animal_strings;
    std::map<Animal,std::string> animal_emoji;

    public:
    PlatypusGame(PlatypusMachine m) : machine(m)
    {
        for (int i=0; i<boardSize; i++) board[i]=YELLOW;
        animal_strings[KANGAROO] = "Kangaroo";
        animal_strings[WOMBAT] = "Wombat";
        animal_strings[EMU] = "Emu";
        animal_strings[PLATYPUS] = "Platypus";

        animal_emoji[KANGAROO] = "🦘";
        animal_emoji[WOMBAT] = "🦔";
        animal_emoji[EMU] = "🦃";
        animal_emoji[PLATYPUS] = "🦫";
    }

    void setboard(const std::string_view& str)
    {
        for (int i=0; i<21; ++i)
        {
            board[i] = (Colour)str[i];
        }
    }

    int play()
    {
        //std::cout << "Playing platypus game\n";

        for (int move = 0; machine.getAnimal() != TERMINATED; ++move)
        {
            const int pos = machine.getPosition() % 20; // Loops
            //printf("%*s%s\n", pos*2, "",animal_emoji[machine.getAnimal()].c_str());
            // Print the board
            for (Colour c : board)
            {
                //std::cout << (c==YELLOW ? "🟨" : "🟩");
            }
            //std::cout<<" | Animal: \"" << animal_strings[machine.getAnimal()] << "\"   \t| Points: " << points << " | Move: " << move <<'\n';
            // Print a position marker
            //printf("%*s☝️\n\n", pos*2, "");

            // Do the move
            Colour colour = machine.doTurn(board[pos]);
            if (board[pos] != colour) { points++; board[pos] = colour; }

            if (machine.getAnimal() == TERMINATED)
            {
                //std::cout << "Machine terminated in " << move << " moves\n";
                return move;
            }
        }
        return -1;
    }
};

void generateStrings(std::string current, int remaining,const std::function<void(const std::string&)>& process) {
    if (remaining == 0) {
        process(current);
        return;
    }
    
    generateStrings(current + 'Y', remaining - 1,process);
    generateStrings(current + 'G', remaining - 1,process);
}

int main()
{
    TransitionTable trans;
    
    trans[Transition(KANGAROO,YELLOW)] = Action(EMU,GREEN,GHOST_GUM);
    trans[Transition(KANGAROO,GREEN)] = Action(WOMBAT,YELLOW,WATTLE);

    trans[Transition(EMU,YELLOW)] = Action(PLATYPUS,YELLOW,GHOST_GUM);
    trans[Transition(EMU,GREEN)] = Action(KANGAROO,YELLOW,GHOST_GUM);

    trans[Transition(WOMBAT,YELLOW)] = Action(EMU,GREEN,GHOST_GUM);
    trans[Transition(WOMBAT,GREEN)] = Action(WOMBAT,GREEN,GHOST_GUM);

    trans[Transition(PLATYPUS,YELLOW)] = Action(KANGAROO,GREEN,WATTLE);

    // platypus(1, [t(y,k,g,e,gg),t(g,k,y,w,wa),t(y,e,y,p,gg),t(g,e,y,k,gg),t(y,w,g,w,gg),t(g,w,g,w,gg),t(y,p,g,k,wa)]).
    int min = 99999999;
    std::string min_string;
    int duplicates = 0;
    PlatypusMachine machine(trans);
    generateStrings("",21,[&min,&min_string,&machine,&duplicates](const std::string& s) {
        PlatypusGame game(machine);
        game.setboard(s);
        const int m = game.play();
        if (m < min)
        {
            min = m;
            min_string = s;
            duplicates = 0;
        }
        if (m == min) duplicates++;
    });

    std::cout << "best answer is " << min_string << " in " << min << " with " << duplicates << " other solutions\n";

    //PlatypusMachine machine(trans);

    //PlatypusGame game(machine);
    //game.play();

    return 0;
}