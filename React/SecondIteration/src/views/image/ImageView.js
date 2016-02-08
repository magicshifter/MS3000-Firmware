import React, {PropTypes, Component} from 'react';
import {connect} from 'react-redux';

import {actions} from 'redux/modules/pixels';

import {pixelsType, layoutType, colorType} from 'utils/propTypes';
import {rgba_toString} from 'utils/colors';
import {getPixelId, makePixelsArray} from 'utils/pixels';
import {multimax} from 'utils/math';

import classes from './ImageView.scss';

const mapStateToProps =
  state => {
    const {imageView, pixels, layout} = state;

    return {
      pixels: makePixelsArray(pixels),
      color: imageView.get('color'),
      totalColumns: imageView.get('totalColumns'),
      visibleColumns: imageView.get('visibleColumns'),
      rows: imageView.get('rows'),
      layout: layout.toJS(),
    };
  };

export class ImageView extends Component {
  static propTypes = {
    pixelClick: PropTypes.func.isRequired,
    pixels: pixelsType.isRequired,
    rows: PropTypes.number.isRequired,
    color: colorType.isRequired,
    visibleColumns: PropTypes.number.isRequired,
    totalColumns: PropTypes.number.isRequired,
    layout: layoutType,
  };

  constructor(props) {
    super(props);

    this.onMouseOver = this.onMouseOver.bind(this);
  }

  onMouseOver(e, pixel) {
    const {pixelClick, color} = this.props;
    if (e.buttons === 1) {
      pixelClick({pixel, color});
    }
  }

  renderPixel(column, row, pxSize) {
    const {totalColumns, visibleColumns, pixels, pixelClick, color} = this.props;

    const pixelId = getPixelId(totalColumns, column, row);
    const pixel = pixels[pixelId];

    return (
      column < visibleColumns &&
      <td
        className={classes['pixel']}
        key={`r-${row + 1}-c-${column + 1}`}
        onClick={() => pixelClick({pixel, color})}
        onMouseOver={e => this.onMouseOver(e, pixel)}
        style={{
          width: pxSize,
          height: pxSize,
          backgroundColor: rgba_toString(pixel.color),
        }}
      ></td>
    );
  }

  renderPixels() {
    const {layout, rows, totalColumns, visibleColumns} = this.props;

    const widthMargin = layout.width * 0.2;
    const heightMargin = layout.height * 0.2;
    const maxWidth = layout.width - widthMargin;
    const maxHeight = layout.height - heightMargin;

    const pixelListSize = multimax(layout.width, maxWidth, maxHeight);
    var pxSize = Math.max(Math.floor(pixelListSize / (Math.max(visibleColumns, rows) + 1)), 1);

    let rowArray = [];
    for (let i = 0; i < rows; i++) {
      rowArray.push(i);
    }

    let columnArray = [];
    for (let i = 0; i < totalColumns; i++) {
      columnArray.push(i);
    }

    const style = {
      tr: {
        height: pxSize,
      },
    };

    return rowArray.map(
      row => (
        <tr
          key={`r-${row + 1}`}
          style={style.tr}
        >
          {columnArray.map(column => this.renderPixel(column, row, pxSize))}
        </tr>
      )
    );
  }

  render() {
    const {
      layout, // layout state object
    } = this.props;

    const style = {
      table: {},
    };

    if (layout.width < 500) {
      style.table = {
        paddingTop: '1em',
      };
    }

    return (
      <div className={classes['container']}>

        <table
          className={classes['list']}
          style={style.table}
        >
          <tbody>
            {this.renderPixels()}
          </tbody>
        </table>
      </div>
    );
  }
}

export default connect(mapStateToProps, actions)(ImageView);
