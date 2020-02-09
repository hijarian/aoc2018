const fs = require('fs');


const placeMarble = (currentBoard, currentMarbleIndex, newMarbleValue) => {
  let newBoard = currentBoard.slice();
  let points = 0;
  let newCurrentMarbleIndex;

  if (newMarbleValue % 23 === 0) {
    points += newMarbleValue;
    let pointsMarbleIndex = currentMarbleIndex - 7;
    if (pointsMarbleIndex < 0) {
      pointsMarbleIndex = currentBoard.length + pointsMarbleIndex;
    }
    points += newBoard.splice(pointsMarbleIndex, 1)[0]; // splice which deleted elements returns array with deleted elements
    newCurrentMarbleIndex = pointsMarbleIndex;
  } else {
    let splicePointIndex = currentMarbleIndex + 2;
    if (splicePointIndex >= currentBoard.length) {
      splicePointIndex = splicePointIndex - currentBoard.length;
    }

    newBoard.splice(splicePointIndex, 0, newMarbleValue);
    newCurrentMarbleIndex = splicePointIndex;
  }

  return {
    board: newBoard,
    currentMarbleIndex: newCurrentMarbleIndex,
    points
  }
};

const printState = (currentPlayer, board, currentMarbleIndex) => {
  let stateString = `${currentPlayer}:  `;

  stateString += board.map((value, index) => index === currentMarbleIndex ? `(${value})` : `${value}`).join(' ');

  console.log(stateString);
};


// Demo example
// const lastMarble = 25;
// const numberOfPlayers = 9;

// Second example
// const lastMarble = 1618;
// const numberOfPlayers = 10;

// Third example
// const lastMarble = 7999;
// const numberOfPlayers = 13;

// My puzzle variant 
const lastMarble = 72164;
const numberOfPlayers = 419;

let playerPoints = {};
let board = [0, 2, 1, 3];
let currentPlayer = 4;
let currentMarbleIndex = 3;
let results;
for (let i = 4; i <= lastMarble; ++i) {
  results = placeMarble(board, currentMarbleIndex, i);
  board = results.board;
  currentMarbleIndex = results.currentMarbleIndex;

  if (results.points) {
    if (!playerPoints[currentPlayer]) {
      playerPoints[currentPlayer] = 0;
    }
    playerPoints[currentPlayer] += results.points;
  }

  if (lastMarble < 30) {
    printState(currentPlayer, board, currentMarbleIndex);
  } 

  ++currentPlayer;
  if (currentPlayer > numberOfPlayers) {
    currentPlayer = 1;
  }
}

console.log(Object.entries(playerPoints).sort(([, leftScore], [, rightScore]) => rightScore - leftScore));
