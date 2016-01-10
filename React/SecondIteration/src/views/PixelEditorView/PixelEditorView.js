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


    var w = this.props.columns;
    var h = this.props.rows;
   
    var fileSize = w*h*4;
    var fileData = new Uint8Array(fileSize);
    fileData[0] = 32;
    fileData[1] = 1;
    fileData[2] = 0xFF;

    var blob = new Blob(fileData);

    formData.append("uploadFile", blob);

    var request = new XMLHttpRequest();
    request.onload = function(oEvent) {
      if (request.status == 200) {
        console.log("Uploaded!");
      } else {
        console.warn("Error " + request.status + " occurred when trying to upload your file.");
      }
    };
    request.open("POST", "http://foo.com/submitform.php");
    request.send(formData);
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
      <input type="button" onClick={this.handleUploadClick.bind(this)} value="send to MS3000" />
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
