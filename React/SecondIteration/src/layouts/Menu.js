import React, {PropTypes, Component} from 'react';
import {connect} from 'react-redux';
import {Link} from 'react-router';
import rgba from 'rgba-convert';

import classes from './Menu.scss';

const mapStateToProps = (state) => {
  const {links, menuTextColor, header: {height}} = state.layout.toJS();
  return {
    height,
    links,
    color: rgba.css(menuTextColor),
  };
};

export class Menu extends Component {
  static propTypes = {
    height: PropTypes.number.isRequired,
    links: PropTypes.array.isRequired,
    color: PropTypes.string.isRequired,
  };

  render() {
    const {links, color, height} = this.props;
    const style = {
      ul: {
        height,
      },
      link: {
        color,
      },
    };

    return (
      <nav className={classes['container']}>
        <ul>
          {links.map(link => (
            <li
              key={link.key || link.text}
              style={style.link}
            >
              <Link to={link.to} activeClassName={classes['active']}>{link.text}</Link>
            </li>
          ))}
        </ul>
      </nav>
    );
  }
}

export default connect(mapStateToProps, {})(Menu);
