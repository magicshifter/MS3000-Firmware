import React, {PropTypes, Component} from 'react';
import {connect} from 'react-redux';
import {Link} from 'react-router';
import rgba from 'rgba-convert';

import Menu from './Menu';

import classes from './Header.scss';

const mapStateToProps = (state) => {
  const {header: {height}, width, menuTextColor} = state.layout.toJS();
  return {
    height,
    width,
    color: rgba.css(menuTextColor),
  };
};

export class Header extends Component {
  static propTypes = {
    height: PropTypes.number.isRequired,
    width: PropTypes.number.isRequired,
    color: PropTypes.string.isRequired,
  };

  render() {
    const {height, width, color} = this.props;
    const style = {
      container: {
        height,
      },
      header: {
        display: width > 500 ? 'inherit' : 'none',
        color: color,
      },
    };

    return (
      <header
        className={classes['main']}
      >
        <Link
          to='/'
          className={classes['container']}
        >
          <span className={classes['logo']} />
          <h3 style={style.header}>MagicShifter 3000</h3>
        </Link>

        <Menu />
      </header>
    );
  }
}

export default connect(mapStateToProps, {})(Header);
