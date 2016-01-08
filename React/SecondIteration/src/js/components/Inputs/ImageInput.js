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

    const {label = 'image upload', rows, columns} = props;

    this.state = {
      label,
      rows,
      columns,
    };

    this.onChange = this.onChange.bind(this);

    this.styles = {
      input: {
        textAlign: 'right',
      },
    };
  }

  onChange(e) {
    const {rows, columns} = this.state;
    getImagePixels(e, columns, rows, (pixels) => {
      console.log({pixels});
    });
  }

  render() {
    const {label} = this.state;

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

