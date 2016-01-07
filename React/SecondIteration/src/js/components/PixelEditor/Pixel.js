// need to import react here
import React, {Component, PropTypes} from 'react';

import Radium from 'radium';

import {rgba_toString} from '../../utils/colors';

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

  static defaultProps = {
    color: {
      r: 0,
      b: 0,
      g: 0,
      a: 155,
    },
    backgroundColor: rgba_toString({r: 0, b: 0, g: 0, a: 0}),
  }

  constructor(props) {
    super(props);

    const {row, rows, column, columns, color} = props;

    this.state = {
      row,
      column,
      color,
      rows,
      columns,
      backgroundColor: rgba_toString(props.color)
    };

    // this feels dirty, but more clear than binding in the event call.
    this.onClick = this.onClick.bind(this);
  }

  onClick(evt) {
    this.setState({
      backgroundColor: rgba_toString(this.state.color),
    });
  }

  render() {
    const {row, column, columns, backgroundColor} = this.state;

    const style = {
      backgroundColor,
      height: ((100 / columns) / 1.6) - .42 + 'vw',
      width: ((100 / columns) / 1.6) - .42 + 'vw',
      float: 'left',
      borderWidth: '.2vw',
      borderStyle: 'solid',
      borderColor: 'grey',

      ':hover': {
        borderColor: 'white',
      },
    };

    return (
      <li
        onClick={this.onClick}
        className={`row-${row} column-${column}`}
        style={style}
      ></li>
    );
  }
}
