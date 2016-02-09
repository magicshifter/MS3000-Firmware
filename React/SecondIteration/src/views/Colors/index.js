import React, {PropTypes, Component} from 'react';
import {connect} from 'react-redux';
import ColorPicker from 'react-color';

import {actions} from 'redux/modules/views/image';

import {colorType, layoutType} from 'utils/propTypes';

import ColorPickerInput from 'components/inputs/ColorPickerInput';
import RGBAInput from 'components/inputs/RGBAInput';
// XXX BLOCK(time) import HexInput from 'components/inputs/HexInput';
import ColorList from 'components/inputs/ColorList';

import classes from './Color.scss';

const mapStateToProps = (state) => {
  const {imageView, layout} = state;
  const color = imageView.get('color').toObject();
  return {
    color,
    layout: layout.toJS(),
  };
};

export class Controls extends Component {
  static propTypes = {
    setColorValue: PropTypes.func.isRequired, // action
    setColor: PropTypes.func.isRequired, // action
    color: colorType.isRequired,
    layout: layoutType,
  };

  constructor(props) {
    super(props);

    this.handleColorChange = this.handleColorChange.bind(this);
  }

  handleColorChange(e) {
    const {setColor} = this.props;
    const {rgb} = e;

    setColor({color: rgb});
  }

  render() {
    const {
      setColorValue, // action
      // XXX BLOCK(time) TIME setColor, // action
      color,
      layout,
    } = this.props;

    const style = {
      rgba: {},
    };

    if (layout.width < 350) {
      style.rgba = {
        position: 'fixed',
        bottom: 0,
        left: '1em',
      };
    }

    return (
      <div
        className={classes['container']}
        style={style.rgba}
      >
        <h5>colors</h5>

        <div className={classes['picker']}>
          <ColorPicker
            color={color}
            onChange={this.handleColorChange}
            onChangeComplete={this.handleColorChange}
            custom={ColorPickerInput}
          />
        </div>

        <div className={classes['rgba']}>
          <RGBAInput
            color={color}
            setColorValue={setColorValue}
          />
        </div>

        {/* XXX BLOCK(time)
          <div className={classes['hex']}>
            <HexInput
              color={color}
              setColor={setColor}
            />
          </div>
        */}

        <div className={classes['list']}>
          <ColorList
            editorColor={color}
          />
        </div>
      </div>
    );
  }
}

export default connect(mapStateToProps, actions)(Controls);
