import React, {PropTypes, Component} from 'react';
import {connect} from 'react-redux';

import {colorType, pixelType} from 'utils/propTypes';

import {rgba_toString} from 'utils/colors';

import {actions} from 'redux/modules/pixels';

import classes from './Pixel.scss';

const mapStateToProps =
  state => {
    const {imageView} = state;
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
    const {pixelClick, color} = this.props;
    const {e, pixel} = args;

    if (e.buttons === 1) {
      pixelClick({pixel, color});
    }
  }

  render() {
    const {pixel, pixelClick, color, size} = this.props;

    return (
      <td
        className={classes['container']}
        onClick={() => pixelClick({pixel, color})}
        onMouseOver={e => this.onMouseOver({e, pixel})}
        style={{
          width: size,
          height: size,
          backgroundColor: rgba_toString(pixel.color),
        }}
      ></td>
    );
  }
}

export default connect(mapStateToProps, actions)(Pixel);

