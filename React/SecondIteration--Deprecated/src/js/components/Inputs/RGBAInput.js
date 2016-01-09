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
    onChange: PropTypes.func,
  }

  static defaultProps = {
    color: {
      r: 0,
      b: 0,
      g: 0,
      a: 155,
    },
    onChange: () => {},
  }

  constructor(props) {
    super(props);

    const {color} = props;

    this.styles = {
      container: {
        backgroundColor: color,
      },
      ul: {
        listStyle: 'none',
      },
    };
  }

  render() {
    const {color} = this.props;

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
          <ColorInput value={color.r} name='r' label='Red:' />
          <ColorInput value={color.g} name='g' label='Green:' />
          <ColorInput value={color.b} name='b' label='Blue:'  />
          <ColorInput value={color.a} name='a' label='Alpha:' />
        </ul>
      </div>
    );
  }
}
