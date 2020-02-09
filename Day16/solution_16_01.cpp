#include <vector>
#include <string>
#include <iostream>
#include <regex>
#include <fstream>
#include <set>
#include <map>
#include <cstdio>

std::vector<int> memory{0, 0, 0, 0};

void addr(int a, int b, int c) { memory[c] = memory[a] + memory[b]; }
void addi(int a, int b, int c) { memory[c] = memory[a] + b; }

void mulr(int a, int b, int c) { memory[c] = memory[a] * memory[b]; }
void muli(int a, int b, int c) { memory[c] = memory[a] * b; }

void banr(int a, int b, int c) { memory[c] = memory[a] & memory[b]; }
void bani(int a, int b, int c) { memory[c] = memory[a] & b; }

void borr(int a, int b, int c) { memory[c] = memory[a] | memory[b]; }
void bori(int a, int b, int c) { memory[c] = memory[a] | b; }

void setr(int a, int, int c) { memory[c] = memory[a]; }
void seti(int a, int, int c) { memory[c] = a; }

void gtir(int a, int b, int c) { memory[c] = a > memory[b] ? 1 : 0; }
void gtri(int a, int b, int c) { memory[c] = memory[a] > b ? 1 : 0; }
void gtrr(int a, int b, int c) { memory[c] = memory[a] > memory[b] ? 1 : 0; }

void eqir(int a, int b, int c) { memory[c] = a == memory[b] ? 1 : 0; }
void eqri(int a, int b, int c) { memory[c] = memory[a] == b ? 1 : 0; }
void eqrr(int a, int b, int c) { memory[c] = memory[a] == memory[b] ? 1 : 0; }

using pair_t = std::pair<std::string, void (*)(int, int, int)>;
std::vector<pair_t> operations{
    pair_t{"addr", addr},
    pair_t{"addi", addi},
    pair_t{"mulr", mulr},
    pair_t{"muli", muli},
    pair_t{"banr", banr},
    pair_t{"bani", bani},
    pair_t{"borr", borr},
    pair_t{"bori", bori},
    pair_t{"setr", setr},
    pair_t{"seti", seti},
    pair_t{"gtir", gtir},
    pair_t{"gtri", gtri},
    pair_t{"gtrr", gtrr},
    pair_t{"eqir", eqir},
    pair_t{"eqri", eqri},
    pair_t{"eqrr", eqrr},
};

std::map<std::string, void (*)(int, int, int)> operations_mapping{};

void test_matcher()
{
  int count = 0;
  for (const auto &operation_description : operations)
  {
    memory[0] = 3;
    memory[1] = 2;
    memory[2] = 1;
    memory[3] = 1;

    std::cout << operation_description.first;
    operation_description.second(2, 1, 2);
    if (memory[0] == 3 && memory[1] == 2 && memory[2] == 2 && memory[3] == 1)
    {
      std::cout << " Match!";
      ++count;
    }
    std::cout << std::endl;
  }
}

std::vector<std::string> count_matches(
    const std::vector<int> &initial_memory,
    const int a,
    const int b,
    const int c,
    const std::vector<int> &result_memory)
{
  std::vector<std::string> result;

  for (const auto &operation_description : operations)
  {
    memory[0] = initial_memory[0];
    memory[1] = initial_memory[1];
    memory[2] = initial_memory[2];
    memory[3] = initial_memory[3];

    operation_description.second(a, b, c);
    if (memory[0] == result_memory[0] && memory[1] == result_memory[1] && memory[2] == result_memory[2] && memory[3] == result_memory[3])
    {
      result.push_back(operation_description.first);
    }
  }
  return result;
}

void print_current_matches(const std::string &opcode, const std::vector<std::string> &current_matches)
{
  std::cout << opcode << ": ";
  for (const auto & opname : current_matches)
  {
    std::cout << opname << " ";
  }
  std::cout << std::endl;
}

int fetch_integer(const std::ssub_match base_sub_match)
{
  return std::stoi(base_sub_match.str());
}

std::map<std::string, std::set<std::string>> read_counts(std::ifstream &inputfile)
{
  std::map<std::string, std::set<std::string>> result{};

  std::regex before_regex("Before: \\[(\\d+), (\\d+), (\\d+), (\\d+)\\]");
  std::regex operation_regex("(\\d+) (\\d+) (\\d+) (\\d+)");
  std::regex after_regex("After:  \\[(\\d+), (\\d+), (\\d+), (\\d+)\\]");

  bool can_read_input{true};

  unsigned block_number{0};
  std::string line{};

  // initialize the regexp stuff
  std::smatch base_match{};

  std::vector<int> initial_memory{0, 0, 0, 0};
  std::string opcode{};
  int a, b, c;
  std::vector<int> result_memory{0, 0, 0, 0};

  std::vector<std::string> current_matches{};

  int samples_matching_more_than_three_opnames{0};

  while (inputfile.good() && can_read_input)
  {
    ++block_number;

    // std::cout << "Block number " << block_number << std::endl;

    std::getline(inputfile, line);

    // std::cout << line << std::endl;

    if (line.length() < 1)
      continue;

    can_read_input = std::regex_match(line, base_match, before_regex);
    if (!can_read_input)
    {
      std::cout << "Cannot read any longer" << std::endl;
      break;
    }

    initial_memory[0] = fetch_integer(base_match[1]);
    initial_memory[1] = fetch_integer(base_match[2]);
    initial_memory[2] = fetch_integer(base_match[3]);
    initial_memory[3] = fetch_integer(base_match[4]);

    std::getline(inputfile, line);
    // std::cout << line << std::endl;
    can_read_input = std::regex_match(line, base_match, operation_regex);
    if (!can_read_input)
    {
      std::cout << "Cannot read any longer" << std::endl;
      break;
    }

    std::ssub_match submatch = base_match[1];
    opcode = submatch.str();
    a = fetch_integer(base_match[2]);
    b = fetch_integer(base_match[3]);
    c = fetch_integer(base_match[4]);

    std::getline(inputfile, line);
    // std::cout << line << std::endl;
    can_read_input = std::regex_match(line, base_match, after_regex);
    if (!can_read_input)
    {
      std::cout << "Cannot read any longer" << std::endl;
      break;
    }

    result_memory[0] = fetch_integer(base_match[1]);
    result_memory[1] = fetch_integer(base_match[2]);
    result_memory[2] = fetch_integer(base_match[3]);
    result_memory[3] = fetch_integer(base_match[4]);

    std::vector<std::string> current_matches = count_matches(initial_memory, a, b, c, result_memory);

    // print_current_matches(opcode, current_matches);
    if (current_matches.size() >= 3)
    {
      // std::cout << "Sample " << block_number << std::endl;
      // print_current_matches(opcode, current_matches);
      ++samples_matching_more_than_three_opnames;
    }

    for (const auto & opname : current_matches)
    {
      result[opcode].insert(opname);
    }
    
    // read empty line to skip separator between blocks
    std::getline(inputfile, line);
    // std::cout << line << std::endl;
  }

  std::cout << "Samples matching more than three opnames: " << samples_matching_more_than_three_opnames << std::endl;

  return result;  
}

void print_counts(const std::map<std::string, std::set<std::string>> & counts)
{
  for (const auto & info : counts)
  {
    std::cout << info.first << ": ";
    for (const auto & opname : info.second)
    {
      std::cout << opname << " ";
    }
    std::cout << std::endl;
  }
}

std::map<std::string, std::string> decipher_counts(std::map<std::string, std::set<std::string>> counts)
{
  std::map<std::string, std::string> mapping{};

  std::string opcode{""};
  std::string opname{""};

  do
  {
    opcode = "";
    opname = "";
    for (const auto &guess : counts)
    {
      if (guess.second.size() == 1)
      {
        opcode = guess.first;
        opname = *(guess.second.begin());
      }
    }

    std::cout << "Opcode " << opcode << " is " << opname << std::endl;  
    mapping[opcode] = opname;

    for (auto &guess : counts)
    {
      guess.second.erase(opname);
    }

    // std::cout << "After purge:" << std::endl;

    // print_counts(counts);
  } while (opcode != "");

  return mapping;
}

void execute_program(std::ifstream &inputfile, std::map<std::string, std::string> & opnames)
{
  std::string line{};
  std::regex operation_regex("(\\d+) (\\d+) (\\d+) (\\d+)");
  std::smatch base_match{};
  std::string opcode{};
  std::string opname{};
  int a, b, c;

  while(inputfile.good())
  {
    std::getline(inputfile, line);
    if (line.size() < 1)
    {
      continue;
    }
    std::printf("%12s: ", line.c_str());

    if (!std::regex_match(line, base_match, operation_regex))
    {
      std::cout << "Cannot read any longer" << std::endl;
      break;
    }

    std::ssub_match submatch = base_match[1];
    opcode = submatch.str();
    opname = opnames[opcode];
    a = fetch_integer(base_match[2]);
    b = fetch_integer(base_match[3]);
    c = fetch_integer(base_match[4]);

    auto operation = operations_mapping[opname];

    std::printf("%3d %3d %3d %3d -> %s %3d %3d %3d => ", memory[0], memory[1], memory[2], memory[3], opname.c_str(), a, b, c);

    operation(a, b, c);

    std::printf("%3d %3d %3d %3d\n", memory[0], memory[1], memory[2], memory[3]);
  }
}

int main()
{
  for (const auto & pair : operations)
  {
    operations_mapping[pair.first] = pair.second;
  }

  std::ifstream inputfile{"./input_16.txt"};
  if (!inputfile.is_open())
  {
    std::cout << "Unable to open file" << std::endl;
    return 1;
  }

  std::map<std::string, std::set<std::string>> counts = read_counts(inputfile);

  print_counts(counts);

  std::map<std::string, std::string> mapping = decipher_counts(counts);

  std::cout << "Collected mapping:" << std::endl;

  for (const auto &pair : mapping)
  {
    std::cout << pair.first << ": " << pair.second << std::endl; 
  }

  memory[0] = memory[1] = memory[2] = memory[3] = 0;
  execute_program(inputfile, mapping);

  std::cout << "Memory is: " << memory[0] << " " << memory[1] << " " << memory[2] << " " << memory[3] << std::endl;

  inputfile.close();

  return 0;
}