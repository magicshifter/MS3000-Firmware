import React, {Component, PropTypes} from 'react';

import {getImagePixels} from 'utils/images';

import classes from './ImageInput.scss';

export default class ImageInput extends Component {
  static propTypes = {
    rows: PropTypes.number,
    columns: PropTypes.number,
    label: PropTypes.string,
  };

  constructor(props) {
    super(props);

    this.onChange = this.onChange.bind(this);
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
      <div className={classes['imageInput']}>
        {label &&
          <label>{label}</label>
        }
        <input
          type='file'
          name='fileUpload'
          onChange={this.onChange}
        />
      </div>
    );
  }
}

