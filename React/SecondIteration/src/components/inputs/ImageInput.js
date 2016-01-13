import React, {Component, PropTypes} from 'react';
import {connect} from 'react-redux';

import {actions} from 'redux/modules/pixelEditor';

import {pixelEditorType} from 'utils/propTypes';
import {getImagePixels} from 'utils/images';

import classes from './ImageInput.scss';

const mapStateToProps = (state) => {
  const {pixelEditor} = state;
  return {
    pixelEditor: pixelEditor.toJS(),
  };
};

export class ImageInput extends Component {
  static propTypes = {
    setPixels: PropTypes.func.isRequired,
    pixelEditor: pixelEditorType,
    label: PropTypes.string,
  };

  constructor(props) {
    super(props);

    this.onChange = this.onChange.bind(this);
  }

  onChange(e) {
    const {pixelEditor, setPixels} = this.props;
    const {rows, visibleColumns, totalColumns} = pixelEditor;

    getImagePixels(e, totalColumns, visibleColumns, rows, pixels => {
      console.log(pixels);
      // setPixels(pixels, visibleColumns, rows);
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
