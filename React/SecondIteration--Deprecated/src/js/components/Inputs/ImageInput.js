import React, {Component, PropTypes} from 'react';
import Radium from 'radium';

import {getImagePixels} from '../../utils/images';

@Radium
export default class ColorInput extends Component {
  static propTypes = {
    rows: PropTypes.number,
    columns: PropTypes.number,
  }

  constructor(props) {
    super(props);

    this.onChange = this.onChange.bind(this);

    this.styles = {
      input: {
        textAlign: 'right',
      },
    };
  }

  onChange(e) {
    const {rows, columns} = this.props;
    getImagePixels(e, columns, rows, (pixels) => {
      console.log({pixels});
    });
  }

  render() {
    const {label} = this.props;

    return (
      <li style={this.styles.li}>
        {label &&
          <label>{label}</label>
        }
        <input
          type='file'
          name='fileUpload'
          onChange={this.onChange}
        />
      </li>
    );
  }
}

