
export function getImagePixels(e, maxWidth = 16, maxHeight = 16, cb = () => {}) {
  // console.log('picUpload runs: ', {e});
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

    cb(pixels);
  };
}
