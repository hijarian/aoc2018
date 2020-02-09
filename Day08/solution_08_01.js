const fs = require('fs');

// global variable hack to get the solution for 8.1
let foundMetadata = 0;

const readNextNode = (data, pointer) => {
  const startPointer = pointer;
  let endPointer = pointer;
  let children = [];
  let metadata = [];
  const childrenCount = +data[pointer];
  const metadataCount = +data[pointer+1];
  endPointer = pointer + 1; // position of number of metadata

  for (let i = 0; i < childrenCount; ++i) {
    ++endPointer; // position of the header of the child
    const child = readNextNode(data, endPointer);
    // console.log('returned child value', child.value);
    children.push(child);
    endPointer = child.endPointer; // temporarily set to the end of the last number in the description of the child; at the next iteration it will be moved to next child.
  }

  for (let i = 0; i < metadataCount; ++i) {
    ++endPointer; // move to position of this metadata

    // side effect to get the solution for part 1.
    foundMetadata += +data[endPointer];

    metadata.push(+data[endPointer]);
  }

  let value = 0;
  // calculation of value is task 8.2
  if (children.length === 0) {
    value = metadata.reduce((accumulator, value) => accumulator + value, 0);
    // console.log('leaf child value', value);
  }
  else {
    // console.log('requested value', metadata.join(','), children.map(child => child.value).join(','));
    for (let i = 0; i < metadataCount; ++i) {
      const childIndex = metadata[i];
      if (childIndex === 0) {
        continue;
      }
      if (!children[childIndex - 1]) {
        continue;
      }
      value += children[childIndex - 1].value;
    }
    // console.log('intermediary child value', value);
  }

  return {
    startPointer,
    children,
    metadata,
    value,
    endPointer // points EXACTLY at the last meaningful value of the description. in case of `0 0` (no children, no meta) endPointer points to second 0.
  };
};


const processData = data => {
  return {
    nodes: readNextNode(data, 0),
    sum: foundMetadata
  };
}

fs.readFile(
  'C:/Users/somebodyrotten/projects/AoC2018/input_08.txt',
  (_, data) => {
    console.log(processData(String(data).trim().split(' ')));
    // console.log(String(data).trim().split(' ').length);
  }
)