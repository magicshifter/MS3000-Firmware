import React, {PropTypes, Component} from 'react';
import {connect} from 'react-redux';
import rgba from 'rgba-convert';

import {actions} from 'redux/modules/views/text';

import {actions as imageActions} from 'redux/modules/views/image';

import FontSelect from 'components/inputs/FontSelect';

import {fontType, colorType, pixelsType} from 'utils/propTypes';
import {getImagePixels} from 'utils/images';

import classes from './Text.scss';

const mapStateToProps =
  (state) => {
    const {color, pixels} = state.imageView.toJS();
    const {fonts, fontId, text} = state.textView.toJS();
    return {
      fonts,
      fontId,
      text,
      color,
      pixels,
    };
  };

export class Text extends Component {
  static propTypes = {
    setText: PropTypes.func.isRequired, // action
    setFont: PropTypes.func.isRequired, // action
    setPixels: PropTypes.func.isRequired, // action

    text: PropTypes.string,
    pixels: pixelsType.isRequired,
    fontId: PropTypes.number,
    fonts: PropTypes.arrayOf(fontType).isRequired,
    color: colorType,
  };

  constructor(props) {
    super(props);

    this.uploadText = this.uploadText.bind(this);
  }

  uploadText() {
    const {text, fontId, color, fonts, pixels, setPixels} = this.props;
    console.log({text, fontId, color}, this.props);

    const canvas = document.createElement('canvas');

    //canvas.height = 16;
    //canvas.width = 96;
    const ctx = canvas.getContext('2d');



   ctx.font = '18px Courier';
  ctx.fillStyle = '#FFFFFF';
  // todo find smallest bounding rectangle for font
  var baseLine = 12;
  var offset = 0;
    ctx.fillText("hey", offset, baseLine);




      const w = 40;
      const h = Math.min(16, 16);
      const totalColumns = 96;

      let rawData = ctx.getImageData(0, 0, w, h).data;

      for (let column = 0; column < w; column++) {
        for (let row = 0; row < h; row++) {
          if (column < w && row < h) {
            const pixelIdx = column + (row * totalColumns);
            const rawIdx = 4 * (column + row * w);

            var pixel = pixels[pixelIdx];
            // console.log('set pixel with index', pixelIdx, 'in row', row, 'and column', column);

            pixel.color = {r: rawData[rawIdx + 0], g: rawData[rawIdx + 1], b: rawData[rawIdx + 2], a: 255};

            //pixel.color = {r: 255, g: 255, b: 0, a: 255};
            // console.log(pixel.color);
          }
        }
      }

      setPixels({pixels, width:w, height:h});


    //console.log({fontStyle, colorStyle});
    //document.body.appendChild(canvas);
  }

  render() {
    const {
      text = '! MAGIC !',
      setFont, setText, // actions
    } = this.props;

    return (
      <div className={classes['container']}>
        <h3>Upload Text</h3>

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

export default connect(mapStateToProps, {...actions, ...imageActions})(Text);
