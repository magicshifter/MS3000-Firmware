import React, {PropTypes, Component} from 'react';
import {connect} from 'react-redux';

import {actions} from 'redux/modules/pixels';

import {pixelsType, layoutType, colorType} from 'utils/propTypes';
import {getPixelId, makePixelsArray} from 'utils/pixels';
import {multimax} from 'utils/math';

import Pixel from 'components/PixelEditor/Pixel';
import PixelEditorMenu from 'components/PixelEditor/PixelEditorMenu';

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

  render() {
    const {layout, rows, totalColumns, visibleColumns, pixels} = this.props;

    const zoom = layout.zoomLevel;
    const widthMargin = layout.width * 0.2;
    const heightMargin = layout.height * 0.2;
    const maxWidth = (layout.width - widthMargin) * zoom;
    const maxHeight = (layout.height - heightMargin) * zoom;

    const pixelListSize = multimax(layout.width, maxWidth, maxHeight);
    var pxSize = Math.max(Math.floor(pixelListSize / (Math.max(visibleColumns, rows) + 1)), 1) * zoom;

    let rowArray = [];
    for (let i = 0; i < rows; i++) {
      rowArray.push(i);
    }

    let columnArray = [];
    for (let i = 0; i < totalColumns; i++) {
      columnArray.push(i);
    }

    return (
      <div className={classes['container']}>
        <table className={classes['list']}>
          <tbody>
            {rowArray.map(
              row => (
                <tr key={`r-${row + 1}`}>
                  {columnArray.map(
                    column => {
                      if (column < visibleColumns) {
                        const pixelId = getPixelId(totalColumns, column, row);
                        const pixel = pixels[pixelId];

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
        <PixelEditorMenu />
      </div>
    );
  }
}

export default connect(mapStateToProps, actions)(PixelEditor);
