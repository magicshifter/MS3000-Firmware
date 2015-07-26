import Component from '../components/component.react';
import React from 'react';
import {FormattedHTMLMessage} from 'react-intl';
import {Link} from 'react-router';
import {msg} from '../intl/store';
import Logout from '../auth/logout.react';

class Header extends Component {

  static propTypes = {
    isLoggedIn: React.PropTypes.bool.isRequired,
  };

  render() {
    const styles = {
      header: {
        h1: {
          marginBottom: 0,
          float: 'left',
        },
        nav: {
          float: 'right',
        },
        li: {
          fontSize: '1em',
          float: 'left',
          display: 'inline-block',
          margin: '0 0 0 .5em',
        },
      },
    };

    const {isLoggedIn} = this.props;

    let menuItems = [
      <Link key='home' to='home'>{msg('menu.home')}</Link>,
      <Link key='leds' to='leds'>{msg('menu.leds')}</Link>,
    ];
    if (isLoggedIn) {
      menuItems.push(<Link
                        key='settings'
                        to='settings'>
                        {msg('menu.settings')}
                    </Link>);
      menuItems.push(<Logout key='logout' />);
    } else {
      menuItems.push(<Link key='login' to='login'>{msg('menu.login')}</Link>);
    }

    let i = 0;
    menuItems = menuItems.map((item) => {
      i += 1;
      return <li key={i} style={styles.li}>{item}</li>;
    });

    return (
      <header className='main'>
        <h1 style={styles.header.h1}>
          <FormattedHTMLMessage message={msg('header.h1Html')} />
        </h1>
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
