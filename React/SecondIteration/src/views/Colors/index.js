import React, {PropTypes, Component} from 'react';
import {connect} from 'react-redux';
import ColorPicker from 'react-color';
import Immutable from 'immutable';

import {actions as imageActions} from 'redux/modules/views/image';
import {actions as colorListActions} from 'redux/modules/colorList';

import {colorType, layoutType} from 'utils/propTypes';

import ColorPickerInput from 'components/inputs/ColorPickerInput';
import RGBAInput from 'components/inputs/RGBAInput';
// XXX BLOCK(time) import HexInput from 'components/inputs/HexInput';
import ColorList from 'components/inputs/ColorList';
import ColorNav from 'components/colors/ColorNav';

import classes from './Colors.scss';

const mapStateToProps =
  ({imageView, layout, colorList}) => {
    const {color} = imageView.toJS();
    const colors = colorList.toJS();

    return {
      color,
      colors,
      layout: layout.toJS(),
    };
  };

export class Colors extends Component {
  static propTypes = {
    setColorValue: PropTypes.func.isRequired, // action
    setColor: PropTypes.func.isRequired, // action
    addColor: PropTypes.func.isRequired, // action
    color: colorType.isRequired,
    layout: layoutType,

    colors: PropTypes.arrayOf(colorType).isRequired,
  };

  constructor(props) {
    super(props);

    this.handleColorChange = this.handleColorChange.bind(this);
    this.handleAddColorClick = this.handleAddColorClick.bind(this);
  };

  handleColorChange(e) {
    const {setColor} = this.props;
    const {rgb} = e;

    setColor({color: rgb});
  }

  handleAddColorClick(e) {
    const {colors, color, addColor} = this.props;

    const colorExists =
      colors.some(
        c =>
          Immutable.Map(c).equals(Immutable.Map(color))
      );

    if (!colorExists) {
      addColor([...colors, color]);
    }
  };

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
          <ColorNav
            handleAddColorClick={this.handleAddColorClick}
          />

          <ColorList />
        </div>
      </div>
    );
  }
}

const actions = {
  ...colorListActions,
  ...imageActions,
};

export default connect(mapStateToProps, actions)(Colors);
