#include <iostream>
#include <map>
#include <utility>
#include <vector>
#include <cstdio>
#include <functional>
#include <cmath>
#include <cstdlib>
#include <ctime>

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
    WATTLE = -1,
    GHOST_GUM = 1
};

enum Colour
{
    YELLOW = 'Y',
    GREEN = 'G'
};

typedef std::pair<Animal, Colour> Transition;
struct Action
{
    Animal animal;
    Colour colour;
    Tree tree;
    Action(Animal a, Colour c, Tree t) : animal(a), colour(c), tree(t) {}
    Action() = default;
};
typedef std::map<Transition, Action> TransitionTable;

class PlatypusMachine
{
    Animal animal = KANGAROO;
    int position = 10; // The 'billabong'
    TransitionTable t;

public:
    PlatypusMachine(TransitionTable &transitions) : t(transitions)
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
        Transition trans = Transition(animal, cellColour);
        if (t.find(trans) == t.end())
        {
            animal = TERMINATED;
            return cellColour;
        }
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

    std::map<Animal, std::string> animal_strings;
    std::map<Animal, std::string> animal_emoji;

public:
    PlatypusGame(PlatypusMachine m) : machine(m)
    {
        for (int i = 0; i < boardSize; i++)
            board[i] = YELLOW;
        animal_strings[KANGAROO] = "\"Kangaroo\"";
        animal_strings[WOMBAT] = "\"Wombat\"";
        animal_strings[EMU] = "\"Emu\"";
        animal_strings[PLATYPUS] = "\"Platypus\"";

        animal_emoji[KANGAROO] = "🦘";
        animal_emoji[WOMBAT] = "🦔";
        animal_emoji[EMU] = "🦃";
        animal_emoji[PLATYPUS] = "🦫";
    }

    void setboard(const std::string_view &str)
    {
        for (int i = 0; i < 21; ++i)
        {
            board[i] = (Colour)str[i];
        }
    }

    int play(bool print = false)
    {
        // std::cout << "Playing platypus game\n";

        for (int move = 0; machine.getAnimal() != TERMINATED && move < 200; ++move)
        {
            const int pos = machine.getPosition() % 20; // Loops

            // Print the board
            if (print)
            {
                printf("%*s%s\n", pos * 2, "", animal_emoji[machine.getAnimal()].c_str());
                for (Colour c : board)
                {
                    std::cout << (c == YELLOW ? "🟨" : "🟩");
                }
                std::cout << " | Animal: " << animal_strings[machine.getAnimal()] << "\t| Points: " << points << " | Move: " << move << '\n';
                // Print a position marker
                printf("%*s☝️\n\n", pos * 2, "");
            }

            // Do the move
            Colour colour = machine.doTurn(board[pos]);
            if (board[pos] != colour)
            {
                points++;
                board[pos] = colour;
            }

            if (machine.getAnimal() == TERMINATED)
            {
                // std::cout << "Machine terminated in " << move << " moves\n";
                return move;
            }
        }
        return -1;
    }
};

void generateStrings(std::string current, int remaining, const std::function<void(const std::string &)> &process)
{
    if (remaining == 0)
    {
        process(current);
        return;
    }

    generateStrings(current + 'Y', remaining - 1, process);
    generateStrings(current + 'G', remaining - 1, process);
}

int main()
{
    // TransitionTable trans;

    // trans[Transition(KANGAROO,YELLOW)] = Action(EMU,GREEN,GHOST_GUM);
    // trans[Transition(KANGAROO,GREEN)] = Action(WOMBAT,YELLOW,WATTLE);

    // trans[Transition(EMU,YELLOW)] = Action(PLATYPUS,YELLOW,GHOST_GUM);
    // trans[Transition(EMU,GREEN)] = Action(KANGAROO,YELLOW,GHOST_GUM);

    // trans[Transition(WOMBAT,YELLOW)] = Action(EMU,GREEN,GHOST_GUM);
    // trans[Transition(WOMBAT,GREEN)] = Action(WOMBAT,GREEN,GHOST_GUM);

    // trans[Transition(PLATYPUS,YELLOW)] = Action(KANGAROO,GREEN,WATTLE);

    // platypus(1, [t(y,k,g,e,gg),t(g,k,y,w,wa),t(y,e,y,p,gg),t(g,e,y,k,gg),t(y,w,g,w,gg),t(g,w,g,w,gg),t(y,p,g,k,wa)]).

    // Generate a random platypus machine.
    TransitionTable trans;
    regenerate:
    std::vector<Animal> animals = {EMU, EMU, WOMBAT, WOMBAT, KANGAROO, KANGAROO, PLATYPUS};

    srand(static_cast<unsigned int>(time(0))); // seed
    int animal;

    animal = rand() % animals.size();
    trans[Transition(KANGAROO,YELLOW)] = Action(animals[animal],rand() % 2 ? GREEN : YELLOW,rand() % 2 ? WATTLE : GHOST_GUM);
    animals.erase(animals.begin() + animal);

    animal = rand() % animals.size();
    trans[Transition(KANGAROO,GREEN)] = Action(animals[animal],rand() % 2 ? GREEN : YELLOW,rand() % 2 ? WATTLE : GHOST_GUM);
    animals.erase(animals.begin() + animal);

    animal = rand() % animals.size();
    trans[Transition(EMU,YELLOW)] = Action(animals[animal],rand() % 2 ? GREEN : YELLOW,rand() % 2 ? WATTLE : GHOST_GUM);
    animals.erase(animals.begin() + animal);
    
    animal = rand() % animals.size();
    trans[Transition(EMU,GREEN)] = Action(animals[animal],rand() % 2 ? GREEN : YELLOW,rand() % 2 ? WATTLE : GHOST_GUM);
    animals.erase(animals.begin() + animal);

    animal = rand() % animals.size();
    trans[Transition(WOMBAT,YELLOW)] = Action(animals[animal],rand() % 2 ? GREEN : YELLOW,rand() % 2 ? WATTLE : GHOST_GUM);
    animals.erase(animals.begin() + animal);
    
    animal = rand() % animals.size();
    trans[Transition(WOMBAT,GREEN)] = Action(animals[animal],rand() % 2 ? GREEN : YELLOW,rand() % 2 ? WATTLE : GHOST_GUM);
    animals.erase(animals.begin() + animal);

    animal = rand() % animals.size();
    trans[Transition(PLATYPUS,YELLOW)] = Action(animals[animal],rand() % 2 ? GREEN : YELLOW,rand() % 2 ? WATTLE : GHOST_GUM);
    animals.erase(animals.begin() + animal);

    int min = 99999999;
    std::string min_string;
    int duplicates = 0;
    PlatypusMachine machine(trans);
    generateStrings("", 21, [&min, &min_string, &machine, &duplicates](const std::string &s)
                    {
        PlatypusGame game(machine);
        game.setboard(s);
        const int m = game.play();
        if (m < min)
        {
            min = m;
            min_string = s;
            duplicates = 0;
        }
        if (m == min) duplicates++; });

    //std::cout << "best answer is " << min_string << " in " << min << " with " << duplicates << " other solutions\n";
    if (min == -1) { std::cout << "Can't find a solution! Retrying...\n" ;goto regenerate;}

    // PlatypusMachine machine(trans);

    // Print some info
    std::cout << "Welcome to the platypus termination game. These are the transitions for this machine:\n\n";

    // Show the machine:
    std::map<Animal, std::string> animal_strings;
    animal_strings[KANGAROO] = "Kangaroo";
    animal_strings[WOMBAT] = "Wombat";
    animal_strings[EMU] = "Emu";
    animal_strings[PLATYPUS] = "Platypus";

    for (const auto& pair : trans)
    {
        const Transition t = pair.first;
        const Action a = pair.second;

        std::cout << animal_strings[t.first] << " + " << (t.second == YELLOW ? "Yellow" : "Green") << " --> Become " << animal_strings[a.animal] 
        << ", make cell " << (a.colour == YELLOW ? "Yellow" : "Green") << " and move to the " << (a.tree == GHOST_GUM ? "Ghost Gum (Right)" : "Wattle (Left)") << '\n';
    }

    // Print some more info
    std::cout << "\nType 21 colours, 'Y' or 'G' to input a board. The computer found an optimal solution in " << min << " moves. Good luck!\n";

    // Get input:
    std::string player_input;

    for (;;)
    {
        std::getline(std::cin, player_input);

        // Only Y or G allowed
        bool legalchars = true;
        for (const char& c : player_input)
            if (c != YELLOW && c != GREEN) { std::cerr << "Invalid characters, 'Y' and 'G' only.\n"; legalchars = false; break; }

        if (!legalchars) continue;

        // 21 chars check
        if (player_input.length() != 21) { std::cerr << "Input must have 21 cells.\n"; continue; }
        else {break;} // End the loop
    }

    PlatypusGame game(machine);
    game.setboard("GGGYYYYGYGGGGGGYYYYYY");
    int f = game.play(true);

    std::cout << "You ended the platypus game in " << f << " moves.\nThis is the optimal solution:\n";

    PlatypusGame game2(machine);
    game2.setboard(min_string);
    game2.play(true);

    std::cout << "The optimal solution was '" << min_string << "' ending in " << min << " moves.\n";
    std::cout << "Final score: " << std::roundf(100.0f * ((float)min / (float)f)) << "\n";

    return 0;
}