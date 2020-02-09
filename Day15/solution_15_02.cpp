#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <stdexcept>
#include <vector>
#include <curses.h>
#include <type_traits>
#include <algorithm>
#include <queue>

bool skipRender{false};

struct Unit
{
  unsigned row;
  unsigned col;
  char representation;
  int hp{200};
  int damage{3};
  Unit(unsigned row, unsigned col, char representation) : row{row}, col{col}, representation{representation} {};
};

struct Map
{
  std::vector<std::string> terrain;
  std::vector<Unit> units;
  void sort_units();
  std::vector<std::string> get_merged();
  int count_total_hp();
  bool damage(unsigned row, unsigned col);
};

int elves_damage = 3;
const int goblin_damage = 3;

/**
 * @return Whether an Elf died.
 */
bool Map::damage(unsigned row, unsigned col)
{
  for (Unit &unit : this->units)
  {
    if (unit.row == row && unit.col == col)
    {
      unit.hp -= (unit.representation == 'G' ? elves_damage : goblin_damage);

      return unit.hp < 1 && unit.representation == 'E';
    }
  }
  return false;
}

int Map::count_total_hp()
{
  int sum{0};
  for (const auto &unit : this->units)
  {
    if (unit.hp > 0 && unit.representation == 'E')
    {
      sum += unit.hp;
    }
  }

  return sum;
}

void Map::sort_units()
{
  std::sort(
      this->units.begin(),
      this->units.end(),
      [](const Unit &left, const Unit &right) {
        if (left.row == right.row)
          return left.col < right.col;

        return left.row < right.row;
      });
}

std::vector<std::string> Map::get_merged()
{
  std::vector<std::string> result = this->terrain;
  for (const auto &unit : this->units)
  {
    if (unit.hp < 1)
    {
      continue;
    }

    result[unit.row][unit.col] = unit.representation;
  }

  return result;
}

Map read_map(std::string filepath)
{
  std::ifstream inputfile{filepath};

  if (!inputfile.is_open())
  {
    throw std::invalid_argument{"Unable to open file!"};
  }

  Map result{};

  std::string line;
  unsigned row{0};
  unsigned col{0};
  while (inputfile.good())
  {
    std::getline(inputfile, line);
    if (line == "")
    {
      continue;
    }

    col = 0;
    for (auto symbol = line.begin(); symbol != line.end(); ++symbol)
    {
      if (*symbol == 'E' || *symbol == 'G')
      {
        result.units.push_back(Unit(row, col, *symbol));
        *symbol = '.';
      }
      ++col;
    }
    result.terrain.push_back(line);
    ++row;
  }

  result.sort_units();

  return result;
}

void print_units(std::vector<Unit> units)
{
  for (const auto &unit : units)
  {
    if (unit.hp < 1)
    {
      continue;
    }

    auto color = unit.representation == 'E' ? COLOR_PAIR(2) : COLOR_PAIR(1);
    mvaddch(unit.row, unit.col, unit.representation | color);
  }
}

void print_terrain(std::vector<std::string> terrain)
{
  for (const auto &line : terrain)
  {
    printw("%s\n", line.c_str());
  }
}

void log_units(const unsigned row_offset, const unsigned col_offset, const std::vector<Unit> &units)
{
  unsigned current_row{row_offset};
  std::string title;
  for (auto &unit : units)
  {
    auto color = unit.representation == 'E' ? COLOR_PAIR(2) : COLOR_PAIR(1);
    title = unit.representation == 'E' ? "Elf" : "Goblin";
    attron(color);
    mvprintw(current_row, col_offset, "%6s (%2d, %2d) HP: %3d", title.c_str(), unit.col, unit.row, unit.hp);
    attroff(color);
    ++current_row;
  }
}

void print_round_number(unsigned round_number)
{
  mvprintw(33, 0, "Elves damage: %2d   Round number: %2d", elves_damage, round_number);
}

void print_map(const Map &map, unsigned round_number)
{
  if (!skipRender)
  {
    clear();
    print_terrain(map.terrain);
    print_units(map.units);
    unsigned row_offset{0};
    unsigned col_offset{map.terrain[0].size() + 5};
    log_units(row_offset, col_offset, map.units);
  }
  print_round_number(round_number);
  refresh();
}

void print_unit_action(unsigned row, unsigned col, const Unit &unit, const std::string action)
{
  if (skipRender)
    return;

  mvprintw(
      row,
      col,
      "%5s (%2d, %2d): %s",
      unit.representation == 'E' ? "Elf" : "Goblin",
      unit.col,
      unit.row,
      action.c_str());
}

void prepare_screen()
{
  initscr();

  // @see http://tldp.org/HOWTO/NCURSES-Programming-HOWTO/color.html
  start_color();
  init_pair(1, COLOR_RED, COLOR_BLACK);
  init_pair(2, COLOR_GREEN, COLOR_BLACK);

  init_pair(3, COLOR_RED, COLOR_YELLOW);
  init_pair(4, COLOR_GREEN, COLOR_YELLOW);
  init_pair(5, COLOR_WHITE, COLOR_YELLOW);

  init_pair(6, COLOR_RED, COLOR_BLUE);
  init_pair(7, COLOR_GREEN, COLOR_BLUE);
  init_pair(8, COLOR_WHITE, COLOR_BLUE);

  noecho();
  curs_set(0);

  clear();
}

std::vector<Unit> find_all_enemies(const Unit &unit, const std::vector<Unit> &units)
{
  std::vector<Unit> result{};
  for (const auto &other : units)
  {
    if ((other.representation != unit.representation) && (other.hp > 0))
    {
      result.push_back(other);
    }
  }
  return result;
}

auto get_target_cell_color(char symbol)
{
  if (symbol == 'E')
    return COLOR_PAIR(4);
  if (symbol == 'G')
    return COLOR_PAIR(3);

  return COLOR_PAIR(5);
}

auto get_path_cell_color(char symbol)
{
  if (symbol == 'E')
    return COLOR_PAIR(6);
  if (symbol == 'G')
    return COLOR_PAIR(7);

  return COLOR_PAIR(8);
}

void paint_target_cells(const std::vector<std::pair<unsigned, unsigned>> &target_cells)
{
  char current_char;
  for (const auto &coordinates : target_cells)
  {
    current_char = mvinch(coordinates.first, coordinates.second) & A_CHARTEXT;
    mvaddch(coordinates.first, coordinates.second, current_char | get_target_cell_color(current_char));
  }
}

void paint_path(const std::vector<std::pair<unsigned, unsigned>> &path_cells)
{
  char current_char;
  for (const auto &coordinates : path_cells)
  {
    current_char = mvinch(coordinates.first, coordinates.second) & A_CHARTEXT;
    mvaddch(coordinates.first, coordinates.second, current_char | get_path_cell_color(current_char));
  }
}

std::vector<bool> make_mask(const std::vector<std::string> &merged_map)
{
  unsigned width = merged_map[0].size();
  std::vector<bool> is_passable(merged_map.size() * width);
  unsigned row{0};
  unsigned col;

  for (const auto &line : merged_map)
  {
    col = 0;
    for (const auto &cell : line)
    {
      is_passable[row * width + col] = (cell == '.');
      ++col;
    }
    ++row;
  }

  return is_passable;
}

std::vector<std::pair<int, int>> offsets{
    std::pair<int, int>{-1, 0},
    std::pair<int, int>{0, -1},
    std::pair<int, int>{0, 1},
    std::pair<int, int>{1, 0}};

char location;

using coordinates_t = std::pair<unsigned, unsigned>;
using path_t = std::vector<coordinates_t>;
using queue_candidate_t = std::pair<coordinates_t, path_t>;

path_t find_shortest_path(
    const Unit &current_unit,
    const coordinates_t &target_cell,
    const std::vector<std::string> &merged_map)
{

  // char target_char = mvinch(target_cell.first, target_cell.second) & A_CHARTEXT;
  // mvaddch(target_cell.first, target_cell.second, target_char | COLOR_PAIR(6));

  std::vector<bool> is_passable = make_mask(merged_map);
  unsigned width = merged_map[0].size();
  path_t path{};

  std::queue<queue_candidate_t> candidates{};
  candidates.push(
      queue_candidate_t{
          coordinates_t{current_unit.row, current_unit.col},
          path_t{coordinates_t{current_unit.row, current_unit.col}}});

  queue_candidate_t candidate;
  unsigned row;
  unsigned col;

  while (!candidates.empty())
  {
    candidate = candidates.front();
    coordinates_t candidate_coordinates = candidate.first;
    path_t candidate_path = candidate.second;
    if (candidate_coordinates.first == target_cell.first && candidate_coordinates.second == target_cell.second)
    {
      return candidate_path;
    }

    candidates.pop();
    // mvaddch(candidate_coordinates.first, candidate_coordinates.second, '.');
    for (const auto &offset : offsets)
    {
      row = candidate.first.first + offset.first;
      col = candidate.first.second + offset.second;
      location = merged_map[row][col];
      if (location == '.' && is_passable[row * width + col])
      {
        coordinates_t new_coordinates{row, col};
        path_t new_path = candidate_path;
        new_path.push_back(new_coordinates);
        if (new_coordinates.first == target_cell.first && new_coordinates.second == target_cell.second)
        {
          return new_path;
        }
        // mvaddch(row, col, 'x');
        // getch();
        queue_candidate_t new_candidate{new_coordinates, new_path};
        candidates.push(new_candidate);
        is_passable[row * width + col] = false;
      }
    }
  }

  return path;
}

int main(int argc, char **argv)
{
  bool skipTurns{true};
  bool skipRounds{false};
  char command{' '};

  std::string inputfilename{"./input_15.txt"};
  if (argc > 1)
  {
    inputfilename = std::string{argv[1]};
  }

  if (argc > 2)
  {
    skipRender = true;
  }

  Map initial_map = read_map(inputfilename);
  prepare_screen();

  unsigned round_number{0};
  bool is_battle_ended = false;
  bool is_elves_victory = false;
  Map map;

  while (!is_elves_victory)
  {
    ++elves_damage;
    round_number = 0;
    is_battle_ended = false;
    map = initial_map;

    // in each round
    while (!is_battle_ended)
    {
      print_map(map, round_number);

      if (skipRounds == false)
      {
        command = getch();
        if (command == 'q')
        {
          goto end;
        }
        if (command == 's')
        {
          skipRounds = true;
        }
        if (command == 'v')
        {
          skipTurns = false;
        }
      }

      // every turn:
      for (Unit &current_unit : map.units)
      {
        if (skipTurns == false)
        {
          command = getch();
          if (command == 'q')
          {
            goto end;
          }
          if (command == 's')
          {
            skipRounds = false;
            skipTurns = true;
          }
        }

        // 0. Check if this unit is dead
        if (current_unit.hp < 1)
        {
          continue;
        }

        print_map(map, round_number);

        // current turn unit
        // gets all enemies.
        std::vector<Unit> enemies = find_all_enemies(current_unit, map.units);

        // 1. Check whether we won:
        // if no enemies we stop: return number of rounds passed (!) multiplied by sum of leftover units HP.
        if (enemies.size() < 1)
        {
          print_unit_action(34, 0, current_unit, "Victory! No enemies found");
          mvprintw(35, 0, "Final result: %d", map.count_total_hp() * round_number);
          is_battle_ended = true;
          is_elves_victory = current_unit.representation == 'E';
          break;
        }

        std::vector<std::string> merged_map = map.get_merged();

        // 2. Attack if possible:
        // checks cells in range
        bool elfMeetsGoblin{false};
        bool goblinMeetsElf{false};
        bool canMove{false};
        std::vector<Unit> enemies_in_range{};
        unsigned enemy_row;
        unsigned enemy_col;
        for (const auto &offset : offsets)
        {
          enemy_row = current_unit.row + offset.first;
          enemy_col = current_unit.col + offset.second;
          location = merged_map[enemy_row][enemy_col];
          // if enemy in range, attack it (reduce its HP by 3)
          elfMeetsGoblin = current_unit.representation == 'E' && location == 'G';
          goblinMeetsElf = current_unit.representation == 'G' && location == 'E';
          if (elfMeetsGoblin || goblinMeetsElf)
          {
            for (const auto &enemy : map.units)
            {
              if (enemy.hp < 1)
              {
                continue;
              }
              if (enemy.row == enemy_row && enemy.col == enemy_col)
              {
                enemies_in_range.push_back(enemy);
              }
            }
          }
          if (location == '.')
          {
            canMove = true;
          }
        }

        if (!enemies_in_range.empty())
        {
          int min_enemy_hp = 100000;
          unsigned min_enemy_hp_row;
          unsigned min_enemy_hp_col;
          for (const auto &enemy : enemies_in_range)
          {
            if (enemy.hp < min_enemy_hp)
            {
              min_enemy_hp = enemy.hp;
              min_enemy_hp_row = enemy.row;
              min_enemy_hp_col = enemy.col;
            }
          }
          print_unit_action(34, 0, current_unit, std::string{"Will attack enemy at ("} + std::to_string(min_enemy_hp_col) + "," + std::to_string(min_enemy_hp_row) + ")");
          if (map.damage(min_enemy_hp_row, min_enemy_hp_col) == true)
          {
            is_battle_ended = true;
            break;
          }
          // then pass turn to next unit.
          continue;
        }

        // 3. (guard case) Check whether we can move at all:
        if (!canMove)
        {
          print_unit_action(34, 0, current_unit, "Can't move, skip turn");
          continue;
        }

        print_unit_action(34, 0, current_unit, "Will move!");

        // 4. Move
        std::vector<std::pair<unsigned, unsigned>> target_cells{};
        // if no enemy in range
        //   for every enemy:
        for (const auto &enemy : enemies)
        {
          //     collest adjacent free cells
          for (const auto &offset : offsets)
          {
            location = merged_map[enemy.row + offset.first][enemy.col + offset.second];
            if (location == '.')
            {
              target_cells.push_back(std::pair<unsigned, unsigned>{enemy.row + offset.first, enemy.col + offset.second});
            }
          }
        }

        // if no adjacent free cells collected pass turn to next unit
        if (target_cells.size() < 1)
        {
          print_unit_action(34, 0, current_unit, "No place to move to, skip turn");
          continue;
        }

        //paint_target_cells(target_cells);

        std::vector<std::vector<std::pair<unsigned, unsigned>>> shortest_paths;

        // for every candidate free cell:
        std::vector<std::pair<unsigned, unsigned>> path;
        for (const auto &cell : target_cells)
        {
          //   find shortest path
          path = find_shortest_path(current_unit, cell, merged_map);
          //   if no shortest path disregard this cell
          if (path.size() > 0)
          {
            shortest_paths.push_back(path);
          }
        }

        // if no cells with shortest paths pass turn to next unit
        if (shortest_paths.size() < 1)
        {
          print_unit_action(34, 0, current_unit, "Can't move to target cells, skip turn");
          continue;
        }

        std::sort(
            shortest_paths.begin(),
            shortest_paths.end(),
            [](const auto &left, const auto &right) {
              if (left.size() == right.size())
              {
                const auto left_step = *(left.begin() + 1);
                const auto right_step = *(right.begin() + 1);
                if (left_step.first == right_step.first)
                {
                  return left_step.second < right_step.second;
                }
                return left_step.first < right_step.first;
              }
              return left.size() < right.size();
            });

        path = shortest_paths[0];

        // take first step in this shortest path.
        if (!skipRender && !skipTurns)
        {
          paint_path(path);
        }

        std::pair<unsigned, unsigned> step = *(path.begin() + 1);
        print_unit_action(34, 0, current_unit, std::string{"Will move to "} + std::to_string(step.second) + "," + std::to_string(step.first) + "\n");
        if (!skipRender)
        {
          for (auto step = path.begin(); step != path.end(); ++step)
          {
            printw("(%2d, %2d) ", step->first, step->second);
          }
        }

        // update coordinates of current unit in map
        current_unit.row = step.first;
        current_unit.col = step.second;

        // 5. Attack after move if possible:
        // checks cells in range
        for (const auto &offset : offsets)
        {
          enemy_row = current_unit.row + offset.first;
          enemy_col = current_unit.col + offset.second;
          location = merged_map[enemy_row][enemy_col];
          // if enemy in range, attack it (reduce its HP by 3)
          elfMeetsGoblin = current_unit.representation == 'E' && location == 'G';
          goblinMeetsElf = current_unit.representation == 'G' && location == 'E';
          if (elfMeetsGoblin || goblinMeetsElf)
          {
            for (const auto &enemy : map.units)
            {
              if (enemy.hp < 1)
              {
                continue;
              }
              if (enemy.representation == current_unit.representation)
              {
                continue;
              }
              if (enemy.row == enemy_row && enemy.col == enemy_col)
              {
                enemies_in_range.push_back(enemy);
              }
            }
          }
        }

        if (!enemies_in_range.empty())
        {
          int min_enemy_hp = 100000;
          unsigned min_enemy_hp_row;
          unsigned min_enemy_hp_col;
          for (const auto &enemy : enemies_in_range)
          {
            if (enemy.hp < min_enemy_hp)
            {
              min_enemy_hp = enemy.hp;
              min_enemy_hp_row = enemy.row;
              min_enemy_hp_col = enemy.col;
            }
          }
          print_unit_action(34, 0, current_unit, std::string{"Will attack enemy at ("} + std::to_string(min_enemy_hp_col) + "," + std::to_string(min_enemy_hp_row) + ")");
          if (map.damage(min_enemy_hp_row, min_enemy_hp_col) == true)
          {
            is_battle_ended = true;
            break;
          }
        }
      }
      // This is important: remove the dead bodies, because otherwise there'll be problems with targeting.
      map.units.erase(
          std::remove_if(
              map.units.begin(),
              map.units.end(),
              [](const Unit &unit) { return unit.hp < 1; }),
          map.units.end());
      
      // This is important: units take turns according to their sorting order.
      map.sort_units();

      ++round_number;
    }
  }
  getch();
end:
  endwin();
  return 0;
}