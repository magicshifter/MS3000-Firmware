import React, {PropTypes, Component} from 'react';
import {connect} from 'react-redux';
import {actions} from 'redux/modules/layout';

import Header from './Header';
import Sidebar from './Sidebar';

import PixelEditor from 'components/PixelEditor/PixelEditor';

import 'styles/core.scss';

import classes from './CoreLayout.scss';

const mapStateToProps =
  (state) => ({
    width: state.layout.toJS().sidebar.width,
  });

class CoreLayout extends Component {
  static propTypes = {
    main: PropTypes.element,
    sidebar: PropTypes.element,
    sidebarMenu: PropTypes.element,
    footer: PropTypes.element,
    width: PropTypes.number.isRequired,
    children: PropTypes.element,
    windowResize: PropTypes.func.isRequired,
  };

  constructor(props) {
    super(props);

    window.addEventListener('resize', props.windowResize);
  }

  render() {
    const {children, sidebar, footer, sidebarMenu, width} = this.props;

    return (
      <div className={classes['container']}>
        <Header />
        <div className={classes['view']}>
          <div className={classes['content']}>
            <PixelEditor />
            {children}
          </div>
          <Sidebar
            menu={sidebarMenu}
            main={sidebar}
            footer={footer}
            width={width}
          />
        </div>
      </div>
    );
  }
}

export default connect(mapStateToProps, actions)(CoreLayout);
