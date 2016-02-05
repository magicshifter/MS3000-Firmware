import React, {PropTypes, Component} from 'react';
import {connect} from 'react-redux';

import {actions} from 'redux/modules/pixels';

import {pixelsType, layoutType, colorType} from 'utils/propTypes';
import {getPixelId, makePixelsArray} from 'utils/pixels';
import {multimax} from 'utils/math';

import Pixel from 'components/PixelEditor/Pixel';

import classes from './PixelEditor.scss';

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

export class PixelEditor extends Component {
  static propTypes = {
    pixels: pixelsType.isRequired,
    rows: PropTypes.number.isRequired,
    color: colorType.isRequired,
    visibleColumns: PropTypes.number.isRequired,
    totalColumns: PropTypes.number.isRequired,
    layout: layoutType,
  };

  constructor(props) {
    super(props);

    this.defaultStyle = {
      table: {},
      tr: {},
    };
  }

  render() {
    const {layout, rows, totalColumns, visibleColumns, pixels} = this.props;

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

    const style = this.defaultStyle;

    style.tr.height = pxSize;

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
            {rowArray.map(
              row => (
                <tr
                  key={`r-${row + 1}`}
                  style={style.tr}
                >
                  {columnArray.map(
                    column => {
                      const pixelId = getPixelId(totalColumns, column, row);
                      const pixel = pixels[pixelId];
                      if (column < visibleColumns) {
                        return (
                          <Pixel
                            key={`r-${row + 1}-c-${column + 1}`}
                            pixel={pixel}
                            size={pxSize}
                          />
                        );
                      }
                    }
                  )}
                </tr>
              )
            )}
          </tbody>
        </table>
      </div>
    );
  }
}

export default connect(mapStateToProps, actions)(PixelEditor);
