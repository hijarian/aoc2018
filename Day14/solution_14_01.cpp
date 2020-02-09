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

int main(int argc, char **argv)
{
  if (argc < 1)
  {
    std::cout << "Please provide the number of recipes to skip." << std::endl;

    return 1;
  }
  
  int skip_recipes = std::stoi(argv[1]);

  std::cout << "will skip " << skip_recipes << " recipes" << std::endl;

  std::list<int> recipes{3, 7};
  int recipes_amount = 2;
  std::list<int>::iterator last_recipe = recipes.end();
  std::list<int>::iterator first_elf = recipes.begin();
  std::list<int>::iterator second_elf = recipes.begin();
  ++second_elf;

  print_recipes(recipes, first_elf, second_elf);

  while (recipes_amount < skip_recipes + 10)
  {
    int new_recipe = *first_elf + *second_elf;
    if (new_recipe < 10)
    {
      recipes.push_back(new_recipe);
      recipes_amount += 1;
      if (recipes_amount == skip_recipes)
      {
        last_recipe = recipes.end();
        --last_recipe;
      }
    }
    else
    {
      int first_digit = new_recipe % 100 / 10;
      int second_digit = new_recipe % 10;
      recipes.push_back(first_digit);
      
      recipes_amount += 1;
      if (recipes_amount == skip_recipes)
      {
        last_recipe = recipes.end();
        --last_recipe;
      }

      recipes.push_back(second_digit);
      recipes_amount += 1;
      if (recipes_amount == skip_recipes)
      {
        last_recipe = recipes.end();
        --last_recipe;
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

    if (skip_recipes < 20)
    {
      print_recipes(recipes, first_elf, second_elf);
    }
  }

  ++last_recipe;
  std::cout << "Last 10 recipes: ";
  for (int i = 0; i < 10; ++i)
  {
    std::cout << *last_recipe;
    ++last_recipe;
  }
  std::cout << std::endl;

  return 0;
}