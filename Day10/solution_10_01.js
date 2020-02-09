const fs = require('fs');

const processData = data => {
  const lines = data.split('\n');
  const points = [];
  const velocities = [];
  lines.forEach(
    line => {
      const match = line.match(/position=< *(-?\d+), *(-?\d+)> velocity=< *(-?\d+), *(-?\d)>/);
      points.push({ x: +match[1], y: +match[2] });
      velocities.push({ x: +match[3], y: +match[4] });
    }
  )

  let state = points.slice();
  let minX = 100000;
  let maxX = -100000;
  let minY = 100000;
  let maxY = -100000;
  let area = 0;
  let previousArea = Infinity;
  let second = 0;
  let previousState = [];

  while (second < 50000) {
    // console.log(state[0].x, state[0].y);

    minX = 100000;
    maxX = -100000;
    minY = 100000;
    maxY = -100000;
    for (let i = 0; i < state.length; ++i) {
      if (state[i].x < minX) 
        minX = state[i].x;

      if (state[i].x > maxX) 
        maxX = state[i].x;
      
      if (state[i].y < minY)
        minY = state[i].y;
      
      if (state[i].y > maxY)
        maxY = state[i].y;
    }

    area = Math.abs(maxX - minX) * Math.abs(maxY - minY);

    if (second % 1000 === 0 || second > 10930) {
      console.log(second, area, minX, maxX, minY, maxY);
    }

    if (previousArea < area) {
      console.log(
        'FOUND STATE WITH MIN AREA',
        second,
        previousArea,
        area,
        minX, maxX, minY, maxY);
      break;
    }

    previousArea = area;
    previousState = state.slice();

    // movement
    for (let i = 0; i < state.length; ++i) {
      state[i].x += velocities[i].x;
      state[i].y += velocities[i].y;
    }

    ++second;
  }
  console.log('DONE');

  return previousState;
};

fs.readFile(
  'C:/Users/somebodyrotten/projects/AoC2018/input_10.txt',
  (_, data) => {
    fs.writeFile(
      'C:/Users/somebodyrotten/projects/AoC2018/output_10.js',
      JSON.stringify(processData(String(data).trim())),
      (error) => {
        if (error)
          console.error(error);
        console.log('PUT RESULT TO FILE');
      }
    );
  }
)
