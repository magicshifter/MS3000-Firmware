import React, {PropTypes, Component} from 'react';
import {connect} from 'react-redux';
import {actions} from 'redux/modules/layout';

import Header from './Header';
import Sidebar from './Sidebar';

import 'styles/core.scss';

import classes from './CoreLayout.scss';

class CoreLayout extends Component {
  static propTypes = {
    main: PropTypes.element,
    sidebar: PropTypes.element,
    sidebarMenu: PropTypes.element,
    children: PropTypes.element,
    windowResize: PropTypes.func.isRequired,
  };

  constructor(props) {
    super(props);

    window.addEventListener('resize', props.windowResize);
  }

  render() {
    const {children, main, sidebar, sidebarMenu} = this.props;

    return (
      <div className={classes['container']}>
        <Header />
        <div className={classes['view']}>
          <div className={classes['content']}>
            {main || children}
          </div>
          <Sidebar
            menu={sidebarMenu}
            main={sidebar}
          />
        </div>
      </div>
    );
  }
}

export default connect(() => ({}), actions)(CoreLayout);
