import React, {Component} from 'react';
import {connect} from 'react-redux';

import {actions} from 'redux/modules/views/image';

import ImageInput from 'components/inputs/ImageInput';

import classes from './Upload.scss';

const mapStateToProps =
  () => ({});

export class Images extends Component {
  static propTypes = {};

  render() {
    return (
      <div
        className={classes['container']}
      >
        <h5>upload</h5>
        <ImageInput />
      </div>
    );
  }
}

export default connect(mapStateToProps, actions)(Images);
