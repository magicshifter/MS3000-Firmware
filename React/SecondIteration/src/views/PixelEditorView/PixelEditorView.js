import React, {PropTypes, Component} from 'react';
import {connect} from 'react-redux';

import {actions} from 'redux/modules/pixelEditor';

import {colorType} from 'utils/propTypes';
import {rgba_toString} from 'utils/colors';

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

    pixelEditor: PropTypes.shape({
      pixels: PropTypes.array.isRequired,
      rows: PropTypes.number.isRequired,
      columns: PropTypes.number.isRequired,
      color: colorType.isRequired,
    }).isRequired,

    layout: PropTypes.shape({
      height: PropTypes.number.isRequired,
      width: PropTypes.number.isRequired,
    }).isRequired,
  };

  render() {
    const {
      pixelClick, // actions
      pixelEditor, layout, // state objects
    } = this.props;

    const {pixels, columns} = pixelEditor;

    const pixelListSize =
      layout.height > layout.width
      ? layout.width * 0.6
      : layout.height * 0.7;

    const pxSize =
      layout.height > layout.width
      ? pixelListSize / columns
      : pixelListSize / columns;

    return (
      <div className={classes['container']}>

        <ul
          className={classes['list']}
          style={{
            height: pixelListSize,
            width: pixelListSize,
          }}
        >
          {pixels && pixels.map(p => {
            const id = (((p.row - 1) * columns) - 1) + p.column;
            return (
              <li
                key={`${p.column}-${p.row}`}
                className={`${classes['pixel']} id-${id} r-${p.row} c-${p.column}`}
                style={{
                  backgroundColor: rgba_toString(p.color),
                  height: pxSize,
                  width: pxSize,
                }}
                onClick={() => pixelClick(id)}
              ></li>
            );
          })}
        </ul>

        <PixelEditorSidebar />
      </div>
    );
  }
}

export default connect(mapStateToProps, actions)(PixelEditorView);
