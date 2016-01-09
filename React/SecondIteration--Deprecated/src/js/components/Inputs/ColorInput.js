import React, {Component, PropTypes} from 'react';

import Radium from 'radium';

import {rgba_toString} from '../../utils/colors';

import {dispatch} from '../../Store';

import {SET_COLOR_VALUE} from '../../actions/pixelEditor';


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

    console.log("colorinput props", props);

    this.onChange = this.onChange.bind(this);
    this.inc = this.inc.bind(this);
    this.dec = this.dec.bind(this);

    this.styles = {
      li: {
        textAlign: 'right',
      },
    };
  }

  onChange(e) {
    const {name} = this.props;

    dispatch({
      type: SET_COLOR_VALUE,
      value: e.target.value,
      name,
    });
  }

  inc() {
    let {name, value} = this.props;
    value = value + 1;

    dispatch({
      type: SET_COLOR_VALUE,
      value,
      name,
    });
  }

  dec() {
    let {name, value} = this.props;
    value = value - 1;

    dispatch({
      type: SET_COLOR_VALUE,
      value,
      name,
    });
  }

  render() {
    const {value, name, label} = this.props;

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

