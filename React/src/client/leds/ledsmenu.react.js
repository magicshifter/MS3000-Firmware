import React from 'react';

import Component from '../components/component.react';
import {msg} from '../intl/store';
import * as actions from './actions';

export default class LedsMenu extends Component {

  render() {
    return (
      <div>
        <h3>{msg('pages.leds.menu.title')}</h3>
        <ul>
          <li>
            <button onClick={(e) => actions.selectAllLeds(e)}>
              {msg('pages.leds.menu.all')}
            </button>
          </li>
          <li>
            <button onClick={(e) => actions.toggleAllLeds(e)}>
              {msg('pages.leds.menu.toggle')}
            </button>
          </li>
          <li>
            <button onClick={(e) => actions.deselectAllLeds(e)}>
              {msg('pages.leds.menu.none')}
            </button>
          </li>
        </ul>
      </div>
    );
  }
}
