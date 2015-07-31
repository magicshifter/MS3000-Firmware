import Component from '../components/component.react';
import React from 'react';
import {FormattedHTMLMessage} from 'react-intl';
import {Link} from 'react-router';
import {msg} from '../intl/store';
import Logout from '../auth/logout.react';
import Logo from '../components/logo.react';
import Nav from '../components/nav.react';
import StatusBar from '../components/statusbar.react';

import styles from './app.styles';

export default class Header extends Component {

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

    let homeLinksTo = 'login';
    if (isLoggedIn) {
      menuItems.push(<Logout key='logout' />);
      homeLinksTo = 'settings';
    }

    return (
      <header className='main' style={styles.header.wrapper}>
        <div style={styles.header.background}></div>
        <div style={styles.header.container}>
          <Link to={homeLinksTo} style={styles.header.link}>
            <Logo
              src='logo.png'
              size='10vw'
              float='left'
            />
            <h1 style={styles.header.h1}>
              <FormattedHTMLMessage message={msg('header.h1Html')} />
            </h1>
          </Link>
          <Nav menuItems={menuItems} />
        </div>
        <StatusBar {...this.props} />
      </header>
    );
  }
}
