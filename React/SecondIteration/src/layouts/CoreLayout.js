import React, {PropTypes, Component} from 'react';
import {connect} from 'react-redux';

import {actions} from 'redux/modules/layout';

import Header from './Header';
import Sidebar from './Sidebar';
import SecondaryMenu from './SecondaryMenu';

import PixelEditor from 'components/PixelEditor/PixelEditor';

import {colorType} from 'utils/propTypes';

import 'styles/core.scss';
import classes from './CoreLayout.scss';

const mapStateToProps =
  ({imageView}) => {
    const {color} = imageView.toJS();
    return {
      color,
    };
  };

class CoreLayout extends Component {
  static propTypes = {
    children: PropTypes.element,
    windowResize: PropTypes.func.isRequired,
    scrollEvent: PropTypes.func.isRequired,
    color: colorType.isRequired,
  };

  constructor(props) {
    super(props);

    const {windowResize, scrollEvent} = props;

    window.addEventListener('resize', windowResize);
    window.addEventListener('mousewheel', scrollEvent);
  }

  render() {
    const {children, color} = this.props;

    return (
      <div className={classes['container']}>
        <Header />

        <div className={classes['view']}>
          <PixelEditor />

          <SecondaryMenu color={color} />

          {children && <Sidebar children={children} />}
        </div>
      </div>
    );
  }
}

export default connect(mapStateToProps, actions)(CoreLayout);
