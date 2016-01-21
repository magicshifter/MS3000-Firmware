import React, {PropTypes, Component} from 'react';
import {connect} from 'react-redux';

import {pixelType} from 'utils/propTypes';

import FileUploadInput from 'components/inputs/FileUploadInput';

import classes from './Footer.scss';

const mapStateToProps =
  ({imageView, settingsView, pixels}) => ({
    visibleColumns: imageView.get('visibleColumns'),
    totalColumns: imageView.get('totalColumns'),
    rows: imageView.get('rows'),
    pixels: pixels.toArray().map(px => px.toJS()),
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
        <FileUploadInput
          pixels={pixels}
          height={rows}
          width={visibleColumns}
          totalWidth={totalColumns}
          url={[protocol, host].join('://')}
          header='Upload:'
          text='send to MS3000'
        />
      </div>
    );
  }
}

export default connect(mapStateToProps, {})(Files);
