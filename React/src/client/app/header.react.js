import Component from '../components/component.react';
import React from 'react';
import {FormattedHTMLMessage} from 'react-intl';
import {Link} from 'react-router';
import {msg} from '../intl/store';
import Logout from '../auth/logout.react';
import styles from './app.styles';
import Logo from '../components/logo.react';
import Nav from '../components/nav.react';
import StatusBar from '../components/statusbar.react';

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

    if (isLoggedIn) {
      menuItems.push(<Logout key='logout' />);
    }

    return (
      <header className='main' style={styles.header.container}>
        <div style={styles.header.background}></div>
        <Link to='home' style={styles.header.link}>
          <Logo src='logo.png' size='10vw' float='left' />
          <h1 style={styles.header.h1}>
            <FormattedHTMLMessage message={msg('header.h1Html')} />
          </h1>
        </Link>
        <Nav menuItems={menuItems} />
        <StatusBar {...this.props} />
      </header>
    );
  }
}

export default Header;
