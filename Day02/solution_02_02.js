const fs = require('fs');

const processData = data => {
  const raw = data
    .split('\n')
    .sort();

  let first = raw[0];
  let second;
  let differencesCount;
  for (let index = 1; index < raw.length; ++index) {
    differencesCount = 0;
    second = raw[index];
    for (let letter = 0; letter < first.length; ++letter) {
      if (first[letter] !== second[letter]) {
        differencesCount += 1;
      }
    }
    if (differencesCount === 1) {
      console.log('ONLY ONE DIFFERENCE between', first, 'and', second);
      break;
    }
    first = raw[index];
  }

  return [first, second];
};


fs.readFile(
	'C:/Users/somebodyrotten/projects/AoC2018/input_02.txt',
	(_, data) => {
		console.log(
			processData(String(data))
		);
	}
);