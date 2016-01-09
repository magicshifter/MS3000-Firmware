import React, {Component, PropTypes} from 'react';
import Radium from 'radium';

import {rgba_toString} from '../../utils/colors';

import {dispatch} from 'redux';

@Radium
export default class Pixel extends Component {

  static propTypes = {
    row: PropTypes.number,
    rows: PropTypes.number,
    column: PropTypes.number,
    columns: PropTypes.number,
    color: PropTypes.shape({
      r: PropTypes.number,
      b: PropTypes.number,
      g: PropTypes.number,
      a: PropTypes.number,
    }),
  }

  constructor(props) {
    super(props);

    // this feels dirty, but more clear than binding in the event call.
    this.onClick = this.onClick.bind(this);

    this.styles = {
      backgroundColor: rgba_toString(props.color),
      height: ((100 / props.columns) / 1.6) - .42 + 'vw',
      width: ((100 / props.columns) / 1.6) - .42 + 'vw',
      float: 'left',
      borderWidth: '.2vw',
      borderStyle: 'solid',
      borderColor: 'grey',

      ':hover': {
        borderColor: 'white',
      },
    };
  }

  onClick(evt) {
    const {rows, row, column, color} = this.props;
    const id = (row * rows) + column;

    dispatch({
      type: 'pixel/click',
      id,
      color,
    });
  }

  render() {
    const {row, column, color} = this.props;

    this.styles.backgroundColor = rgba_toString(color);

    return (
      <li
        onClick={this.onClick}
        className={`row-${row} column-${column}`}
        style={this.styles}
      ></li>
    );
  }
}
