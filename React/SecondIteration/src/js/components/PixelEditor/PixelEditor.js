// need to import react here
import React, {Component, PropTypes} from 'react';

import Radium from 'radium';
import ColorPicker from 'react-color';

import Pixel from './Pixel';

@Radium
export default class PixelEditor extends Component {

  static propTypes = {
    rows: PropTypes.number,
    columns: PropTypes.number,
  }

  static defaultProps = {
    rows: 16,
    columns: 16,
  }

  constructor(props) {
    super(props);

    const {rows, columns} = this.props;

    this.state = {
      rows,
      columns,
    };
  }

  render() {
    const {rows, columns} = this.state;

    let pixels = [];
    for (let r = 0; r < rows; r++) {
      for (let c = 0; c < columns; c++) {
        pixels.push(
          <Pixel
            key={`row-${r}-col-${c}`}
            row={r}
            column={c}
            rows={rows}
            columns={columns}
            container={this._container}
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
          <ColorPicker type='sketch' />
        </div>
      </section>
    );
  }
}
