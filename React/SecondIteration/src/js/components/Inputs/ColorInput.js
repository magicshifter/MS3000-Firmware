import React, {Component, PropTypes} from 'react';

import Radium from 'radium';

import {rgba_toString} from '../../utils/colors';

@Radium
export default class ColorInput extends Component {
  static propTypes = {
    value: PropTypes.number,
    name: PropTypes.string,
    min: PropTypes.number,
    max: PropTypes.number,
  }

  constructor(props) {
    super(props);

    const {value, name, label, onChange, max = 255, min = 0} = props;

    this.state = {
      value,
      name,
      max,
      min,
      label,
    };

    this.onChange = this.onChange.bind(this);
    this.onParentChange = props.onChange;
    this.inc = this.inc.bind(this);
    this.dec = this.dec.bind(this);

    this.styles = {
      li: {
        textAlign: 'right',
      },
    };
  }

  minmax(val) {
    const {max, min} = this.state;

    if (val > max) { val = max; }
    if (val < min) { val = min; }

    return val;
  }

  changeValue(val) {
    const {name} = this.state;
    const value = this.minmax(parseInt(val));

    if (typeof this.onParentChange === 'function') {
      this.onParentChange({[name]: value});
    }

    this.setState({
      value,
    });
  }

  onChange(e) {
    this.changeValue(e.target.value);
  }

  inc() {
    this.changeValue(this.state.value + 1);
  }

  dec() {
    this.changeValue(this.state.value - 1);
  }

  render() {
    const {value, name, label} = this.state;

    return (
      <li style={this.styles.li}>
        {label &&
          <label>{label}</label>
        }
        <button onClick={this.dec}>
          -
        </button>
        <input
          type='text'
          name={name}
          value={value}
          size='3'
          onChange={this.onChange}
        />
        <button onClick={this.inc}>
          +
        </button>
      </li>
    );
  }
}

