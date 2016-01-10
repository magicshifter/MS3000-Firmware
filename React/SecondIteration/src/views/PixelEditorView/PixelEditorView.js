import React, {PropTypes, Component} from 'react';
import {connect} from 'react-redux';

import {actions} from 'redux/modules/pixelEditor';

import Pixel from './Pixel';

import RGBAInput from 'components/inputs/RGBAInput';
import ImageInput from 'components/inputs/ImageInput';
import FileUploadInput from 'components/inputs/FileUploadInput';

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
    const {pixelClick, setColorValue} = this.props;
    const {pixels, color, rows, columns} = this.props.pixelEditor;
    const {host, protocol} = this.props.settings;

    const width = window.innerWidth > window.innerHeight
                  ? '56%'
                  : '100%';

    const styles = {
      ul: {
        listStyle: 'none',
        width: width,
        display: 'inline-block',
        margin: 0,
        padding: 0,
      },
    };

    return (
      <div className='pixelEditor container'>
        <ul className='pixelList list' style={styles.ul}>
          {pixels && pixels.map(pixel => {
            const {row, column, color} = pixel;
            return (
              <Pixel
                key={`${column}-${row}`}
                row={row}
                column={column}
                columns={columns}
                color={color}
                pixelClick={pixelClick}
              />
            );
          })}
        </ul>

        <div className='colorPicker'>
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
