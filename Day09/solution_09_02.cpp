#include <list>
#include <map>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

int place_marble(
  std::list<int> &board,
  auto &current_marble_pointer,
  int new_marble_value
) {
  unsigned points = 0;

  if (new_marble_value % 23 == 0)
  {
    points += new_marble_value;
    int steps = 7;
    while (steps > 0)
    {
      if (current_marble_pointer == board.begin())
      {
        current_marble_pointer = board.end();
      }
      --current_marble_pointer;
      --steps;
    }
    points += *current_marble_pointer;

    current_marble_pointer = board.erase(current_marble_pointer);
  }
  else
  {
    int steps = 2;
    while (steps > 0)
    {
      if (current_marble_pointer == board.end())
      {
        current_marble_pointer = board.begin();
        --steps;
      }
      ++current_marble_pointer;
      --steps;
    }
    current_marble_pointer = board.insert(current_marble_pointer, new_marble_value);
  }

  return points;
}

void print_state(const int current_player, const std::list<int> &board, const auto &current_marble_pointer)
{
  std::cout << current_player << ':';

  for (auto it = board.begin(); it != board.end(); ++it)
  {
    if (it == current_marble_pointer)
    {
      std::cout << " (" << *it << ")";
    }
    else
    {
      std::cout << " " << *it;
    }
  }

  std::cout << std::endl; 
}

int main(int argc, char **argv)
{
  if (argc > 3)
  {
    std::cout << "Special case" << std::endl;
    std::list<int> board { 0, 16, 8, 4, 9, 2, 10, 5, 11, 1, 12, 6, 13, 3, 14, 7, 15 };
    std::list<int>::iterator current_marble_pointer = board.begin();
    ++current_marble_pointer; // marble 16

    print_state(0, board, current_marble_pointer);

    int points = place_marble(board, current_marble_pointer, 23);

    print_state(points, board, current_marble_pointer);

    return 2;
  }
  if (argc < 3)
  {
    std::cout << "First argument is number of players, second is last marble value.";
    return 1;
  }
  
  const int last_marble = std::stoi(argv[2]);
  const int number_of_players = std::stoi(argv[1]);

  std::list<int> board { 0, 2, 1, 3 };
  int current_player = 4;
  auto current_marble_pointer = board.begin();
  ++current_marble_pointer;
  ++current_marble_pointer;
  ++current_marble_pointer; // marble 3

  std::map<unsigned, unsigned> player_points;
  unsigned current_points;
  for (int i = 4; i <= last_marble; ++i)
  {
    current_points = place_marble(board, current_marble_pointer, i);
    if (current_points > 0)
    {
      auto current_player_points = player_points[current_player];
      player_points[current_player] = current_player_points += current_points;
    }

    if (last_marble < 50)
    {
      print_state(current_player, board, current_marble_pointer);
    }

    ++current_player;
    if (current_player > number_of_players)
    {
      current_player = 1;
    }
  }

  std::vector<std::pair<unsigned, int>> sorted_scores;
  for (auto it = player_points.begin(); it != player_points.end(); ++it)
  {
    sorted_scores.push_back(std::pair<unsigned, int>{it->second, it->first});
  }

  std::sort(sorted_scores.begin(), sorted_scores.end());

  for (auto it = sorted_scores.begin(); it != sorted_scores.end(); ++it)
  {
    std::cout << it->second << ": " << it->first << std::endl;
  }

  return 0;
}
