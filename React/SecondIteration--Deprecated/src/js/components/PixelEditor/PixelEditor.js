// need to import react here
import React, {Component, PropTypes} from 'react';
import Radium from 'radium';
import ColorPicker from 'react-color';
import objectAssign from 'object-assign';

import {dispatch} from '../../Dispatcher';

import Pixel from './Pixel';
import RGBAInput from '../Inputs/RGBAInput';
import ImageInput from '../Inputs/ImageInput';

@Radium
export default class PixelEditor extends Component {

  static propTypes = {
    rows: PropTypes.number,
    columns: PropTypes.number,
    color: PropTypes.shape({
      a: PropTypes.number,
      b: PropTypes.number,
      g: PropTypes.number,
      r: PropTypes.number,
    }),
  }

  render() {
    const {rows, columns, color, alpha} = this.props;
    console.log('props', this.props);

    let pixels = [];
    for (let r = 0; r < rows; r++) {
      for (let c = 0; c < columns; c++) {
        pixels.push(
          <Pixel
            color={color}
            key={`row-${r}-col-${c}`}
            row={r}
            column={c}
            rows={rows}
            columns={columns}
          />
        );
      }
    }

    const styles = {
      container: {
        listStyle: 'none',
        margin: 0,
        padding: 0,
        display: 'inline-block',
        width: '60%',
      },
      colorPickerContainer: {
        width: '39%',
        float: 'right',
      },
    };

    return (
      <section className='pixelEditor'>
        <ul
          className='pixelContainer'
          style={styles.container}
        >
          {pixels}
        </ul>
        <div
          className='colorPickerContainer'
          style={styles.colorPickerContainer}
        >
          <RGBAInput
            color={color}
          />
          <ImageInput label='upload image' rows={rows} columns={columns} />
        </div>
      </section>
    );
  }
}
