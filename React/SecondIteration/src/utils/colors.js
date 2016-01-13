import {isRGBAObject} from 'utils/types';

export function rgba_toString(color) {
  if (!isRGBAObject(color)) {
    console.error('invalid color passed to utils/colors.js -> rgba_toString', color);
    return color;
  }
  const {r, g, b, a} = color;
  return `rgba(${r}, ${g}, ${b}, ${a})`;
}

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
