export const getImagePixels =
  // totalColumns: 96, actual maximum number of spawned pixels
  // maxWidth: the visibleColumns of the imageView state
  // maxHeight: maximum Rows, should be == 16 and never change
  (file, canvas, pixels, totalColumns, maxWidth = 16, maxHeight = 16, cb = () => {}) => {
    file.url = window.URL.createObjectURL(file);

    let img = new window.Image();
    img.src = file.url;

    const ctx = canvas.getContext('2d');

    img.onload =
      e => {
        const loadedImg = e.path[0];
        let { width, height } = loadedImg;

        ctx.clearRect(0, 0, canvas.width, canvas.height);
        ctx.drawImage(loadedImg, 0, 0, width, height);
        let rawData = ctx.getImageData(0, 0, width, height).data;

        const w = Math.min(width, totalColumns);
        const h = Math.min(height, maxHeight);

        for (let column = 0; column < w; column++) {
          for (let row = 0; row < h; row++) {
            if (column < width && row < height) {
              const pixelIdx = column + (row * totalColumns);
              const rawIdx = 4 * (column + row * width);

              var pixel = pixels[pixelIdx];
              // console.log('set pixel with index', pixelIdx, 'in row', row, 'and column', column);

              pixel.color = { r: rawData[rawIdx + 0], g: rawData[rawIdx + 1], b: rawData[rawIdx + 2], a: 255 };
              // console.log(pixel.color);
            }
          }
        }
        const data = canvas.toDataURL();

        cb({ data, pixels, width, height });
      };
  };
