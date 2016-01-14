import {getPixelId} from 'utils/pixels';
import {rgba_fromArray} from 'utils/colors';
import {isColor} from 'utils/types';

export const getImagePixels =
  (e, pixels, totalColumns, maxWidth = 16, maxHeight = 16, cb = () => {}) => {
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
          height = parseInt(width * heightWidthRatio, 10);
        }
      } else if (width < height) {
        if (parseInt(height, 10) > maxHeight) {
          height = maxHeight;
          width = parseInt(height * widthHeightRatio, 10);
        }
      }

      ctx.drawImage(loadedImg, 0, 0, width, height);
      let pixelData = ctx.getImageData(0, 0, width, height).data;

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

      const pixelColors = rgba_fromArray(pixelData);

      for (let column = 0; column < totalColumns; column++) {
        for (let row = 0; row < height; row++) {
          const id = getPixelId(totalColumns, column + 1, row + 1);
          const pixel = pixels[id];
          if (pixel && pixel.color) {
            const colorId = getPixelId(width, column + 1, row + 1);
            const color = pixelColors[colorId];
            if (isColor(color)) {
              console.log('set pixel with id', id, 'in row', row, 'and column', column);
              pixels[id].color = color;
            }
          }
        }
      }

      cb(pixels);
    };
  };
