#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <stdexcept>
#include <vector>
#include <algorithm>

enum class Direction { Left, Forward, Right };
std::ostream& operator<< (std::ostream& stream, const Direction& direction)
{
  switch (direction)
  {
    case Direction::Left:
      stream << "Left";
      break;
    case Direction::Forward:
      stream << "Forward";
      break;
    case Direction::Right:
      stream << "Right";
      break;
  }
  return stream;
}

struct Cart{
  int id;
  char representation;
  int row;
  int col;
  Direction next_rotation;
  bool is_crashed = false;
};

std::pair<std::vector<std::string>, std::vector<Cart>> read_input(std::string filename)
{
  std::ifstream inputfile{filename};
  if (!inputfile.is_open())
  {
    throw std::invalid_argument("Unable to open file");
  }

  std::vector<std::string> map;
  std::string line;

  std::vector<Cart> carts;
  int cart_number = 1;
  int row{};
  int col{};
  while (inputfile.good())
  {
    std::getline(inputfile, line);
    if (line.length() < 1)
      continue;

    col = 0;
    for (const char & symbol : line)
    {
      if (symbol == 'v' || symbol == '>' || symbol == '<' || symbol == '^')
      {
        Cart cart;
        cart.row = row;
        cart.col = col;
        cart.representation = symbol;
        cart.next_rotation = Direction::Left;
        cart.id = cart_number;
        ++cart_number;

        carts.push_back(cart);
        if (symbol == 'v' || symbol == '^')
          line[col] = '|';
        if (symbol == '>' || symbol == '<')
          line[col] = '-';
      }
      ++col;
    }
    map.push_back(line);
    ++row;
  }

  return std::pair<std::vector<std::string>, std::vector<Cart>>(map, carts);
}

void turn(Cart &cart, char corner_type)
{
  if (corner_type != '/' && corner_type != '\\')
  {
    throw std::invalid_argument("Corner may be either / or \\.");
  }

  switch (cart.representation)
  {
    case '^':
      cart.representation = corner_type == '/' ? '>' : '<';
      break;
    case '>':
      cart.representation = corner_type == '/' ? '^' : 'v';
      break;
    case '<':
      cart.representation = corner_type == '/' ? 'v' : '^';
      break;
    case 'v':
      cart.representation = corner_type == '/' ? '<' : '>';
      break;
    default:
      throw std::logic_error("Invalid cart representation: " + cart.representation);
  }
}

char rotate_left(char representation)
{
  switch (representation)
  {
    case '^': return '<';
    case '>': return '^';
    case 'v': return '>';
    case '<': return 'v';
    default:
      throw std::logic_error("Invalid cart representation: " + representation);
  }
}

char rotate_right(char representation)
{
  switch (representation)
  {
    case '^': return '>';
    case '>': return 'v';
    case 'v': return '<';
    case '<': return '^';
    default:
      throw std::logic_error("Invalid cart representation: " + representation);
  }
}

void rotate(Cart &cart)
{
  if (cart.next_rotation == Direction::Left)
  {
    cart.representation = rotate_left(cart.representation);
    cart.next_rotation = Direction::Forward;
    return;
  }

  if (cart.next_rotation == Direction::Forward)
  {
    cart.next_rotation = Direction::Right;
    return;
  }

  if (cart.next_rotation == Direction::Right)
  {
    cart.representation = rotate_right(cart.representation);
    cart.next_rotation = Direction::Left;
    return;
  }
}

void sort_carts(std::vector<Cart> &carts)
{
  std::sort(
    carts.begin(),
    carts.end(),
    [](const Cart &left, const Cart &right)
    {
      if (left.row == right.row)
        return left.col < right.col;

      return left.row < right.row;   
    }
  );
}

void print_carts(const std::vector<Cart> &carts)
{
  for (const Cart & cart : carts)
  {
    std::cout << cart.id << ": " << cart.representation;
    std::cout << " at " << cart.col << "," << cart.row;
    std::cout << " " << cart.next_rotation;
    
    if (cart.is_crashed)
      std::cout << " --CRASHED-- ";

    std::cout << std::endl; 
  }
}

std::vector<std::string> merge(const std::vector<std::string> &map, const std::vector<Cart> &carts)
{
  std::vector<std::string> result{};
  for (const std::string &row : map)
  {
    result.push_back(row);
  }

  for (const Cart & cart : carts)
  {
    result[cart.row][cart.col] = cart.is_crashed ? 'X' : cart.representation;
  }

  return result;
}

std::vector<std::string> show_carts(const std::vector<std::string> &map, const std::vector<Cart> &carts)
{
  std::string blank{map[0]};
  for (char & symbol : blank)
  {
    symbol = ' ';
  }

  std::vector<std::string> result{};
  for (const std::string &row : map)
  {
    result.push_back(blank);
  }

  for (const Cart & cart : carts)
  {
    result[cart.row][cart.col] = cart.is_crashed ? 'X' : cart.representation;
  }

  return result;
}


void print_map(const std::vector<std::string> &map)
{
  for (const std::string & row : map)
  {
    std::cout << row << std::endl;
  }
}

std::pair<bool, std::pair<int, int>> find_collision(const std::vector<Cart> & carts)
{
  std::map<std::string, int> coordinate_counts{};
  std::map<std::string, std::pair<int, int>> coordinates{};
  for (const Cart & cart : carts)
  {
    if (cart.is_crashed == true)
    {
      continue;
    }
    std::string key{std::to_string(cart.col) + "," + std::to_string(cart.row)};
    auto counts = coordinate_counts[key];
    coordinate_counts[key] = counts + 1;
    coordinates[key].first = cart.col;
    coordinates[key].second = cart.row;
  }
  
  for (const auto & it : coordinate_counts)
  {
    if (it.second > 1)
    {
      return std::pair<bool, std::pair<int, int>>(true, coordinates[it.first]);
    }
  }
  return std::pair<bool, std::pair<int, int>>(false, std::pair<int, int>(-1, -1));
}

void move(Cart & cart)
{
  switch (cart.representation)
  {
    case '^':
      --cart.row;
      break;
    case 'v':
      ++cart.row;
      break;
    case '>':
      ++cart.col;
      break;
    case '<':
      --cart.col;
      break;
    default:
      throw std::logic_error("Invalid representation: " + cart.representation);
  }
}

void mark_crashed_carts(std::vector<Cart> &carts, const std::pair<int, int> collision_coordinates)
{
  for (Cart & cart : carts)
  {
    if (cart.col == collision_coordinates.first && cart.row == collision_coordinates.second)
    {
      cart.is_crashed = true;
    }
  }
}

int count_remaining_carts(std::vector<Cart> &carts)
{
  int count = 0;
  for (Cart & cart : carts)
  {
    if (cart.is_crashed == false)
    {
      ++count;
    }
  }
  return count;
}

int main(int argc, char** argv)
{
  std::vector<std::string> map;
  std::vector<Cart> carts;

  std::tie(map, carts) = read_input("C:/Users/somebodyrotten/projects/AoC2018/input_13.txt");
  std::cout << map.size() << " lines" << std::endl;
  print_carts(carts);

  // print_map(merge(map, carts));
  bool has_collision = false;
  std::pair<int, int> collision_coordinates{};
  int remaining_carts = carts.size();
  int tick = 1;
  while (remaining_carts > 1)
  {
    // std::cout << "Tick " << tick << std::endl;

    for (Cart & cart : carts)
    {
      // std::cout << "Moves: cart #" << cart.id << " at " << cart.col << "," << cart.row << std::endl;
      
      if (cart.is_crashed == true)
      {
        continue;
      }

      move(cart);
      char position = map[cart.row][cart.col];
      if (position == '/' || position == '\\')
      {
        turn(cart, position);
      }
      if (position == '+')
      {
        rotate(cart);
      }
      std::tie(has_collision, collision_coordinates) = find_collision(carts);
      if (has_collision)
      {
        std::cout << "Collision at " << collision_coordinates.first << "," << collision_coordinates.second << std::endl;
        mark_crashed_carts(carts, collision_coordinates);
        remaining_carts = count_remaining_carts(carts);
        std::cout << remaining_carts << " carts remaining" << std::endl;
      }
    }
    sort_carts(carts);

    if (map.size() < 15)
      print_map(merge(map, carts));

    // print_carts(carts);
    ++tick;
  }
  print_carts(carts);
  std::cout << "DONE" << std::endl;
}