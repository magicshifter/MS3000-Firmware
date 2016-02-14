import React, { PropTypes, Component } from 'react';
import { connect } from 'react-redux';
import rgba from 'rgba-convert';

import { actions } from 'redux/modules/views/text';
import { actions as pixelActions } from 'redux/modules/pixels';
import { actions as imageActions } from 'redux/modules/views/image';

import FontSelect from 'components/inputs/FontSelect';

import { fontType, pixelsType, colorType } from 'utils/propTypes';
import { makePixelsArray } from 'utils/pixels';

import classes from './Font.scss';

const mapStateToProps =
  (state) => {
    const { totalColumns, rows, color } = state.imageView.toJS();
    const pixels = makePixelsArray(state.pixels);
    const { fonts, fontId, text } = state.textView.toJS();
    return {
      fonts,
      fontId,
      text,
      totalColumns,
      rows,
      pixels,
      color,
    };
  };

export class Font extends Component {
  static propTypes = {
    setText: PropTypes.func.isRequired, // action
    setFont: PropTypes.func.isRequired, // action
    setColumns: PropTypes.func.isRequired, // action
    setPixels: PropTypes.func.isRequired, // action

    text: PropTypes.string,
    pixels: pixelsType.isRequired,
    fontId: PropTypes.number,
    totalColumns: PropTypes.number,
    rows: PropTypes.number,
    color: colorType.isRequired,
    fonts: PropTypes.arrayOf(fontType).isRequired,
  };

  constructor(props) {
    super(props);

    this.uploadText = this.uploadText.bind(this);
  }

  uploadText() {
    const {
      text, fontId, fonts, // text state
      pixels, // pixel state
      totalColumns, rows, color, // ImageView state
      setPixels, setColumns, // actions
    } = this.props;

    const canvas = document.createElement('canvas');

    const ctx = canvas.getContext('2d');

    const font = fonts[fontId];
    ctx.font = '18px ' + (font && font.css || 'Courier');

    ctx.fillStyle = rgba.css(color) || '#FFFFFF';

    var baseLine = 14;
    var offset = 0;
    ctx.fillText(text, offset, baseLine);
    var textMeasurements = ctx.measureText(text); // TextMetrics object

    const h = Math.min(rows, 16);
    const w = Math.min(Math.round(textMeasurements.width), totalColumns);
    console.log(w);

    let rawData = ctx.getImageData(0, 0, w, h).data;

    for (let column = 0; column < w; column++) {
      for (let row = 0; row < h; row++) {
        if (column < w && row < h) {
          const pixelIdx = column + (row * totalColumns);
          const rawIdx = 4 * (column + row * w);

          var pixel = pixels[pixelIdx];
          pixel.color = { r: rawData[rawIdx + 0], g: rawData[rawIdx + 1], b: rawData[rawIdx + 2], a: 255 };
        }
      }
    }

    setPixels(pixels);
    setColumns({ value: w });
  }

  render() {
    const {
      text = '! MAGIC !',
      setFont, setText, // actions
    } = this.props;

    return (
      <div className={classes['container']}>
        <h5>fonts</h5>

        <FontSelect
          setFont={setFont}
        />

        <div
          className={classes['valueInput']}
          key='valueInput'
        >
          <input
            type='text'
            value={text}
            onChange={e => setText(e.target.value)}
          />
        </div>
        <div>
          <input
            type='button'
            onClick={this.uploadText}
            value='Use Text'
          />
        </div>
      </div>
    );
  }
}

export default connect(mapStateToProps, { ...actions, ...imageActions, ...pixelActions })(Font);
