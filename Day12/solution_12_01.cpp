#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <stdexcept>

std::pair<std::string, std::map<std::string, char>> read_input(std::string filename)
{
  std::ifstream inputfile (filename);
  if (!inputfile.is_open())
  {
    throw std::invalid_argument("Unable to open file");
  }

  std::string target;
  std::map<std::string, char> rules;

  std::string line;
  std::getline(inputfile, line);
  target = line.substr(15);
  std::getline(inputfile, line);

  while ( inputfile.good() )
  {
    std::getline(inputfile, line);
    if (line.length() < 1)
      continue;
      
    rules[line.substr(0, 5)] = line.substr(9).at(0); 
  }
  inputfile.close(); 

  return std::pair<std::string, std::map<std::string, char>>{target, rules};
}

void print_rules(std::map<std::string, char> rules)
{
  for (const auto &rule : rules) 
    std::cout << rule.first << ": " << rule.second << std::endl;
}

void print_target(std::string target)
{
  std::cout << target << "(" << target.length() << " symbols)" << std::endl;
}

std::string sample(std::string input, size_t point)
{
  if (point == 0)
  {
    return std::string{".."}.append(input.substr(0, 3));
  }
  if (point == 1)
  {
    return std::string{"."}.append(input.substr(0, 4));
  }
  if (point == input.length() - 1)
  {
    return input.substr(input.length() - 3).append("..");
  }
  if (point == input.length() - 2)
  {
    return input.substr(input.length() - 4).append(".");
  }
  if (point > 1 && point < input.length() - 2)
  {
    return input.substr(point - 2, 5);
  }
}

void print_sample(std::string input, size_t point)
{
    std::cout << point << ": " << sample(input, point) << std::endl;
}

void print_sanity_test(std::string target, std::map<std::string, char> rules)
{
  print_target(target);
  print_rules(rules);

  print_sample(target, 0);
  print_sample(target, 1);
  print_sample(target, 2);
  print_sample(target, 3);
  print_sample(target, 4);
  print_sample(target, 5);
  print_sample(target, 96);
  print_sample(target, 97);
  print_sample(target, 98);
}

int count_value(const std::string world, const int origin)
{
  int point = -origin;
  int value = 0;
  for(const auto &letter : world)
  {
    if (letter == '#')
    {
      value += point;
    }
    ++point;
  }
  return value;
}

void print_world(int generation, std::string world, int origin)
{
  std::cout << generation << " " << world << " " << count_value(world, origin) << std::endl;  
}


std::string advance(std::string population, std::map<std::string, char> rules)
{
  std::string next_generation;
  for (size_t point = 0; point < population.length(); ++point)
  {
    next_generation.append(std::string{rules[sample(population, point)]});
  }

  return next_generation;
}

int main () {
  std::string population;
  std::map<std::string, char> rules;
  try 
  {
    std::tie(population, rules) = read_input("C:/Users/somebodyrotten/projects/AoC2018/input_12.txt");
  }
  catch (const std::exception &problem)
  {
    std::cout << problem.what() << std::endl;
    return 1;
  }

  print_rules(rules);
  std::string world{"...................."};
  world.append(population);
  world.append("..............................................................................................................................................");
  std::cout << "                    v" << std::endl;
  print_world(0, world, 20);

  for (int i = 1; i < 300; ++i)
  {
    world = advance(world, rules);
    print_world(i, world, 20);
  }

  return 0;
}