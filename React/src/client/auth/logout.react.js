import './logout.styl';
import * as actions from './actions';
import Component from '../components/component.react';
import React from 'react';
import {msg} from '../intl/store';

export default class Logout extends Component {

  render() {
    return (
      <a className='logout'
        children={msg('menu.logout')}
        onClick={actions.logout}
      ></a>
    );
  }

}
