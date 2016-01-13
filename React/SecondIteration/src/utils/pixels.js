import Immutable from 'immutable';

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
            {r: 0, b: 0, g: 0, a: 155},
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
  (pixel) => {
    const {color, id, row, column, visible} = pixel;
    const immutableColor = Immutable.Map(color);
    return Immutable.Map({
      color: Immutable.Map(color),
      id,
      row,
      column,
      visible,
    });
  }

export const makePixelObject =
  immutablePixel => {
    const pixel = immutablePixel.toObject();
    pixel.color = pixel.color.toObject();
    return pixel;
  };

export const makePixelsObject =
  immutablePixels => {
    const pixels = immutablePixels.toArray();
    return pixels.map(px => makePixelObject(px));
  };

export const createImmutablePixels =
  (totalColumns, visibleColumns, rows) =>
    Immutable.List(
      createPixels(totalColumns, visibleColumns, rows)
        .map(px => makePixelImmutable(px)));
