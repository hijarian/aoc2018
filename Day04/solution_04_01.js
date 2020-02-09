const fs = require('fs');

const guards = {};

const processData = data => {
  let raw = data.split('\n').sort();

  let currentGuard;
  let sleepStart;
  raw.forEach(
    explanation => {
      let match;
      if (match = explanation.match(/Guard #(\d+) begins shift/)) {
        currentGuard = match[1];
        if (!guards[currentGuard]) {
          guards[currentGuard] = { id: currentGuard, timeline: Array(60).fill(0) };
        }
      };
      if (match = explanation.match(/\[.+00:(\d\d)\] falls asleep/)) {
        sleepStart = +match[1];
      }
      if (match = explanation.match(/\[.+00:(\d\d)\] wakes up/)) {
        for (let minute = sleepStart; minute < (+match[1]); ++minute) {
          guards[currentGuard].timeline[minute] += 1
        }
      }
    }
  )

  const guardsList = Object.values(guards).map(
    guard => ({
      id: guard.id,
      timeline: guard.timeline,
      totalSlept: guard.timeline.reduce((accumulator, value) => accumulator + value, 0),
      mostFrequentlySleptMinute: guard.timeline
        .reduce(
          ({maxSlept, maxSleptMinute}, value, index) => {
            if (value > maxSlept) {
              return { maxSlept: value, maxSleptMinute: index }
            }
            return { maxSlept, maxSleptMinute };
          },
          { maxSlept: 0, maxSleptMinute: 0 }
        )
    })
  );
  
  // return guard who slept the most
  //return guardsList.sort((left,right) => right.totalSlept - left.totalSlept)[0];
  return guardsList.sort((left, right) => right.mostFrequentlySleptMinute.maxSlept - left.mostFrequentlySleptMinute.maxSlept)[0];
};

fs.readFile(
	'C:/Users/somebodyrotten/projects/AoC2018/input_04.txt',
	(_, data) => {
		console.log(
			processData(String(data))
		);
	}
);

