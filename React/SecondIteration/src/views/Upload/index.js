import React, { Component } from 'react';
import { connect } from 'react-redux';

import { actions } from 'redux/modules/views/image';

import FileUploadInput from 'components/inputs/FileUploadInput';

import classes from './Upload.scss';

const mapStateToProps =
  () => ({});

export class Upload extends Component {
  static propTypes = {};

  render() {
    return (
      <div
        className={classes['container']}
      >
        <h5>upload</h5>
        <FileUploadInput />
      </div>
    );
  }
}

export default connect(mapStateToProps, actions)(Upload);
