#include <curses.h>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>

using map_t = std::vector<char>;

#define WIDTH 50
#define HEIGHT 50
#define LUMBERYARD_CELL 1
#define WOOD_CELL 2
#define EMPTY_CELL 3

map_t read_map(std::string inputfilename)
{
  map_t result(WIDTH * HEIGHT);
  std::ifstream inputfile{inputfilename};

  std::string line{};
  unsigned row{0};
  unsigned col{0};
  while(inputfile.good())
  {
    std::getline(inputfile, line);
    col = 0;
    for (const char &symbol : line)
    {
      result[row * WIDTH + col] = symbol;
      ++col;
    }
    ++row;
  }

  return result;
}

void prepare_screen()
{
  initscr();

  // @see http://tldp.org/HOWTO/NCURSES-Programming-HOWTO/color.html
  start_color();
  init_pair(LUMBERYARD_CELL, COLOR_RED, COLOR_BLACK);
  init_pair(WOOD_CELL, COLOR_GREEN, COLOR_BLACK);
  init_pair(EMPTY_CELL, COLOR_CYAN, COLOR_BLACK);

  noecho();
  curs_set(0);

  clear();
}

void print_cell(const unsigned position, const char contents)
{
  unsigned row{position / WIDTH};
  unsigned col{position % WIDTH};

  chtype color = contents == '.' ? COLOR_PAIR(EMPTY_CELL) : contents == '|' ? COLOR_PAIR(WOOD_CELL) : COLOR_PAIR(LUMBERYARD_CELL);
  mvaddch(row, col, contents | color);
}

int count_symbols(const map_t &map, const char target_symbol)
{
  int count{0};
  for (const auto &symbol : map)
  {
    if (target_symbol == symbol)
    {
      ++count;
    }
  }

  return count;
}

void print_value(const unsigned minute, const map_t &map)
{
  int lumberyards_count = count_symbols(map, '#');
  int woods_count = count_symbols(map, '|');
  mvprintw(
    2,
    HEIGHT + 5,
    "Minute: %2d. Lumberyards: %3d. Woods: %3d. Total value: %d",
    minute,
    lumberyards_count,
    woods_count,
    lumberyards_count * woods_count
  );
}

void print_map(const map_t &map)
{
  unsigned i{0};
  for (auto it = map.begin(); it != map.end(); ++it)
  {
    print_cell(i, *it);
    ++i;
  }
}

using pos_t = std::pair<int, int>;

std::vector<std::pair<int, int>> offsets = {
  pos_t{-1, -1},
  pos_t{-1,  0},
  pos_t{-1,  1},
  pos_t{0, -1},
  pos_t{0, 1},
  pos_t{1, -1},
  pos_t{1, 0},
  pos_t{1, 1},
};

char calculate_new_cell(const int position, const map_t &map)
{
  int row = position / WIDTH;
  int col = position % WIDTH;

  int neighbor_row;
  int neighbor_col;

  char target_cell = map[position];
  // mvaddch(7, WIDTH + 2, target_cell);

  int lumbers_count{0};
  int empty_count{0};
  int woods_count{0};
  for (const auto &offset : offsets)
  {
    neighbor_row = row + offset.first;
    if (neighbor_row < 0 || neighbor_row >= HEIGHT)
    {
      continue;
    }
    neighbor_col = col + offset.second;
    if (neighbor_col < 0 || neighbor_col >= WIDTH)
    {
      continue;
    }
    switch (map[neighbor_row * WIDTH + neighbor_col])
    {
      case '.':
        ++empty_count;
        break;
      case '|':
        ++woods_count;
        break;
      case '#':
        ++lumbers_count;
        break;
    }

    // mvaddch(7 + offset.first, WIDTH + 2 + offset.second, map[neighbor_row * WIDTH + neighbor_col]);
  }

  char new_cell{};
  switch (target_cell)
  {
    case '.':
      new_cell = (woods_count > 2) ? '|' : '.';
      break;
    case '|':
      new_cell = (lumbers_count > 2) ? '#' : '|';
      break;
    case '#':
      new_cell = (lumbers_count > 0 && woods_count > 0) ? '#' : '.';
      break;
    default:
      new_cell = target_cell;
  }

  // mvprintw(3, WIDTH + 5, "(%2d,%2d): %dE %dW %dL", row, col, empty_count, woods_count, lumbers_count);
  // mvprintw(6, WIDTH + 10, "=> %c", new_cell);
    // getch();

  return new_cell;
}

map_t mutate_map(const map_t &old_map)
{
  map_t result(WIDTH * HEIGHT);
  int pos{0};
  for (auto it = result.begin(); it != result.end(); ++it)
  {
    *it = calculate_new_cell(pos, old_map);
    ++pos;
  }
  return result;
}

int main(const int argc, const char** argv)
{
  std::string inputfilename{"./input_18.txt"};
  if (argc > 1)
  {
    inputfilename = std::string{argv[1]};    
  }

  map_t map = read_map(inputfilename);
  prepare_screen();

  int minute{0};

  while(minute < 101)
  {
    print_map(map);
    print_value(minute, map);
    map = mutate_map(map);

    ++minute;
    // getch();
  }
  getch();
  endwin();

  return 0;
}