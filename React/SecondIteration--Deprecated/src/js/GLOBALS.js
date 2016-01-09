import Immutable from 'immutable';

const rows = 16;
const columns = 16;

let pixels = [];

for (let column = 0; column < columns; column++) {
  for (let row = 0; row < rows; row++) {
    pixels.push({
      color: {r: 0, b: 0, g: 0, a: 155},
      row,
      column,
    });
  }
}


export default Immutable.fromJS({
  settings: {
    host: 'magicshifter.local',
    port: 80,
  },
  pixelEditor: {
    rows,
    columns,
    pixels,
    color: {r: 0, g: 0, b: 0, a: 155},
  },
});


