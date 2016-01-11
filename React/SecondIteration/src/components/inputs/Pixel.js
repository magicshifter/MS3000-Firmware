import React, {PropTypes, Component} from 'react';
import {connect} from 'react-redux';

import assign from 'object-assign';

import {colorType} from 'utils/propTypes';
import {rgba_toString} from 'utils/colors';

import {actions} from 'redux/modules/pixelEditor';

import classes from './Pixel.scss';

const mapStateToProps = (state) => {
  const {pixelClick} = state;
  return {
    pixelClick,
  };
};

export default class Pixel extends Component {
  static propTypes = {
    pixelClick: PropTypes.func.isRequired,
    pixel: PropTypes.shape({
      color: colorType.isRequired,
      row: PropTypes.number.isRequired,
      column: PropTypes.number.isRequired,
    }).isRequired,
    height: PropTypes.string.isRequired,
    width: PropTypes.string.isRequired,
    pixelId: PropTypes.number.isRequired,
  };

  render() {
    const {height, width, pixel, pixelId, pixelClick} = this.props;
    const {row, column, color} = pixel;

    return (
      <li
        className={`${classes['container']} r-${row} c-${column}`}
        style={
          assign({}, this.styles, {
            backgroundColor: rgba_toString(color),
            height,
            width,
          })
        }
        onClick={() => pixelClick({id: pixelId})}
      ></li>
    );
  }
}

export default connect(mapStateToProps, actions)(Pixel);

