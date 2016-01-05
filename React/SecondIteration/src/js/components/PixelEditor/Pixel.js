// need to import react here
import React, {Component, PropTypes} from 'react';

import Radium from 'radium';

@Radium
export default class Pixel extends Component {

  static propTypes = {
    row: PropTypes.number,
    column: PropTypes.number,
    color: PropTypes.string,
    container: PropTypes.shape({
      height: PropTypes.number,
      width: PropTypes.number,
    }),
  }

  static defaultProps = {
    backgroundColor: '#000000',
  }

  constructor(props) {
    super(props);

    const {rows, columns, row, column, backgroundColor} = this.props;

    this.state = {
      row,
      column,
      backgroundColor,
      rows,
      columns,
    };
  }

  render() {
    const {rows, columns, row, column, backgroundColor} = this.state;

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
        className={`row-${row} column-${column}`}
        style={style}
      ></li>
    );
  }
}
