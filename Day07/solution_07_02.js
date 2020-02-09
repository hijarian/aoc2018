const fs = require('fs');

// Returns 61 for A, 62 for B etc.
// Char code for `A` is 65 
const getRequiredSeconds = (letter) => {
  return letter.charCodeAt(0) - 64; 
}

const MAX_WORKERS = 2;

// note: with getRequiredSeconds = letter => 1 and MAX_WORKERS = 1 this solves exactly like solution_07_01 

const processData = data => {
  const stepDependencies = {};
  data.split('\n').forEach(
    definition => {
      const match = definition.match(/Step (.) must be finished before step (.) can begin./);
      // console.log(definition, match);
      if (!stepDependencies[match[2]]) {
        stepDependencies[match[2]] = [];
      }
      if (!stepDependencies[match[1]]) {
        stepDependencies[match[1]] = [];
      }
      stepDependencies[match[2]].push(match[1]);
    }
  );
  // console.log(stepDependencies);
  let input = Object.entries(stepDependencies);
  let output = '';
  let step = 0;
  let workers = [];
  let doneLetters = [];
  let logRecord = {
    second: 0,
    worker1: '',
    worker2: '',
    worker3: '',
    worker4: '',
    worker5: '',
    result: ''
  };

  const printLogRecord = record => {
    let result = String(record.second).padStart(6, ' ');
    for (let i in record.workers) {
      result += ' ' + record.workers[i].letter;
    }
    result += ' ' + record.result;

    console.log(result);
  }

  // find all letters with no followers
  let candidates = input.filter(([, followers]) => followers.length === 0);
  // sort descending because if there's several candidates, first must be the first letter, and we are going here from end to start, in reverse order.
  candidates = candidates.sort((left, right) => left[0].localeCompare(right[0]));
  // forget about all letters without followers, they are candidates now.
  input = input.filter(([, followers]) => followers.length > 0);

  // add work to workers
  while (workers.length < MAX_WORKERS && candidates.length > 0) {
    workers.push({
      letter: candidates[0][0],
      done: 0,
      required: getRequiredSeconds(candidates[0][0])
    });
    candidates = candidates.slice(1);
  }

  while (workers.length > 0) {
    logRecord.second = step;
    logRecord.workers = workers;

    // console.log('-----------------------------second', step);
    doneLetters = [];
    // process work
    for (let i in workers) {
      workers[i].done += 1;
      if (workers[i].done === workers[i].required) {
        doneLetters.push(workers[i].letter);
        output += workers[i].letter;
      }
    }
    logRecord.result = output;
    // remove workers which done their work
    workers = workers.filter(({done, required}) => done < required);

    // console.log('done letters', doneLetters.join(','));
    // console.log('output', output);

    // remove done letters from dependencies lists of all leftover letters
    input = input.map(([letter, followers]) => [letter, followers.filter(letter => !doneLetters.includes(letter))]);
    // console.log('leftover letters', input.length);

    // determine new candidates, if any
    newCandidates = input.filter(([, dependencies]) => dependencies.length === 0);
    // console.log('new candidates', newCandidates.join(','));
    input = input.filter(([, followers]) => followers.length > 0);
    // console.log('leftover letters number', input.length);

    // add new candidates to old candidates
    for (let index = 0; index < newCandidates.length; ++index) {
      candidates.push(newCandidates[index]);
    }
    candidates = candidates.sort((left, right) => left[0].localeCompare(right[0]));
    // console.log('new candidates', candidates.join(','))

    // add work
    while (workers.length < MAX_WORKERS && candidates.length > 0) {
      workers.push({
        letter: candidates[0][0],
        done: 0,
        required: getRequiredSeconds(candidates[0][0])
      });
      candidates = candidates.slice(1);
    }
    // console.log('number of workers', workers.length);
    // console.log('leftover candidates', candidates.join(','));

    ++step;
    printLogRecord(logRecord);
  }
  console.log('DONE');
  console.log('leftover candidates', candidates);
  console.log('leftover input', input);
  
  return output;
};
fs.readFile(
//  'C:/Users/somebodyrotten/projects/AoC2018/input_07.txt',
  'C:/Users/somebodyrotten/projects/AoC2018/input_07.example.txt',
  (_, data) => {
    console.log(processData(String(data).trim()));
  }
)