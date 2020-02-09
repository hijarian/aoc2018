const fs = require('fs');

// returns { twos: <number>, threes: <number> }, 
// just multiply them together.
const processData = data => {
  return data
    .split('\n')
    .map(countLetters)
    .reduce(
      (accumulator, value) => {
        if (hasExactlyTwoDuplicates(value)) {
          accumulator.twos += 1;
        }
        if (hasExactlyThreeDuplicates(value)) {
          accumulator.threes += 1;
        }
        return accumulator;
      },
      { twos: 0, threes: 0 }
    );
};

const countLetters = string => {
  const letters = {};
  for (let letter of string) {
    if (!letters[letter]) {
      letters[letter] = 0;
    }
    letters[letter] += 1;
  }
  return letters;
};

const hasExactlyTwoDuplicates = letters => {
  for (let index in letters) {
    if (letters[index] === 2) {
      return true;
    }
  }
  return false;
};

const hasExactlyThreeDuplicates = letters => {
  for (let index in letters) {
    if (letters[index] === 3) {
      return true;
    }
  }
  return false;
};

fs.readFile(
	'C:/Users/somebodyrotten/projects/AoC2018/input_02.txt',
	(_, data) => {
		console.log(
			processData(String(data))
		);
	}
);