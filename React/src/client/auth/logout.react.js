import * as actions from './actions';
import Component from '../components/component.react';
import React from 'react';
import {msg} from '../intl/store';
import Logo from '../components/logo.react';

export default class Logout extends Component {

  render() {
    const styles = {
      a: {
        cursor: 'pointer',
      },
    };

    return (
      <li>

        <a
          style={styles.a}
          onClick={actions.logout}
        ><Logo
          src='logout'
          size='10vw'
          shadow='5px 5px 0px #78909C'>
        </Logo></a>
      </li>
    );
  }

}
