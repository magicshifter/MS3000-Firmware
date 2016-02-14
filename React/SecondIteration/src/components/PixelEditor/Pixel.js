import React, {PropTypes, Component} from 'react';
import {connect} from 'react-redux';
import rgba from 'rgba-convert';

import {colorType, pixelType} from 'utils/propTypes';

import {actions} from 'redux/modules/pixels';

import classes from './Pixel.scss';

const mapStateToProps =
  ({imageView}) => {
    return {
      color: imageView.get('color'),
      rows: imageView.get('rows'),
    };
  };

export class Pixel extends Component {
  static propTypes = {
    pixel: pixelType.isRequired,
    color: colorType.isRequired,
    size: PropTypes.number.isRequired,
    pixelClick: PropTypes.func.isRequired,
    pixelHover: PropTypes.func.isRequired,
  };

  constructor(props) {
    super(props);

    this.onMouseOver = this.onMouseOver.bind(this);

    this.defaultStyle = {
      table: {},
      tr: {},
    };
  }

  onMouseOver(args) {
    const {pixelHover, color} = this.props;
    const {e, pixel} = args;

    if (e.buttons === 1) {
      pixelHover({pixel, color});
    }
  }

  render() {
    const {pixel, pixelClick, color, size} = this.props;

    const style = {
      backgroundColor: rgba.css(pixel.color),
      width: size,
      height: size,
    };

    return (
      <td
        className={classes['container']}
        onMouseDown={() => pixelClick({pixel, color})}
        onMouseOver={e => this.onMouseOver({e, pixel})}
        style={style}
      ></td>
    );
  }
}

export default connect(mapStateToProps, actions)(Pixel);

