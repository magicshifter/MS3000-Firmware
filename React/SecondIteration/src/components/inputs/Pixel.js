import React, {PropTypes, Component} from 'react';
import assign from 'object-assign';

import {colorType} from 'utils/propTypes';
import {rgba_toString} from 'utils/colors';

import classes from './Pixel.scss';

export default class Pixel extends Component {
  static propTypes = {
    pixel: PropTypes.shape({
      color: colorType,
      row: PropTypes.number.isRequired,
      column: PropTypes.number.isRequired,
    }).isRequired,
    columns: PropTypes.number.isRequired,
    pixelClick: PropTypes.func.isRequired,
  };

  constructor(props) {
    super(props);

    const {columns} = props;

    const widthValue = window.innerWidth > window.innerHeight
                      ? ((100 / columns) / 1.6) - 0.42 + 'vw'
                      : (100 / columns) - 0.2 + 'vw';

    this.styles = {
      height: widthValue,
      width: widthValue,
    };

    this.onClick = this.onClick.bind(this);
  }

  onClick(e) {
    const {columns, pixel} = this.props;
    const {pixelClick, row, column} = pixel;
    const id = (((row - 1) * columns) - 1) + column;

    pixelClick({id});
  }

  render() {
    const {row, column, color} = this.props.pixel;

    return (
      <li
        className={`${classes['container']} r-${row} c-${column}`}
        onClick={this.onClick}
        style={assign({}, this.styles, {backgroundColor: rgba_toString(color)})}
      ></li>
    );
  }
}
