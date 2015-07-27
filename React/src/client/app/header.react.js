import Component from '../components/component.react';
import React from 'react';
import {FormattedHTMLMessage} from 'react-intl';
import {Link} from 'react-router';
import {msg} from '../intl/store';
import Logout from '../auth/logout.react';
import Radium from 'radium';
import MenuItem from './menuitem.react';
import styles from './app.styles';
import Logo from '../components/logo.react';

@Radium
class Header extends Component {

  static propTypes = {
    isLoggedIn: React.PropTypes.bool.isRequired,
  };

  render() {

    const {isLoggedIn} = this.props;

    let menuItems = ['leds', 'draw'];
    if (isLoggedIn) {
      menuItems.push('settings');
    } else {
      menuItems.push('login');
    }

    let i = 0;
    menuItems = menuItems.map((to) => {
      i += 1;
      return <MenuItem key={i} to={to} />;
    });

    if (isLoggedIn) {
      menuItems.push(<Logout key='logout' />);
    }

    return (
      <header className='main' style={styles.header.container}>
        <div style={styles.header.background}></div>
        <Link to='home' style={styles.header.link}>
          <Logo src="logo.png" size="10vw" />
          <h1 style={styles.header.h1}>
            <FormattedHTMLMessage message={msg('header.h1Html')} />
          </h1>
        </Link>
        <nav style={styles.header.nav}>
          <ul>
            {menuItems}
          </ul>
        </nav>
      </header>
    );
  }
}

export default Header;
