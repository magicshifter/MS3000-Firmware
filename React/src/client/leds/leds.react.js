import React from 'react';

import Component from '../components/component.react';
import {msg} from '../intl/store';

import Led from './led.react';
import ColorPicker from '../colorpicker/colorpicker.react';
import ColorList from '../colorpicker/colorlist.react';

export default class Leds extends Component {

  render() {
    const styles = {
      container: {
        width: '100%',
        display: 'inline-block',
      },
      list: {
        display: 'inline-block',
      },
      controls: {
        display: 'inline-block',
        backgroundColor: '#fff',
        borderRadius: '5px',
        border: '2px solid grey',
      },
    };

    const { leds } = this.props;
    const list = leds.get('list');
    let ledsHtml = [];
    list.map((val, key) => {
      ledsHtml.push(
        <Led
          key={key}
          id={key}
          value={val.get('value')}
          active={val.get('active')}
          {...this.props}
        />
      );
    });

    return (
      <div style={styles.container}>
        <div style={styles.list}>
          {ledsHtml}
        </div>
        <div style={styles.controls}>
          <ColorPicker {...this.props} />
          <ColorList {...this.props} />
        </div>
      </div>
    );
  }
}
