// need to import react here
import React, {Component, PropTypes} from 'react';
import Radium from 'radium';
import ColorPicker from 'react-color';
import objectAssign from 'object-assign';

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

  static defaultProps = {
    rows: 16,
    columns: 16,
    color: {
      a: 155,
      b: 0,
      g: 0,
      r: 0,
    },
  }

  constructor(props) {
    super(props);

    console.log(props);

    const {rows, columns, color, alpha} = props;

    this.state = {
      rows,
      columns,
      color,
      alpha,
    };

    // this feels dirty, but more clear than binding in the event call.
    this.onColorChange = this.onColorChange.bind(this);
    this.onColorInputChange = this.onColorInputChange.bind(this);
  }

  onColorChange(e) {
    const {r, g, b} = e.rgb;
    const color = objectAssign(this.state.color, {r, g, b});
    this.setState({
      color,
    });
  }

  onColorInputChange(newColor) {
    const {name, value} = newColor;
    const color = objectAssign(this.state.color, {[name]: parseInt(value)});
    this.setState({
      color,
    });
  }

  render() {
    const {rows, columns, color, alpha} = this.state;

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
            onChange={this.onColorInputChange}
          />
          <ImageInput label='upload image' rows={rows} columns={columns} />
        </div>
      </section>
    );
  }
}
