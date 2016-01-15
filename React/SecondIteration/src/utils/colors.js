import rgba from 'rgba-convert';

import {isString, isObject, isArray} from 'utils/types';

export const rgba_toString =
  color =>
    rgba.css(color);

export const colorsTo =
  (convertTo, colors) => {
    let newColors = {};

    Object.keys(colors)
      .forEach(key => {
        const color = colors[key];
        if (isString(color)) {
          if (convertTo === 'obj') {
            newColors[key] = rgba.obj(color);
          } else {
            newColors[key] = rgba(color);
          }
        } else if (isArray(color) || isObject(color)) {
          if (convertTo === 'css') {
            newColors[key] = rgba.css(color);
          } else {
            newColors[key] = colorsTo(convertTo, colors[key]);
          }
        }
      });

    return newColors;
  };

export const colorsToRGBA =
  colors =>
    colorsTo('rgba', colors);

export const colorsToCSS =
  colors =>
    colorsTo('css', colors);

export const colorsToObj =
  colors =>
    colorsTo('obj', colors);

export const rgba_fromArray =
  pixelArray => {
    let convertedColors = [];
    const colorNames = ['r', 'b', 'g', 'a'];

    for (let currentColorId = 0; currentColorId < pixelArray.length; currentColorId += 4) {
      let color = {};

      Object.keys(colorNames)
        .forEach(key => {
          const colorName = colorNames[key];
          const colorKey = currentColorId + key;
          color[colorName] = pixelArray[colorKey];
        });

      convertedColors.push(color);
    }

    return convertedColors;
  };
