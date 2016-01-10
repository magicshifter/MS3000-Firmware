import React, {PropTypes, Component} from 'react';
import {connect} from 'react-redux';

import {actions} from 'redux/modules/pixelEditor';

import Pixel from 'components/inputs/Pixel';

import RGBAInput from 'components/inputs/RGBAInput';
import ImageInput from 'components/inputs/ImageInput';
import FileUploadInput from 'components/inputs/FileUploadInput';

import classes from './PixelEditorView.scss';

const mapStateToProps = (state) => {
  const {pixelEditor, settings, layout} = state;
  return {
    pixelEditor,
    settings,
    layout,
  };
};

export class PixelEditorView extends Component {
  static propTypes = {
    pixelClick: PropTypes.func.isRequired,
    setColor: PropTypes.func.isRequired,
    setColorValue: PropTypes.func.isRequired,

    pixelEditor: PropTypes.shape({
      pixels: PropTypes.array.isRequired,
      rows: PropTypes.number.isRequired,
      columns: PropTypes.number.isRequired,

      color: PropTypes.shape({
        r: PropTypes.number,
        g: PropTypes.number,
        b: PropTypes.number,
        a: PropTypes.number,
      }).isRequired,
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
    const {pixelClick, setColorValue, pixelEditor, settings} = this.props;
    const {pixels, color, rows, columns} = pixelEditor;
    const {host, protocol} = settings;

    return (
      <div className={classes['container']}>
        <ul className={classes['list']}>
          {pixels && pixels.map(p => (
            <Pixel
              key={`${p.column}-${p.row}`}
              pixel={p}
              columns={columns}
              pixelClick={pixelClick}
            />
          ))}
        </ul>

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
      </div>
    );
  }
}

export default connect(mapStateToProps, actions)(PixelEditorView);
