import React, {PropTypes, Component} from 'react';
import {connect} from 'react-redux';

import {actions} from 'redux/modules/pixelEditor';

import {colorType} from 'utils/propTypes';
import {rgba_toString} from 'utils/colors';

// import Pixel from 'components/inputs/Pixel';

import RGBAInput from 'components/inputs/RGBAInput';
import NumberInput from 'components/inputs/NumberInput';
import ImageInput from 'components/inputs/ImageInput';
import FileUploadInput from 'components/inputs/FileUploadInput';

import classes from './PixelEditorView.scss';

const mapStateToProps = (state) => {
  const {pixelEditor, settings, layout} = state;
  return {
    pixelEditor: pixelEditor.toJS(),
    settings: settings.toJS(),
    layout: layout.toJS(),
  };
};

export class PixelEditorView extends Component {
  static propTypes = {
    pixelClick: PropTypes.func.isRequired,
    setColorValue: PropTypes.func.isRequired,
    setColumns: PropTypes.func.isRequired,

    pixelEditor: PropTypes.shape({
      pixels: PropTypes.array.isRequired,
      rows: PropTypes.number.isRequired,
      columns: PropTypes.number.isRequired,
      color: colorType.isRequired,
    }).isRequired,

    settings: PropTypes.shape({
      protocol: PropTypes.string.isRequired,
      host: PropTypes.string.isRequired,
    }).isRequired,

    layout: PropTypes.shape({
      height: PropTypes.number.isRequired,
      width: PropTypes.number.isRequired,
    }).isRequired,
  };

  render() {
    const {
      setColorValue, setColumns, pixelClick, // actions
      pixelEditor, settings, layout, // state objects
    } = this.props;

    const {pixels, color, rows, columns} = pixelEditor;
    const {host, protocol} = settings;

    const pixelListSize =
      layout.height > layout.width
      ? layout.width * 0.6
      : layout.height * 0.6;

    const pxSize =
      layout.height > layout.width
      ? pixelListSize / columns
      : pixelListSize / columns;

    const controlSize =
      layout.height > layout.width
      ? layout.width * 0.39
      : layout.height * 0.39;

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

        <div
          className={classes['controls']}
          style={{
            width: controlSize,
          }}
        >
          <div className={classes['picker']}>
            <RGBAInput
              color={color}
              setColorValue={setColorValue}
            />

            <ImageInput label='upload image' />

            <FileUploadInput
              pixels={pixels}
              height={rows}
              width={columns}
              url={[protocol, host].join('://')}
              text='send to MS3000'
            />
          </div>

          <div className={classes['settings']}>
            <ul>
              <lh>
                Editor Settings
              </lh>
              <li>
                <NumberInput
                  label='Columns:'
                  name='columns'
                  val={columns}
                  action={setColumns}
                />
              </li>
            </ul>
          </div>
        </div>
      </div>
    );
  }
}

export default connect(mapStateToProps, actions)(PixelEditorView);
