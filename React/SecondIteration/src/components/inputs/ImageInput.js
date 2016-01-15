import React, {Component, PropTypes} from 'react';
import {connect} from 'react-redux';

import {actions} from 'redux/modules/views/image';

import {pixelsType} from 'utils/propTypes';
import {getImagePixels} from 'utils/images';

import classes from './ImageInput.scss';

const mapStateToProps = (state) => {
  const {imageView} = state;
  return {
    pixels: imageView.get('pixels').toJS(),
    visibleColumns: imageView.get('visibleColumns'),
    totalColumns: imageView.get('totalColumns'),
    rows: imageView.get('rows'),
  };
};

export class ImageInput extends Component {
  static propTypes = {
    setPixels: PropTypes.func.isRequired,
    pixels: pixelsType.isRequired,
    rows: PropTypes.number.isRequired,
    visibleColumns: PropTypes.number.isRequired,
    totalColumns: PropTypes.number.isRequired,
    label: PropTypes.string,
  };

  constructor(props) {
    super(props);

    this.onChange = this.onChange.bind(this);
  }

  onChange(e) {
    const {setPixels, pixels, rows, visibleColumns, totalColumns} = this.props;

    getImagePixels(e, pixels, totalColumns, visibleColumns, rows, (pixels) => {
      setPixels(pixels, visibleColumns, rows);
    });
  }

  render() {
    const {label} = this.props;

    return (
      <div className={classes['input']}>

        {
          label &&
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

export default connect(mapStateToProps, actions)(ImageInput);
