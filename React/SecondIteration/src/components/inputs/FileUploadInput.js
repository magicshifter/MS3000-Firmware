import React, {Component, PropTypes} from 'react';

import classes from './FileUploadInput.scss';

export default class FileUploadInput extends Component {
  static propTypes = {
    url: PropTypes.string.isRequired,
    pixels: PropTypes.array.isRequired,
    height: PropTypes.number.isRequired,
    width: PropTypes.number.isRequired,
    totalWidth: PropTypes.number.isRequired,
    label: PropTypes.string,
    text: PropTypes.string,
  };

  constructor(props) {
    super(props);

    this.onClick = this.onClick.bind(this);
  }

  onClick() {
    const {height, width, pixels, totalWidth} = this.props;
    const fileSize = width * height * 4;
    const fileData = new Uint8Array(fileSize);

    const headerSize = 0;

    var fileName = this.refs.fileName.value;
    fileName = fileName + '.magicBitmap';
    var url = 'http://magicshifter.local';

    for (let x = 0; x < width; x++) {
      for (let y = 0; y < height; y++) {
        const idx = x + (y * totalWidth);
        const pixel = pixels[idx];
        const fileDataIdx = headerSize + 4 * (y + x * width);

        fileData[fileDataIdx + 0] = pixel.color.b;
        fileData[fileDataIdx + 1] = pixel.color.g;
        fileData[fileDataIdx + 2] = pixel.color.r;
        fileData[fileDataIdx + 3] = 0xFF;
      }
    }

    const blob = new Blob([fileData]);

    const formData = new FormData();
    formData.append('uploadFile', blob, fileName);

    const request = new XMLHttpRequest();
    request.onload =
      () =>
        request.status === '200'
        ? console.log('Uploaded!')
        : console.warn(`Error ${status} occurred when trying to upload your file.`);

    request.timeout = 3000;
    request.ontimeout =
      () =>
        console.warn(`Connection to ${url} timed out!!!`);

    request.open('POST', `${url}/upload`);
    request.send(formData);
  }

  render() {
    const {label, text = 'Upload File'} = this.props;

    return (
      <div>
        <input type='text' defaultValue='userImage' ref='fileName' />
        <div className={classes['input']}>

          {label &&
            <label>{label}</label>
          }
          <input
            type='button'
            onClick={this.onClick}
            value={text}
          />
        </div>
      </div>
    );
  }
}

