#include <iostream>
#include <string>
#include <vector>

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

void print_power_level(int x, int y, int serial_number)
{
  std::cout << x << "," << y << " sn " << serial_number << ": " << power_level(x, y, serial_number) << std::endl;
}

void test()
{
  print_power_level(3, 5, 8);
  print_power_level(122, 79, 57);
  print_power_level(217, 196, 39);
  print_power_level(101, 153, 71);
}

int total_power_level(int x, int y, int serial_number)
{
  int total_power = 0;
  for (int current_x = x; current_x < x + 3; ++current_x)
  {
    for (int current_y = y; current_y < y + 3; ++current_y)
    {
      total_power += power_level(current_x, current_y, serial_number);
    }
  }

  return total_power;
}

// My serial number is 8772
int main(int argc, char** argv)
{
  if (argc < 2)
  {
    std::cout << "You must provide a serial number" << std::endl;
    return 1;
  }

  const int serial_number = std::stoi(argv[1]);

  std::pair<int, int> max_total_power_coordinates{1,1};
  int max_total_power{0};
  int current_total_power;
  for (int x = 1; x <= 298; ++x)
  {
    for (int y = 1; y <= 298; ++y)
    {
      current_total_power = total_power_level(x, y, serial_number);
      if (current_total_power > max_total_power)
      {
        max_total_power = current_total_power;
        max_total_power_coordinates.first = x;
        max_total_power_coordinates.second = y;
      }
    }
  }

  std::cout << max_total_power_coordinates.first << "x";
  std::cout << max_total_power_coordinates.second << ": ";
  std::cout << max_total_power << std::endl;

  return 0;
}