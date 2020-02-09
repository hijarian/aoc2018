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

  return { points, velocities };
};

fs.readFile(
  'C:/Users/somebodyrotten/projects/AoC2018/input_10.txt',
  (_, data) => {
    fs.writeFile(
      'C:/Users/somebodyrotten/projects/AoC2018/input_10.json',
      JSON.stringify(processData(String(data).trim()))
    )
  }
)
