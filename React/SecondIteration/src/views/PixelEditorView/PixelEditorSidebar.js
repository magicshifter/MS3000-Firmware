import React, {PropTypes, Component} from 'react';
import {connect} from 'react-redux';

import {actions} from 'redux/modules/pixelEditor';

import {colorType} from 'utils/propTypes';

import RGBAInput from 'components/inputs/RGBAInput';
import NumberInput from 'components/inputs/NumberInput';
import ImageInput from 'components/inputs/ImageInput';
import FileUploadInput from 'components/inputs/FileUploadInput';

import classes from './PixelEditorSidebar.scss';

const mapStateToProps = (state) => {
  const {pixelEditor, settings, layout} = state;
  return {
    pixelEditor: pixelEditor.toJS(),
    settings: settings.toJS(),
    layout: layout.toJS(),
  };
};

export class PixelEditorSidebar extends Component {
  static propTypes = {
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
      setColorValue, setColumns, // actions
      pixelEditor, settings, layout, // state objects
    } = this.props;

    const {pixels, color, rows, columns} = pixelEditor;
    const {host, protocol} = settings;

    const containerWidth =
      layout.height > layout.width
      ? layout.width * 0.39
      : layout.height * 0.29;

    return (
      <div
        className={classes['container']}
        style={{
          width: containerWidth,
        }}
      >
        <div className={classes['picker']}>

          <h3>Controls</h3>

          <RGBAInput
            color={color}
            setColorValue={setColorValue}
          />

          <div>
            <ImageInput label='upload image' />
          </div>

          <FileUploadInput
            pixels={pixels}
            height={rows}
            width={columns}
            url={[protocol, host].join('://')}
            text='send to MS3000'
          />
        </div>

        <div className={classes['settings']}>
          <h3>Settings</h3>
          <ul>
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
    );
  }
}

export default connect(mapStateToProps, actions)(PixelEditorSidebar);
