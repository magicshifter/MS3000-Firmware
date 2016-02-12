import React, {Component, PropTypes} from 'react';
import {connect} from 'react-redux';

import {actions} from 'redux/modules/pixels';
import {actions as imageActions} from 'redux/modules/views/image';

import {getIconCssClass} from 'utils/icons';

import {pixelsType} from 'utils/propTypes';
import {getImagePixels} from 'utils/images';
import {makePixelsArray} from 'utils/pixels';

import classes from './ImageInput.scss';

const mapStateToProps = (state) => {
  const {imageView, pixels} = state;
  return {
    pixels: makePixelsArray(pixels),
    visibleColumns: imageView.get('visibleColumns'),
    totalColumns: imageView.get('totalColumns'),
    rows: imageView.get('rows'),
  };
};

export class ImageInput extends Component {
  static propTypes = {
    setColumns: PropTypes.func.isRequired,
    setPixels: PropTypes.func.isRequired,

    pixels: pixelsType.isRequired,
    rows: PropTypes.number.isRequired,
    visibleColumns: PropTypes.number.isRequired,
    totalColumns: PropTypes.number.isRequired,
    label: PropTypes.string,
  };

  constructor(props) {
    super(props);

    this.state = {
      pixels: [],
    };

    this.onUseImageClick = this.onUseImageClick.bind(this);
    this.onClearCanvasClick = this.onClearCanvasClick.bind(this);
    this.onImageInputchange = this.onImageInputchange.bind(this);
  }

  onClearCanvasClick(e) {
    const {canvas} = this.refs;

    const ctx = canvas.getContext('2d');
    ctx.clearRect(0, 0, canvas.width, canvas.height);
    this.setState({
      pixels: [],
    });
  }

  onUseImageClick() {
    const {
      setPixels, setColumns, // actions
    } = this.props;

    const {
      pixels, columns,
    } = this.state;

    if (pixels.length) {
      setPixels(pixels);
      setColumns({value: columns});
    }
  }

  onImageInputchange(e) {
    const {pixels, rows, visibleColumns, totalColumns} = this.props;
    const {canvas} = this.refs;

    const {files} = e.target;
    const file = files[0];

    getImagePixels(file, canvas, pixels, totalColumns, visibleColumns, rows, ({data, pixels, width, height}) => {
      this.setState({
        imageUrl: data,
        pixels,
        columns: width,
      });
      this.onUseImageClick();
    });
  }

  render() {
    const {label} = this.props;
    const {pixels} = this.state;

    const fontSize = pixels.length ? '50px' : '200px';

    return (
      <div className={classes['container']}>
        {label && <label>{label}</label>}

        <div className={classes['input_container']}>
          <input
            type='file'
            name='fileUpload'
            onChange={this.onImageInputchange}
            //style={{fontSize}}
          />

          <i
            className={getIconCssClass('images')}
            //style={{fontSize}}
          />
        </div>

        <div
          className={classes['sub_container']}
          style={{
            // XXX block(time) hide for now
            display: false && pixels.length ? 'inherit' : 'none',
          }}
        >
          <canvas
            ref='canvas'
          />

          <button
            onClick={this.onUseImageClick}
          >
            Use Image
          </button>
        </div>
      </div>
    );
  }
}

export default connect(mapStateToProps, {...actions, ...imageActions})(ImageInput);
