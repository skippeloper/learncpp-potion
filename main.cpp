#include <array>
#include <iostream>
#include <string>
#include <vector>

#include "Random.h"

namespace Potion
{
/**
 * Type of potion.
 */
enum Type
{
    healing,
    mana,
    speed,
    invisibility,
    strength,
    maxPotions
};

/**
 * List of enumerables of `Type`.
 */
constexpr std::array<Type, maxPotions> types{healing, mana, speed, invisibility,
                                             strength};

/**
 * List of potions names.
 */
constexpr std::array<std::string_view, maxPotions> potions{
    "healing", "mana", "speed", "invisibility", "strength"};

/**
 * List of potion costs. Each index relates to the cost of the potion listed in
 * names.
 */
constexpr std::array<int, maxPotions> costs{20, 30, 12, 50, 43};

static_assert(types.size() == maxPotions);
static_assert(potions.size() == maxPotions);
static_assert(costs.size() == maxPotions);

/**
 * Print all available potions and their costs.
 */
void shop()
{
    std::cout << "Here is our selection for today:\n";
    for (auto type : types)
    {
        std::cout << type << ") " << potions[type] << " costs " << costs[type]
                  << '\n';
    }
}

/**
 * Ignore a line of input stream.
 */
void ignoreLine()
{
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

/**
 * Convert a character to it's corresponding int value.
 *
 * @param c character to convert
 * @returns int value
 */
int charNumToInt(char c) { return c - '0'; }

/**
 * Get input from the user as to which potion they want to purchase.
 *
 * @returns Type of the potion they want to purchase.
 */
Type getPotion()
{
    char option{};
    while (true)
    {
        std::cout << "Which potion would you like to buy? Press q to quit: ";
        std::cin >> option;

        if (std::cin)
        {
            if (option == 'q') return maxPotions;

            int potionNumber{charNumToInt(option)};
            if (potionNumber >= maxPotions || potionNumber < 0)
            {
                ignoreLine();
                std::cout
                    << "Input was not a valid option. Please try again!\n";
            }
            else
                return types[potionNumber];
        }
        else
        {
            std::cin.clear();
            ignoreLine();
            std::cout
                << "Encountered error while reading input. Please try again!\n";
        }
    }
}
}  // namespace Potion

class Player
{
public:
    explicit Player(std::string_view name)
        : m_name{name},
          m_gold{Random::get(s_minStartingGold, s_maxStartingGold)}
    {
    }

    std::string_view getName() const { return m_name; }
    int getGold() const { return m_gold; }

    void buyPotion(Potion::Type type)
    {
        int cost{Potion::costs[type]};

        if (m_gold > cost)
        {
            m_gold -= cost;
            m_inventory.push_back(type);
            std::cout << "You have purchased " << Potion::potions[type]
                      << ". You now have " << m_gold << " gold.\n";
        }
        else
        {
            std::cout << "You do not have enough gold to purchase this item!\n";
        }
    }

    void printInventory() const
    {
        std::cout << "Your inventory contains: ";
        for (auto type : m_inventory)
        {
            std::cout << Potion::potions[type] << " ";
        }
        std::cout << "\n";
    }

private:
    static constexpr int s_minStartingGold{80};
    static constexpr int s_maxStartingGold{120};

    std::string m_name{};
    std::vector<Potion::Type> m_inventory{};
    int m_gold{};
};

int main()
{
    std::string name;
    std::cout << "Welcome to Roscoe's potion emporium!\n";
    std::cout << "Please enter your name: ";
    std::cin >> name;

    Player player{name};

    std::cout << "Hello, " << player.getName() << ", you have "
              << player.getGold() << '\n';
    Potion::shop();

    std::cout << "Thanks for shopping at Roscoe's potion emporium!\n";

    while (true)
    {
        Potion::Type type{Potion::getPotion()};

        if (type == Potion::maxPotions)
        {
            break;
        }
        else
        {
            player.buyPotion(type);
        }
    }

    player.printInventory();
}
