import React from 'react';

import Component from '../components/component.react';
import {msg} from '../intl/store';
import * as actions from './actions';

import Led from './led.react';
import ColorPicker from 'react-color-picker';
import '../../../node_modules/react-color-picker/style/index.styl';
import ColorList from '../colorpicker/colorlist.react';
import LedsMenu from './ledsmenu.react';

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
      if (val) {
        ledsHtml.push(
          <Led
            key={key}
            ledId={key}
            value={val.get('value')}
            active={val.get('active')}
          />
        );
      }
    });

    return (
      <div style={styles.container}>
        <div
          style={styles.list}
          onMouseDown={e => actions.startSelection(e)}
          onMouseUp={e => actions.stopSelection(e)}
        >
          {ledsHtml}
        </div>
        <div style={styles.controls}>
          <LedsMenu
            leds={leds}
            value={this.props.value}
          />
          <ColorPicker
            value={this.props.value}
            onDrag={(e) => actions.updateActiveLeds(e)}
          />
        <ColorList
          colorPicker={this.props.colorPicker}
        />
        </div>
      </div>
    );
  }
}
