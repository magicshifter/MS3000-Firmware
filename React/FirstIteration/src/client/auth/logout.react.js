import * as actions from './actions';
import Component from '../components/component.react';
import React from 'react';
import {msg} from '../intl/store';
import Logo from '../components/logo.react';

export default class Logout extends Component {

  render() {
    const styles = {
      container: {
        float: 'left',
        margin: '0 0 0 .5em',
      },
      a: {
        cursor: 'pointer',
      },
      text: {
        width: '100%',
        textAlign: 'center',
        margin: '-.3em 0 0',
        fontSize: '1.2em',
      },
    };

    return (
      <li style={styles.container}>
        <a
          style={styles.a}
          onClick={actions.logout}
        >
          <Logo
            src='logout.svg'
            size='10vw'
            shadow='5px 5px 0px #78909C'>
          </Logo>
          <div style={styles.text}>
            {msg('menu.logout')}
          </div>
        </a>
      </li>
    );
  }

}
