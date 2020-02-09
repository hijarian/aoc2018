const fs = require('fs');

const parseSpecification = specification => {
  const pieces = specification.match(/^\#(\d)+ \@ (\d+)\,(\d+)\: (\d+)x(\d+)$/);
  return {
    id: pieces[1],
    left: +pieces[2],
    top: +pieces[3],
    length: +pieces[4],
    height: +pieces[5]
  };
};

const processData = data => {
  const raw = data.split('\n');

  const fabricArea = [];
  const specificationIsTainted = {};

  function markAreaAccordingToSpecification(specification) {
    if (!specification) {
      return;
    }
    const { left, length, top, height } = parseSpecification(specification);
    console.log(specification, left, length, top, height);
    specificationIsTainted[specification] = false;

    // columns
    for(let horizontalIndex = left; horizontalIndex < left + length; ++horizontalIndex) {
      if (!fabricArea[horizontalIndex]) {
        fabricArea[horizontalIndex] = [];
      }
      // rows
      for (let verticalIndex = top; verticalIndex < top + height; ++verticalIndex) {
        if (!fabricArea[horizontalIndex][verticalIndex]) {
          fabricArea[horizontalIndex][verticalIndex] = [];
        }
        fabricArea[horizontalIndex][verticalIndex].push(specification);
        if (fabricArea[horizontalIndex][verticalIndex].length > 1) {
          for (let specification of fabricArea[horizontalIndex][verticalIndex]) {
            specificationIsTainted[specification] = true;
          } 
        }
      }
    }
  }

  // markAreaAccordingToSpecification(raw[0]);
  raw.forEach(markAreaAccordingToSpecification);

  let count = 0;
  for(horizontalIndex = 0; horizontalIndex < fabricArea.length; ++horizontalIndex) {
    if (!fabricArea[horizontalIndex]) {
      continue;
    }
    for (verticalIndex = 0; verticalIndex < fabricArea[horizontalIndex].length; ++verticalIndex) {
      if (fabricArea[horizontalIndex][verticalIndex] && fabricArea[horizontalIndex][verticalIndex].length > 1) {
        ++count;
      }
    }
  }
  console.log('specifications processed', Object.keys(specificationIsTainted).length);
  let untaintedSpecifications = Object.entries(specificationIsTainted).filter(([specification, is_tainted]) => !is_tainted);
  return { count, untaintedSpecifications };

}

  // const horizontalSize = fabricArea.length;
  // const verticalSize = fabricArea.reduce(
  //   (maxSize, currentRow) => (currentRow.length > maxSize) ? currentRow.length : maxSize,
  //   0
  // );
  // return { horizontalSize, verticalSize };

// console.log(
//   parseSpecification('#87 @ 104,763: 22x16')
// );

fs.readFile(
	'C:/Users/somebodyrotten/projects/AoC2018/input_03.txt',
	(_, data) => {
		console.log(
			processData(String(data))
		);
	}
);
