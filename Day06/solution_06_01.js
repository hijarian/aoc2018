const fs = require('fs');

/**
 * Define a metric
 */
const distance = ({ x: x1, y: y1 }, { x: x2, y: y2 }) => Math.abs(x2 - x1) + Math.abs(y2 - y1);

const sumDistances = (accumulator, value) => accumulator + value.distance;

const processData = data => {
  const points = [];
  let minX = 24000000, maxX = 0, minY = 24000000, maxY = 0;

  data.split('\n').forEach(
    coordinateString => {
      const [x, y] = coordinateString.split(',')
        .map(
          xystring => Number(xystring.trim())
        );

    //  * Determine the max and min boundaries of the area.
      
      if (x < minX) 
        minX = x;
      
      if (x > maxX) 
        maxX = x;
      
      if (y < minY) 
        minY = y;
      
      if (y > maxY) 
        maxY = y;
      
      //  * Name all points.
      points.push({ x, y });
    }
  );
  const areas = {};
  let pointsDistance = [];
  let distancesSum = 0;
  let closestPointsArea = 0;
  for (let xi = minX; xi <= maxX; ++xi) {
    for (let yi = minY; yi <= maxY; ++yi) {
      pointsDistance = [];
      for (let pointIndex in points) {
        pointsDistance.push({
          point: pointIndex,
          distance: distance({ x: xi, y: yi}, points[pointIndex])
        });
      }
      distancesSum = pointsDistance.reduce(sumDistances, 0);
      if (distancesSum < 10000) {
        ++closestPointsArea;
      }
      // console.log(pointsDistance);
      pointsDistance.sort((left, right) => left.distance - right.distance);
      // console.log(pointsDistance);
      // special case: if closest to several points, count as none.
      if (pointsDistance[0].distance === pointsDistance[1].distance) {
        continue;
      }
      const closestPointName = pointsDistance[0].point;
      // console.log(xi, yi, closestPointName);
      // console.log(areas);
      if (!areas[closestPointName]) {
        areas[closestPointName] = 0;
      }
      areas[closestPointName] += 1;
    }
  }
  // return areas;
  return {
    largestAreaPoint: Object
      .entries(areas)
      .sort(
        (left, right) => right[1] - left[1]
      ).map(([name, area]) => `${name}: ${area}`)[0],
    closestPointsArea
  };
//  * Determine the points which are on the boundaries. Ignore them.
//  * For all points inside the boundary, calculate the distance to all the named non-boundary points.
//  *  Among those distances, pick the minimum one. If there's more than one minimum, ignore that point.
//  * Miminum distance for that point determines the name of this point.
//  * Count the number of names counted.
//  * Return the name with the max count.
}


fs.readFile(
	'C:/Users/somebodyrotten/projects/AoC2018/input_06.txt',
	(_, data) => {
		console.log(
			processData(String(data).trim())
		);
	}
);