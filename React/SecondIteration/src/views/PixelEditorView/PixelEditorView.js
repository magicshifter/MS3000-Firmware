import React, {PropTypes, Component} from 'react';
import {connect} from 'react-redux';

import {actions} from 'redux/modules/pixelEditor';

import Pixel from './Pixel';

import RGBAInput from 'components/inputs/RGBAInput';
import ImageInput from 'components/inputs/ImageInput';

const mapStateToProps = (state) => {
  return state.pixelEditor;
};

export class PixelEditorView extends Component {
  static propTypes = {
    pixels: PropTypes.array.isRequired,
    rows: PropTypes.number.isRequired,
    columns: PropTypes.number.isRequired,
    pixelClick: PropTypes.func.isRequired,
    setColor: PropTypes.func.isRequired,
    setColorValue: PropTypes.func.isRequired,
    color: PropTypes.shape({
      r: PropTypes.number,
      g: PropTypes.number,
      b: PropTypes.number,
      a: PropTypes.number,
    }).isRequired,
  };

  constructor(props) {
    super(props);

    this.renderPixel = this.renderPixel.bind(this);
  }

  renderPixel(pixel) {
    const {row, column, color} = pixel;
    const {columns, pixelClick} = this.props;

    return (
      <Pixel
        key={`${column}-${row}`}
        row={row}
        column={column}
        columns={columns}
        color={color}
        pixelClick={pixelClick}
      />
    );
  }

  handleUploadClick() {
    var formData = new FormData();

  // formData.append("username", "Groucho");
  // formData.append("accountnum", 123456); // number 123456 is immediately converted to a string "123456"

  //   // write header
  // shifterFileData[0] = 0x23;
  // shifterFileData[1] = (shifterFileSize & 0xFF0000) >> 16;
  // shifterFileData[2] = (shifterFileSize & 0xFF00) >> 8;
  // shifterFileData[3] = (shifterFileSize & 0xFF) >> 0;
  // shifterFileData[4] = bitPerPixel;
  // shifterFileData[5] = (frames-1); // 0 for static images larger for animations and fonts
  // shifterFileData[6] = frameWidth;
  // shifterFileData[7] = frameHeight;
  // shifterFileData[8] = subType == "font" ? 0xF0 : subType == "bitmap" ? 0xBA : 0x00;
  // shifterFileData[9] = firstChar; // >= 1 for fonts/ 0 for animations
  // shifterFileData[10] = (delayMs & 0xFF00) >> 8; // 0 for fonts
  // shifterFileData[11] = (delayMs & 0xFF) >> 0;
  // for (var idx = 12; idx < headerSize; idx++) {
  //   shifterFileData[idx] = 0xFF;
  // }

    var w = this.props.columns;
    var h = this.props.rows;
    var pixels = this.props.pixels;

    var fileSize = w * h * 4;
    var fileData = new Uint8Array(fileSize);

    var headerSize = 0;

    for (var x = 0; x < w; x++) {
      for (var y = 0; y < h; y++) {
        var idx = x + y * w;
        var pixel = pixels[idx];

        var fileDataIdx = headerSize + (x + y * w) * 4;

        fileData[fileDataIdx + 0] = pixel.color.r;
        fileData[fileDataIdx + 1] = pixel.color.g;
        fileData[fileDataIdx + 2] = pixel.color.b;
        fileData[fileDataIdx + 3] = 0xFF;
      }
    }

    var blob = new Blob([fileData]);

    formData.append('uploadFile', blob);

    var request = new XMLHttpRequest();
    request.onload = function(oEvent) {
      if (request.status === 200) {
        console.log('Uploaded!');
      } else {
        console.warn('Error ' + request.status + ' occurred when trying to upload your file.');
      }
    };
    request.open('POST', '/upload');
    request.send(formData);

    // var iframe = document.createElement("iframe");
    // body.append(iframe);
  }

  render() {
    const {pixels, color, rows, columns, setColorValue} = this.props;

    const width = window.innerWidth > window.innerHeight
                  ? '56%'
                  : '100%';

    const styles = {
      ul: {
        listStyle: 'none',
        width: width,
        display: 'inline-block',
        margin: 0,
        padding: 0,
      },
    };

    return (
      <div className='pixelEditor container'>
      <input type='button' onClick={this.handleUploadClick.bind(this)} value='send to MS3000' />
        <ul className='pixelList list' style={styles.ul}>
          {pixels && pixels.map(this.renderPixel)}
        </ul>
        <div className='colorPicker'>
          <RGBAInput
            color={color}
            setColorValue={setColorValue}
          />
          <ImageInput
            label='upload image'
            rows={rows}
            columns={columns}
          />
        </div>
      </div>
    );
  }
}

export default connect(mapStateToProps, actions)(PixelEditorView);
