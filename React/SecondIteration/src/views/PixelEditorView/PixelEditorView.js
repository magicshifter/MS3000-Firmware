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
