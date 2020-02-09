const fs = require('fs');

const processDataInReverse = data => {
  const stepDependencies = {};
  data.split('\n').forEach(
    definition => {
      const match = definition.match(/Step (.) must be finished before step (.) can begin./);
//      console.log(definition, match);
      if (!stepDependencies[match[2]]) {
        stepDependencies[match[2]] = [];
      }
      if (!stepDependencies[match[1]]) {
        stepDependencies[match[1]] = [];
      }
      stepDependencies[match[1]].push(match[2]);
    }
  );
  console.log(stepDependencies);
  let input = Object.entries(stepDependencies);
  let output = '';

  while (input.length > 0) {
    // find all letters with no followers
    let candidates = input.filter(([, followers]) => followers.length === 0);
//    console.log(candidates);
    // forget about all letters without followers, they are candidates now.
    input = input.filter(([, followers]) => followers.length > 0);
    // sort descending because if there's several candidates, first must be the first letter, and we are going here from end to start, in reverse order.
    candidates = candidates.sort((left, right) => right[0].localeCompare(left[0])); 
    for (let index = 0; index < candidates.length; ++index) {
      const candidate = candidates[index][0];
      // prepend candidate letter to output
      output = candidate + output;
      // remove candidate from followers lists of all leftover letters
      input = input.map(([letter, followers]) => [letter, followers.filter(letter => letter !== candidate)]);
      console.log(output, input.length);
    }
  }

  return output;
};

const processData = data => {
  const stepDependencies = {};
  data.split('\n').forEach(
    definition => {
      const match = definition.match(/Step (.) must be finished before step (.) can begin./);
      console.log(definition, match);
      if (!stepDependencies[match[2]]) {
        stepDependencies[match[2]] = [];
      }
      if (!stepDependencies[match[1]]) {
        stepDependencies[match[1]] = [];
      }
      stepDependencies[match[2]].push(match[1]);
    }
  );
  console.log(stepDependencies);
  let input = Object.entries(stepDependencies);
  let output = '';
  let step = 0;

  // find all letters with no followers
  let candidates = input.filter(([, followers]) => followers.length === 0);
  // sort descending because if there's several candidates, first must be the first letter, and we are going here from end to start, in reverse order.
  candidates = candidates.sort((left, right) => left[0].localeCompare(right[0]));
  console.log('current candidates', candidates);
  // forget about all letters without followers, they are candidates now.
  input = input.filter(([, followers]) => followers.length > 0);
  console.log('current input', input);
  
  while (candidates.length > 0) {
    console.log('--------------------------------step', step);
    const candidate = candidates[0][0];
    candidates = candidates.slice(1);
    // append candidate letter to output
    output += candidate;
    console.log('current output', output);

    // remove candidate from followers lists of all leftover letters
    input = input.map(([letter, followers]) => [letter, followers.filter(letter => letter !== candidate)]);
    
    // determine new candidates, if any
    newCandidates = input.filter(([, followers]) => followers.length === 0);
    console.log('new candidates', newCandidates);
    // add new candidates to old candidates
    for (let index = 0; index < newCandidates.length; ++index) {
      candidates.push(newCandidates[index]);
    }
    candidates = candidates.sort((left, right) => left[0].localeCompare(right[0]));
    console.log('current candidates', candidates);
    input = input.filter(([, followers]) => followers.length > 0);
    console.log('current input', input);
    ++step;
    // if (step > 5) { break; }
  }
  console.log('DONE');
  console.log(candidates);
  console.log(input);
  return output;
};
fs.readFile(
  'C:/Users/somebodyrotten/projects/AoC2018/input_07.txt',
  (_, data) => {
    console.log(processData(String(data).trim()));
  }
)