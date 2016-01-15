import React, {PropTypes, Component} from 'react';
import {connect} from 'react-redux';

import {actions} from 'redux/modules/views/text';

import {toInt} from 'utils/types';
import {fontType} from 'utils/propTypes';

import classes from './FontSelect.scss';

const mapStateToProps =
  (state) => {
    const {fonts, fontId} = state.textView.toJS();
    return {
      fonts,
      fontId,
    };
  };

export class FontSelect extends Component {
  static propTypes = {
    setFont: PropTypes.func.isRequired, // action

    fontId: PropTypes.number,
    fonts: PropTypes.arrayOf(fontType).isRequired,
  };

  render() {
    const {fonts, fontId, setFont} = this.props;

    return (
      <ul className={classes['container']}>
        {Object.keys(fonts).map(key => (
          <li key={fonts[key].name}>
            <input
              type='radio'
              checked={toInt(key) === fontId}
              name='font'
              value={key}
              onChange={() => setFont(key)}
            />
            <label
              style={{
                fontFamily: fonts[key].css,
              }}
            >{fonts[key].name}</label>
          </li>
        ))}
      </ul>
    );
  }
}

export default connect(mapStateToProps, actions)(FontSelect);

