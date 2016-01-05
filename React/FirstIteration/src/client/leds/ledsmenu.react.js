import React from 'react';

import Component from '../components/component.react';
import {msg} from '../intl/store';
import * as actions from './actions';

export default class LedsMenu extends Component {

  render() {
    const styles = {
      container: {

      },
      lh: {
        float: 'left',
        margin: '0 5px 0',
      },
      li: {
        float: 'left',
        margin: '0 5px 0 0',

      },
    };

    return (
      <div>
        <ul>
          <lh style={styles.lh}>{msg('pages.leds.menu.title')}</lh>
          <li style={styles.li}>
            <button onClick={(e) => actions.selectAllLeds(e)}>
              {msg('pages.leds.menu.all')}
            </button>
          </li>
          <li style={styles.li}>
            <button onClick={(e) => actions.toggleAllLeds(e)}>
              {msg('pages.leds.menu.toggle')}
            </button>
          </li>
          <li style={styles.li}>
            <button onClick={(e) => actions.deselectAllLeds(e)}>
              {msg('pages.leds.menu.none')}
            </button>
          </li>
        </ul>
      </div>
    );
  }
}
