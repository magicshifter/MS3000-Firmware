// need to import react here
import React, {Component, PropTypes} from 'react';

import {rgba_toString} from 'utils/colors';
import {colorType, colorNames} from 'utils/propTypes';

import ColorInput from './ColorInput';

import classes from './RGBAInput.scss';

export default class RGBAInput extends Component {

  static propTypes = {
    color: colorType.isRequired,
    setColorValue: PropTypes.func.isRequired,
  };

  render() {
    const {color, setColorValue} = this.props;

    return (
      <div
        className={classes['input']}
        style={{backgroundColor: rgba_toString(color)}}
      >
        <ul>
          {color && Object.keys(color).map(key => (
            typeof color[key] === 'number' &&
            <ColorInput
              val={color[key]}
              key={key}
              name={key}
              label={colorNames[key]}
              setColorValue={setColorValue}
            />
          ))}
        </ul>
      </div>
    );
  }
}
