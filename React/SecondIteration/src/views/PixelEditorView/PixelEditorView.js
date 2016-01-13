import React, {PropTypes, Component} from 'react';
import {connect} from 'react-redux';

import {actions} from 'redux/modules/pixelEditor';

import {pixelEditorType, layoutType} from 'utils/propTypes';
import {rgba_toString} from 'utils/colors';
import {getPixelId} from 'utils/pixels';
import {multimax} from 'utils/math';

import PixelEditorSidebar from './PixelEditorSidebar';

import classes from './PixelEditorView.scss';

const mapStateToProps = (state) => {
  const {pixelEditor, layout} = state;
  return {
    pixelEditor: pixelEditor.toJS(),
    layout: layout.toJS(),
  };
};

export class PixelEditorView extends Component {
  static propTypes = {
    pixelClick: PropTypes.func.isRequired,
    pixelEditor: pixelEditorType,
    layout: layoutType,
  };

  render() {
    const {
      pixelClick, // actions
      pixelEditor, layout, // state objects
    } = this.props;

    const {pixels, visibleColumns, totalColumns, rows} = pixelEditor;

    const {sidebar, header} = layout;

    const maxWidth = layout.width - sidebar.width;
    const maxHeight = layout.height - header.height;
    const pixelListSize = multimax(layout.width, maxWidth, maxHeight);

    let rowArray = [];
    for (let i = 0; i < rows; i++) {
      rowArray.push(i);
    }

    let columnArray = [];
    for (let i = 0; i < totalColumns; i++) {
      columnArray.push(i);
    }

    const pxSize = (pixelListSize - 1) / visibleColumns;

    return (
      <div className={classes['container']}>

        <table
          className={classes['list']}
          style={{
            height: pixelListSize,
            width: pixelListSize,
          }}
        >
          <tbody>
            {rowArray.map(
              r =>
                <tr
                  key={`r-${r + 1}`}
                  style={{
                    height: pxSize,
                  }}
                >
                  {columnArray.map(
                    c =>
                      c < visibleColumns &&
                      <td
                        className={classes['pixel']}
                        key={`r-${r + 1}-c-${c + 1}`}
                        onClick={() => pixelClick(getPixelId(totalColumns, c + 1, r + 1))}
                        style={{
                          width: pxSize,
                          height: pxSize,
                          backgroundColor: rgba_toString(
                            pixels[getPixelId(totalColumns, c + 1, r + 1)].color
                          ),
                        }}
                      ></td>
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
