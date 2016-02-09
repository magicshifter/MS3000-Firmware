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
  (state) => ({
    width: state.layout.toJS().sidebar.width,
  });

class CoreLayout extends Component {
  static propTypes = {
    sidebar: PropTypes.element,
    width: PropTypes.number.isRequired,
    children: PropTypes.element,
    windowResize: PropTypes.func.isRequired,
    scrollEvent: PropTypes.func.isRequired,
  };

  constructor(props) {
    super(props);

    window.addEventListener('resize', props.windowResize);
    window.addEventListener('mousewheel', props.scrollEvent);
  }

  render() {
    const {children, sidebar, width} = this.props;

    return (
      <div className={classes['container']}>
        <Header />
        <div className={classes['view']}>
          <div className={classes['content']}>
            <PixelEditor />
          </div>
          <SecondaryMenu />
          { sidebar &&
            <Sidebar
              main={sidebar}
              width={width}
            />
          }
          <aside className={classes['sidebar']}>
            {children}
          </aside>
        </div>
      </div>
    );
  }
}

export default connect(mapStateToProps, actions)(CoreLayout);
