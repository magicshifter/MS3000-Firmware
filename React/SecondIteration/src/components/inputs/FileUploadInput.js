import React, {Component, PropTypes} from 'react';
import {saveAs} from 'filesaver.js';

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
    this.getBlob = this.getBlob.bind(this);
    this.onFileDownload = this.onFileDownload.bind(this);
    this.getFileName = this.getFileName.bind(this);
  }

  CalcBufferSize(bitPerPixel, w, h) {
    if (bitPerPixel === 24) {
      return w * h * 3;
    }
    if (bitPerPixel === 8) {
      return w * h;
    } else if (bitPerPixel === 1) {
      if ((w * h) % 8) {
        alert('CalcBufferSize: Ugly 1bit BufferSize: ' + (w * h / 8));
        return Math.ceil(w * h / 8);
      }
      return w * h / 8;
    } else {
      alert('CalcBufferSize: Unknown bitPerPixel Value: ' + (w * h / 8));
    }
  }

  getBlob() {
    const {height, width, pixels, totalWidth} = this.props;

    const subType = 'bitmap';
    const headerSize = 16;
    const bitPerPixel = 24;
    const delayMs = 500;
    const fileSize = this.CalcBufferSize(bitPerPixel, width, height) + headerSize;
    const frames = 1;
    const firstChar = 0;

    const fileData = new Uint8Array(fileSize);

    // write header
    fileData[0] = 0x23;
    fileData[1] = (fileSize & 0xFF0000) >> 16;
    fileData[2] = (fileSize & 0xFF00) >> 8;
    fileData[3] = (fileSize & 0xFF) >> 0;

    fileData[4] = bitPerPixel;
    fileData[5] = (frames - 1); // 0 for static images larger for animations and fonts
    fileData[6] = width;
    fileData[7] = height;

    fileData[8] = subType === 'font' ? 0xF0 : subType === 'bitmap' ? 0xBA : 0x00;
    fileData[9] = firstChar; // >= 1 for fonts/ 0 for animations
    fileData[10] = (delayMs & 0xFF00) >> 8; // 0 for fonts
    fileData[11] = (delayMs & 0xFF) >> 0;

    for (let x = 0; x < width; x++) {
      for (let y = 0; y < height; y++) {
        const idx = x + (y * totalWidth);
        const pixel = pixels[idx];
        const fileDataIdx = headerSize + 3 * (y + x * width);

        fileData[fileDataIdx + 0] = pixel.color.r;
        fileData[fileDataIdx + 1] = pixel.color.g;
        fileData[fileDataIdx + 2] = pixel.color.b;
      }
    }

    const blob = new Blob([fileData]);
    return blob;
  }

  getFileName() {
    var fileName = this.refs.fileName.value;
    fileName = fileName + '.magicBitmap';
    return fileName;
  }

  onFileDownload() {
    const blob = this.getBlob();
    const fileName = this.getFileName();
    saveAs(blob, fileName);
  }

  onClick() {
    const blob = this.getBlob();
    const fileName = this.getFileName();

    var url = this.props.url;
    // alert(url);
    if (url === 'http://') {
      url = '';
    }
    // alert(url);

    const formData = new FormData();
    formData.append('uploadFile', blob, fileName);

    const request = new XMLHttpRequest();
    request.onload =
      () =>
        request.status === 200
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
      <div className={classes['input']}>
        <label>Filename:</label>
        <input type='text' defaultValue='userImage' ref='fileName' />

        {label && <label>{label}</label>}

        <input
          type='button'
          onClick={this.onClick}
          value={text}
        />
        <input
          type='button'
          onClick={this.onFileDownload}
          value='download file'
        />
      </div>
    );
  }
}

