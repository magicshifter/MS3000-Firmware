import React, {PropTypes, Component} from 'react';
import {connect} from 'react-redux';

import {actions} from 'redux/modules/layout';

import Header from './Header';
import Sidebar from './Sidebar';
import SecondaryMenu from './SecondaryMenu';

import PixelEditor from 'components/PixelEditor/PixelEditor';

import 'styles/core.scss';

import classes from './CoreLayout.scss';

const mapStateToProps =
  (state) => ({});

class CoreLayout extends Component {
  static propTypes = {
    children: PropTypes.element,
    windowResize: PropTypes.func.isRequired,
    scrollEvent: PropTypes.func.isRequired,
  };

  constructor(props) {
    super(props);

    const {windowResize, scrollEvent} = props;

    window.addEventListener('resize', windowResize);
    window.addEventListener('mousewheel', scrollEvent);

    document.body.addEventListener(
      'click',
      e =>
        console.log({e, loc: window.location})
    );
  }

  render() {
    const {children} = this.props;

    return (
      <div className={classes['container']}>
        <Header />

        <div className={classes['view']}>
          <PixelEditor />

          <SecondaryMenu />

          {children && <Sidebar children={children} />}
        </div>
      </div>
    );
  }
}

export default connect(mapStateToProps, actions)(CoreLayout);
