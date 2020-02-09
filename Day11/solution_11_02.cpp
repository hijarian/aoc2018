#include <iostream>
#include <string>
#include <vector>
#include <functional>
#include <map>

int hun(int input)
{
  return input % 1000 / 100;
}

int power_level(int x, int y, int serial)
{
  int rack_id = x + 10;
  int power = rack_id * y;
  power += serial;
  power *= rack_id;
  power = hun(power) - 5;

  return power;
}

std::vector<int> powers(90000);

int total_power_level(const int x, const int y, const int size, const int serial_number)
{
  int total_power = 0;
  for (int current_x = x; current_x < x + size; ++current_x)
  {
    for (int current_y = y; current_y < y + size; ++current_y)
    {
      total_power += powers[(current_x - 1) + (current_y - 1) * 300];
    }
  }

  return total_power;
}

int test()
{
  int total = 0;
  for (int x = 90; x < 106; ++x)
  {
    for (int y = 269; y < 285; ++y)
    {
      total += power_level(x, y, 18);
    }
  }
  std::cout << total << std::endl;

  total = 0;
  for (int x = 232; x < 244; ++x)
  {
    for (int y = 251; y < 263; ++y)
    {
      total += power_level(x, y, 42);
    }
  }
  std::cout << total << std::endl;

  return 0;
}

// My serial number is 8772
// This is crazy slow even with precaching.
// Probably need to precache whole blocks of 300x300 table, like 2x2 blocks and 3x3 blocks,
// and use them when calculating the sums.
int main(int argc, char **argv)
{
  if (argc < 2)
  {
    std::cout << "You must provide a serial number" << std::endl;
    return 1;
  }

  const int serial_number = std::stoi(argv[1]);

  for (int x = 0; x < 300; ++x)
  {
    std::cout << x << " ";
    for (int y = 0; y < 300; ++y)
    {
      powers[x + y * 300] = power_level(x + 1, y + 1, serial_number);
    }
  }
  std::cout << "Done precaching." << std::endl;

  std::pair<int, int> max_total_power_coordinates{1, 1};
  int size_of_max_area = 300;
  int max_total_power{0};
  int current_total_power;
  for (int size = 300; size >= 1; --size)
  {
    std::cout << "Size " << size << std::endl;
    for (int x = 1; x <= 301 - size; ++x)
    {
      for (int y = 1; y <= 301 - size; ++y)
      {
        current_total_power = total_power_level(x, y, size, serial_number);
        if (current_total_power > max_total_power)
        {
          max_total_power = current_total_power;
          max_total_power_coordinates.first = x;
          max_total_power_coordinates.second = y;
          size_of_max_area = size;
        }
      }
    }
  }

  std::cout << max_total_power_coordinates.first << "x";
  std::cout << max_total_power_coordinates.second << ",";
  std::cout << size_of_max_area << ": ";
  std::cout << max_total_power << std::endl;

  return 0;
}