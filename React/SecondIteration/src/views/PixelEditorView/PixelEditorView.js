import React, {PropTypes, Component} from 'react';
import {connect} from 'react-redux';

import {actions} from 'redux/modules/pixelEditor';

import {pixelsType, layoutType} from 'utils/propTypes';
import {rgba_toString} from 'utils/colors';
import {getPixelId, makePixelsObject} from 'utils/pixels';
import {multimax} from 'utils/math';

import PixelEditorSidebar from './PixelEditorSidebar';

import classes from './PixelEditorView.scss';

const mapStateToProps = (state) => {
  const {pixelEditor, layout} = state;
  return {
    pixels: makePixelsObject(pixelEditor.get('pixels')),
    totalColumns: pixelEditor.get('totalColumns'),
    visibleColumns: pixelEditor.get('visibleColumns'),
    rows: pixelEditor.get('rows'),
    layout: layout.toJS(),
  };
};

export class PixelEditorView extends Component {
  static propTypes = {
    pixelClick: PropTypes.func.isRequired,
    pixels: pixelsType.isRequired,
    rows: PropTypes.number.isRequired,
    visibleColumns: PropTypes.number.isRequired,
    totalColumns: PropTypes.number.isRequired,
    layout: layoutType,
  };

  render() {
    const {
      pixelClick, // actions
      layout, // layout state object
      pixels, visibleColumns, totalColumns, rows, // pixelEditor state
    } = this.props;

    const {sidebar, header} = layout;

    const maxWidth = layout.width - (layout.width > 500 ? sidebar.width : 0);
    const maxHeight = layout.height - header.height;

    const pixelListSize = multimax(layout.width, maxWidth, maxHeight);
    var pxSize = Math.floor(pixelListSize / (Math.max(visibleColumns, rows) + 1));
    if (pxSize < 1) {
      pxSize = 1; // paranoia!!!
    }

    let rowArray = [];
    for (let i = 0; i < rows; i++) {
      rowArray.push(i);
    }

    let columnArray = [];
    for (let i = 0; i < totalColumns; i++) {
      columnArray.push(i);
    }

    const style = {
      table: {},
      tr: {
        height: pxSize,
      },
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
            {rowArray.map(
              r =>
                <tr
                  key={`r-${r + 1}`}
                  style={style.tr}
                >
                  {columnArray.map(
                    c => {
                      const pixelId = getPixelId(totalColumns, c, r);
                      const pixel = pixels[pixelId];
                      return (
                        c < visibleColumns &&
                        <td
                          className={classes['pixel']}
                          key={`r-${r + 1}-c-${c + 1}`}
                          onClick={() => pixelClick(pixelId)}
                          style={{
                            width: pxSize,
                            height: pxSize,
                            backgroundColor: rgba_toString(pixel.color),
                          }}
                        ></td>
                      );
                    }
                  )}
                </tr>
            )}
          </tbody>
        </table>

        <PixelEditorSidebar />
      </div>
    );
  }
}

export default connect(mapStateToProps, actions)(PixelEditorView);
