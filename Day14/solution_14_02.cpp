#include <string>
#include <iostream>
#include <list>
#include <iterator>

void print_recipes(
    const std::list<int> &recipes,
    const std::list<int>::iterator &first_elf,
    const std::list<int>::iterator &second_elf)
{
  for (auto recipe = recipes.begin(); recipe != recipes.end(); ++recipe)
  {
    if (recipe == first_elf)
    {
      std::cout << "[" << *recipe << "] ";
    }
    else if (recipe == second_elf)
    {
      std::cout << "(" << *recipe << ") ";
    }
    else
    {
      std::cout << *recipe << " ";
    }
  }
  std::cout << std::endl;
}

bool match(std::list<int>::iterator position, const std::list<int> &pattern)
{
  bool matches = true;
  for (std::list<int>::const_iterator pattern_position = pattern.begin(); pattern_position != pattern.end(); ++pattern_position)
  {
    if (*position != *pattern_position)
    {
      matches = false;
      break;
    }

    ++position;
  }

  return matches;
}

int main(int argc, char **argv)
{
  if (argc < 1)
  {
    std::cout << "Please provide the pattern of recipes to match." << std::endl;

    return 1;
  }
  std::string pattern_string{argv[1]};

  std::list<int> pattern{};
  for (const auto pattern_char : pattern_string)
  {
    pattern.push_back(pattern_char - '0');
  }
  if (pattern.size() > 7)
  {
    std::cout << "Sorry, can't match patterns larger than 7 elements long" << std::endl;
    return 2;
  }

  std::cout << "will find the position of the following pattern: ";
  for (auto recipe : pattern)
  {
    std::cout << recipe << " ";
  }
  std::cout << std::endl;

  std::list<int> recipes{3, 7};
  unsigned recipes_amount = 2;
  std::list<int>::iterator first_elf = recipes.begin();
  std::list<int>::iterator second_elf = recipes.begin();
  ++second_elf;
  unsigned pattern_match_position = 0;
  std::list<int>::iterator pattern_match_recipe = recipes.begin();

  print_recipes(recipes, first_elf, second_elf);
  // std::cout << "pattern match recipe is " << *pattern_match_recipe << " (" << (match(pattern_match_recipe, pattern) ? "matches" : "does not match") << ")" << std::endl;
  // std::cout << "pattern match recipe is " << *pattern_match_recipe << " now" << std::endl;

  while (true)
  {
    int new_recipe = *first_elf + *second_elf;
    if (new_recipe < 10)
    {
      recipes.push_back(new_recipe);
      ++recipes_amount;
      if (recipes_amount > pattern.size())
      {
        ++pattern_match_recipe;
        ++pattern_match_position;
        if (match(pattern_match_recipe, pattern))
        {
          // std::cout << "found match at position " << pattern_match_position << " when total amount of recipes reached " << recipes_amount << std::endl;
          break;
        }
      }
    }
    else
    {
      int first_digit = new_recipe % 100 / 10;
      int second_digit = new_recipe % 10;
      recipes.push_back(first_digit);
      ++recipes_amount;
      if (recipes_amount > pattern.size())
      {
        ++pattern_match_recipe;
        ++pattern_match_position;
        if (match(pattern_match_recipe, pattern))
        {
          // std::cout << "found match at position " << pattern_match_position << " when total amount of recipes reached " << recipes_amount << std::endl;
          break;
        }
      }

      recipes.push_back(second_digit);
      ++recipes_amount;
      if (recipes_amount > pattern.size())
      {
        ++pattern_match_recipe;
        ++pattern_match_position;
        if (match(pattern_match_recipe, pattern))
        {
          // std::cout << "found match at position " << pattern_match_position << " when total amount of recipes reached " << recipes_amount << std::endl;
          break;
        }
      }
    }

    int first_elf_steps =  1 + *first_elf;
    for (int steps = 0; steps < first_elf_steps; ++steps)
    {
      ++first_elf;
      if (first_elf == recipes.end())
      {
        first_elf = recipes.begin();
      }
    }

    int second_elf_steps =  1 + *second_elf;
    for (int steps = 0; steps < second_elf_steps; ++steps)
    {
      ++second_elf;
      if (second_elf == recipes.end())
      {
        second_elf = recipes.begin();
      }
    }

    if (recipes_amount < 20)
    {
      print_recipes(recipes, first_elf, second_elf);
    }
    // std::cout << "recipes amount: " << recipes_amount << ", pattern match recipe is " << *pattern_match_recipe << " at position " << pattern_match_position << " (" << (match(pattern_match_recipe, pattern) ? "matches" : "does not match") << ")" << std::endl;

    if (recipes_amount > 1000000000)
    {
      std::cout << "Too many recipes watched to no avail ;(" << std::endl;
      break;
    }
  }
  std::cout << "found match at position " << pattern_match_position << " when total amount of recipes reached " << recipes_amount << std::endl;

  return 0;
}