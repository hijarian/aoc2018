const fs = require('fs');

const processData = data => {
  let changes = data.split('\n').filter(stringNumber => stringNumber != '').map(stringNumber => +stringNumber);

  let foundFrequencies = new Set();
  let foundRepeat = false;
  let frequency = 0;
  foundFrequencies.add(frequency);
  let loopCount = 1;
  let frequenciesCount = 1;
  while (!foundRepeat) {
    console.log('loop', loopCount, 'set has', foundFrequencies.size, 'frequencies, checked', frequenciesCount, 'frequencies');
    for (let i = 0; i < changes.length; ++i) {
//      console.log('change:', frequency, '+', changes[i], '=', frequency + changes[i]);
      frequency += changes[i];
      if (foundFrequencies.has(frequency)) {
        console.log('FOUND REPEAT', frequency);
        foundRepeat = true;
        break;
      }
//      console.log('adding', frequency);
      foundFrequencies.add(frequency);
      ++frequenciesCount;
//      console.log('set has', foundFrequencies.size, 'elements');
    }
    ++loopCount;
  }
  console.log('RESULT:', frequency);
};

fs.readFile(
	'C:/Users/somebodyrotten/Downloads/AoC2018/input_01.txt',
	(_, data) => {
		console.log(
			processData(String(data))
		);
	}
);
