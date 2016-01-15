import React, {PropTypes, Component} from 'react';
import {connect} from 'react-redux';
import rgba from 'rgba-convert';

import {actions} from 'redux/modules/views/text';

import FontSelect from 'components/inputs/FontSelect';

import {fontType, colorType} from 'utils/propTypes';

import classes from './Text.scss';

const mapStateToProps =
  (state) => {
    const {color} = state.imageView.toJS();
    const {fonts, fontId, text} = state.textView.toJS();
    return {
      fonts,
      fontId,
      text,
      color,
    };
  };

export class Text extends Component {
  static propTypes = {
    setText: PropTypes.func.isRequired, // action
    setFont: PropTypes.func.isRequired, // action

    text: PropTypes.string,
    fontId: PropTypes.number,
    fonts: PropTypes.arrayOf(fontType).isRequired,
    color: colorType,
  };

  constructor(props) {
    super(props);

    this.uploadText = this.uploadText.bind(this);
  }

  uploadText() {
    const {text, fontId, color, fonts} = this.props;
    console.log({text, fontId, color}, this.props);

    const canvas = document.createElement('canvas');

    canvas.height = 16;
    canvas.width = 96;
    const ctx = canvas.getContext('2d');

    const fontStyle = `16px ${fonts[fontId].css}`;
    const colorStyle = rgba.css(color);

    ctx.font = fontStyle;
    ctx.fillStyle = colorStyle;
    ctx.fillText(text, 1, 1);

    console.log({fontStyle, colorStyle});
    document.body.appendChild(canvas);
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

export default connect(mapStateToProps, actions)(Text);
