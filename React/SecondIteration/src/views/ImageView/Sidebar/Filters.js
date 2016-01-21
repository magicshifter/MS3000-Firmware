import React, {Component} from 'react';
import {connect} from 'react-redux';

import {actions} from 'redux/modules/filters.js';

import classes from './Filters.scss';

const mapStateToProps =
  () => ({});

export class Files extends Component {
  static propTypes = {};

  render() {
    return (
      <div
        className={classes['container']}
      >
        <div>
          <h5>Filters</h5>
          <div>Coming soon</div>
        </div>
      </div>
    );
  }
}

export default connect(mapStateToProps, actions)(Files);
