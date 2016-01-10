import React, {Component, PropTypes} from 'react';

import classes from './FileUploadInput.scss';

export default class FileUploadInput extends Component {
  static propTypes = {
    url: PropTypes.string.isRequired,
    pixels: PropTypes.array.isRequired,
    height: PropTypes.number.isRequired,
    width: PropTypes.number.isRequired,
    label: PropTypes.string,
    text: PropTypes.string,
  };

  constructor(props) {
    super(props);

    this.onClick = this.onClick.bind(this);
  }

  onClick() {
    const {height, width, url} = this.props;

    const formData = new FormData();

    // formData.append('username', 'Groucho');
    // number 123456 is immediately converted to a string '123456'
    // formData.append('accountnum', 123456);

    const fileSize = height * width * 4;
    let fileData = new Uint8Array(fileSize);
    fileData[0] = 32;
    fileData[1] = 1;
    fileData[2] = 0xFF;

    const blob = new Blob(fileData);

    formData.append('uploadFile', blob);

    const request = new XMLHttpRequest();
    request.onload = oEvent => {
      if (request.status === 200) {
        console.log('Uploaded!');
      } else {
        console.warn(`Error ${request.status} occurred when trying to upload your file.`);
      }
    };

    request.open('POST', `${url}/upload`);
    request.send(formData);
  }

  render() {
    const {label, text = 'Upload File'} = this.props;

    return (
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
    );
  }
}

