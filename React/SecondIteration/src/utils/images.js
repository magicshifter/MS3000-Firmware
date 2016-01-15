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

      ctx.drawImage(loadedImg, 0, 0, width, height);
      let rawData = ctx.getImageData(0, 0, width, height).data;

      const w = Math.min(width, totalColumns, maxWidth);
      const h = Math.min(height, maxHeight);

      for (let column = 0; column < w; column++) {
        for (let row = 0; row < h; row++) {
          const pixelIdx = column + (row * totalColumns);
          const rawIdx = 4 * (column + row * width);

          var pixel = pixels[pixelIdx];
          console.log('set pixel with index', pixelIdx, 'in row', row, 'and column', column);

          pixel.color = {r: rawData[rawIdx + 0], g: rawData[rawIdx + 1], b: rawData[rawIdx + 2], a: 255};
          console.log(pixel.color);
        }
      }

      cb(pixels);
    };
  };
