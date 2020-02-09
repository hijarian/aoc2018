const fs = require('fs');

const collapseOppositePolarities = input => {
  let result = '';
  let index = 0;
  while (index < input.length) {
    if (!input[index + 1]) {
      result += input[index];
      break;
    }
    if (input[index].toLowerCase() === input[index + 1] || input[index + 1].toLowerCase() === input[index]) {
      index += 2;
      continue;
    }
    result += input[index];
    ++index; 
  }
  // console.log(result);
  return result;
}


const processDataOld = data => {
  let pastVersion = '';
  let processedVersion = data;
  let iteration = 0;
  while (pastVersion !== processedVersion) {
    ++iteration;
    if (iteration % 500 === 0) {
      console.log(`iteration ${iteration}: ${pastVersion.length}, ${processedVersion.length}`);
    }
    pastVersion = processedVersion;
    processedVersion = collapseOppositePolarities(processedVersion);
    if (iteration % 500 === 0) {
      console.log(`${pastVersion.length} -> ${processedVersion.length}`);
    }
  };
  console.log('DONE');
  return processedVersion.length;
};

const processData1 = data => {
  return react(data).length;
}

const processData2 = data => {
  const first = react(data);
  const pieces = 'abcdefghijklmnopqrstuvwxyz';
  const lengths = {};
  for (let piece of pieces) {
    const trimmed = first.join('').replace(new RegExp(`[${piece}${piece.toUpperCase()}]`, 'g'), '');
    console.log('piece', piece, 'trimmed length', trimmed.length);
    const withoutPiece = react(trimmed);
    lengths[piece] = withoutPiece.length;
  }
  return lengths;
}

const react = data => {
  let letters = data.split('');
  let i = 1;
  while (i < letters.length) {
    if (Math.abs(letters[i].charCodeAt(0) - letters[i-1].charCodeAt(0)) === 32) {
      letters.splice(i - 1, 2);
      i = Math.max(1, i - 1);
    } else {
      ++i;
    }
  }
  return letters;
}

fs.readFile(
	'C:/Users/somebodyrotten/projects/AoC2018/input_05.txt',
	// 'C:/Users/somebodyrotten/projects/AoC2018/input_05.trim.txt',
	(_, data) => {
		console.log(
			processData2(String(data).trim())
		);
	}
);

// console.log(processData('dabAcCaCBAcCcaDA'));