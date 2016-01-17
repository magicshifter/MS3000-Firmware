import Immutable from 'immutable';

import {isFunction} from 'utils/types';

export const getPixelId =
  (columns, column, row) =>
    (row * columns) + column;

export const createPixel =
  (color, columns, column, row, visible = false) => ({
    id: getPixelId(columns, column, row),
    color: color,
    row,
    column,
    visible,
  });

export const createPixels =
  (totalColumns, visibleColumns, rows) => {
    let pixelArray = [];
    for (let row = 0; row < rows; row++) {
      for (let column = 0; column < totalColumns; column++) {
        const visible = column <= visibleColumns;
        pixelArray.push(
          createPixel(
            {r: 0, b: 0, g: 0, a: 255},
            totalColumns,
            column + 1,
            row + 1,
            visible,
          )
        );
      }
    }

    return pixelArray;
  };

export const makePixelImmutable =
  pixel =>
    Immutable.Map({
      ...pixel,
      ...{color: Immutable.Map(pixel.color)},
    });

export const makePixelObject =
  immutablePixel =>
    isFunction(immutablePixel.toJS)
    ? immutablePixel.toJS()
    : immutablePixel;

export const makePixelsObject =
  immutablePixels =>
    immutablePixels
      .toArray()
      .map(px => makePixelObject(px));

export const createImmutablePixels =
  (totalColumns, visibleColumns, rows) =>
    Immutable.List(
      createPixels(totalColumns, visibleColumns, rows)
        .map(px => makePixelImmutable(px)));
