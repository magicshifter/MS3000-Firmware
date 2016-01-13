import Immutable from 'immutable';

import {getPixelId, createPixel} from 'utils/pixels';

export const convertArrayToPixels =
  pixelArray => {
    let convertedColors = [];
    let currentColorId = 0;
    let currentColor = {};
    const colorNames = ['r', 'b', 'g', 'a'];

    for (let pixelColorId = 0; pixelColorId < pixelArray.length; pixelColorId++) {
      const currentColorName = colorNames[currentColorId];
      currentColor[currentColorName] = pixelArray[pixelColorId];

      currentColorId += 1;
      if (currentColorId === 4) {
        convertedColors.push(Immutable.Map(currentColor));
        currentColor = {};
        currentColorId = 0;
      }
    }

    return convertedColors;
  };

export const getImagePixels =
  (e, totalColumns, maxWidth = 16, maxHeight = 16, cb = () => {}) => {
    const {files} = e.target;

    const file = files[0];

    file.url = URL.createObjectURL(file);

    let img = new Image();
    img.src = file.url;

    const canvas = document.createElement('canvas');
    const ctx = canvas.getContext('2d');

    img.onload = (e) => {
      const loadedImg = e.path[0];
      let {width, height} = loadedImg;
      const widthHeightRatio = width / height;
      const heightWidthRatio = height / width;

      if (width > height) {
        if (parseInt(width, 10) > maxWidth) {
          width = maxWidth;
          height = width * heightWidthRatio;
        }
      } else if (width < height) {
        if (parseInt(height, 10) > maxHeight) {
          height = maxHeight;
          width = height * widthHeightRatio;
        }
      }

      ctx.drawImage(loadedImg, 0, 0, width, height);
      let pixels = ctx.getImageData(0, 0, width, height).data;

      // console.log({width, maxWidth, height, maxHeight});

      // center image horizontally and vertically
      // if (width < maxWidth) {
        // const diff = parseInt((maxWidth - width ) / 2);
        // if (diff > 0) {
          // console.log('width diff', diff);
        // }
      // }
      // if (height < maxHeight) {
        // const diff = parseInt((maxHeight - height) / 2);
        // if (diff > 0) {
          // console.log('height diff', diff);
        // }
      // }

      console.log('converted pixels');
      const pixelColors = convertArrayToPixels(pixels);

      let convertedPixels = [];
      for (let column = 0; column < maxWidth; column++) {
        for (let row = 0; row < maxHeight; row++) {
          const color = pixelColors[getPixelId(maxWidth, column, row)];

          convertedPixels.push(createPixel(color, row, column));
        }
      }
      console.log(convertedPixels);
      cb(pixels);
    };
  };
