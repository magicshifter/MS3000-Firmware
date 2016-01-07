// need to import react here
import React, {Component, PropTypes} from 'react';
import objectAssign from 'object-assign';
import Radium from 'radium';

import {rgba_toString} from '../../utils/colors';
import ColorInput from './ColorInput';

@Radium
export default class RGBAInput extends Component {

  static propTypes = {
    color: PropTypes.shape({
      r: PropTypes.number,
      b: PropTypes.number,
      g: PropTypes.number,
      a: PropTypes.number,
    }),
  }

  static defaultProps = {
    color: {
      r: 255,
      b: 255,
      g: 255,
      a: 155,
    },
  }

  constructor(props) {
    super(props);

    const {color} = props;

    this.state = {
      color,
    };

    this.onChange = this.onChange.bind(this);
    this.onParentChange = props.onChange;

    this.styles = {
      container: {
        backgroundColor: color,
      },
      ul: {
        listStyle: 'none',
      },
    };
  }

  onChange(newColor) {
    if (typeof this.onParentChange === 'function') {
      this.onParentChange(newColor);
    }

    const color = objectAssign(this.state.color, newColor);
    this.setState({
      color,
    });
  }

  render() {
    const {color} = this.state;

    const style = {
      container: objectAssign(this.styles.container, {
        backgroundColor: rgba_toString(color),
      }),
    };

    return (
      <div
        style={style.container}
      >
        <ul style={this.styles.ul}>
          <ColorInput value={color.r} name='r' label='Red:' onChange={this.onChange} />
          <ColorInput value={color.g} name='g' label='Green:' onChange={this.onChange} />
          <ColorInput value={color.b} name='b' label='Blue:' onChange={this.onChange} />
          <ColorInput value={color.a} name='a' label='Alpha:' onChange={this.onChange} />
        </ul>
      </div>
    );
  }
}
