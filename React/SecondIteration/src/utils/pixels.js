/* @flow */

import Immutable from 'immutable';

import {defaultLedColor as color} from 'GLOBALS';

import {isFunction, isArray} from 'utils/types';

export const getPixelId =
  (columns, column, row) =>
    (row * columns) + column;

export const createPixels =
  (totalColumns, visibleColumns, rows) => {
    let pixelArray = [];
    for (let row = 0; row < rows; row++) {
      for (let column = 0; column < totalColumns; column++) {
        const visible = column <= visibleColumns;
        const id = getPixelId(totalColumns, column, row);

        pixelArray.push({
          color,
          totalColumns,
          column: column + 1,
          row: row + 1,
          visible,
          id,
        });
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
    isArray(immutablePixels) &&
    immutablePixels
      .toArray()
      .map(
        px => makePixelObject(px)
      ) ||
    immutablePixels;

export const createImmutablePixels =
  (totalColumns, visibleColumns, rows) =>
    Immutable.List(
      createPixels(totalColumns, visibleColumns, rows)
        .map(px => makePixelImmutable(px)));
