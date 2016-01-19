import React, {PropTypes, Component} from 'react';
import {connect} from 'react-redux';

import {actions} from 'redux/modules/views/image';

import {pixelType} from 'utils/propTypes';

import ImageInput from 'components/inputs/ImageInput';
import FileUploadInput from 'components/inputs/FileUploadInput';
import SidebarText from './Text';

import classes from './Files.scss';

const mapStateToProps =
  ({imageView, settingsView, pixels}) => ({
    visibleColumns: imageView.get('visibleColumns'),
    totalColumns: imageView.get('totalColumns'),
    rows: imageView.get('rows'),
    pixels: pixels.toArray().map(px => px.toObject()),
    host: settingsView.get('host'),
    protocol: settingsView.get('protocol'),
  });

export class Files extends Component {
  static propTypes = {
    visibleColumns: PropTypes.number.isRequired,
    totalColumns: PropTypes.number.isRequired,
    rows: PropTypes.number.isRequired,
    pixels: PropTypes.arrayOf(pixelType).isRequired,
    host: PropTypes.string.isRequired,
    protocol: PropTypes.string.isRequired,
  };

  render() {
    const {pixels, rows, visibleColumns, totalColumns, host, protocol} = this.props;

    return (
      <div
        className={classes['container']}
      >
        <div>
          <h5>Load Image from disk</h5>
          <ImageInput />
        </div>

        <div>
          <h5>Upload</h5>
          <FileUploadInput
            pixels={pixels}
            height={rows}
            width={visibleColumns}
            totalWidth={totalColumns}
            url={[protocol, host].join('://')}
            text='send to MS3000'
          />
        </div>

        <SidebarText />
      </div>
    );
  }
}

export default connect(mapStateToProps, actions)(Files);
