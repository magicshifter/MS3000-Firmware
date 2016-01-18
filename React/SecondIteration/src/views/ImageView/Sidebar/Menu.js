import React, {Component} from 'react';
import {Link} from 'react-router';
import {connect} from 'react-redux';

import {imageViewType, settingsViewType, layoutType} from 'utils/propTypes';

import classes from './Menu.scss';

const mapStateToProps = (state) => {
  const {imageView, settingsView, layout} = state;
  return {
    imageView: imageView.toJS(),
    settingsView: settingsView.toJS(),
    layout: layout.toJS(),
  };
};

export class sidebarMenu extends Component {
  static propTypes = {
    imageView: imageViewType,
    settingsView: settingsViewType,
    layout: layoutType,
  };

  constructor(props) {
    super(props);

    this.state = {
      visible: false,
    };
  }

  render() {
    const {layout} = this.props;
    const {visible} = this.state;

    const style = {
      button: {
        display: layout.width < 500
                  ? 'inherit'
                  : 'none',
      },
    };

    return (
      <div className={classes['container']}>
        <button
          className={classes['button']}
          style={style.button}
          onClick={e => this.setState({visible: !this.state.visible})}
        >
          {(visible ? '<<<' : '>>>')}
        </button>
        <nav>
          <ul>
            <li>
              <Link to='/'>controls</Link>
            </li>
            <li>
              <Link to='/paint/files'>files</Link>
            </li>
            <li>
              <Link to='/paint/settings'>settings</Link>
            </li>
          </ul>
        </nav>
      </div>
    );
  }
}

export default connect(mapStateToProps, {})(sidebarMenu);
