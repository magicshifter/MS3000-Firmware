import React, {PropTypes} from 'react';

import Component from '../components/component.react';
import {msg} from '../intl/store';
import * as actions from './actions';

import Led from './led.react';
import ColorPicker from 'react-color-picker';
import '../../../node_modules/react-color-picker/style/index.styl';
import ColorList from '../colorlist/colorlist.react';
import LedsMenu from './ledsmenu.react';
import BrightnessControl from './brightnesscontrol.react.js';

import merge from 'magic-merge';
import globalStyles from '../styles/global';

export default class Leds extends Component {

  static propTypes = {
    activeColor: PropTypes.string,
    brightness: PropTypes.number,
  }

  static defaultProps = {
    brightness: 16,
  }

  render() {
    const styles = {
      container: {
        width: '100%',
        display: 'inline-block',
      },
      list: {
        display: 'inline-block',
      },
      colorPicker: {
        backgroundColor: '#fff',
        borderRadius: '5px',
        border: '2px solid grey',
        float: 'left',
      },
      colorList: {
        float: 'left',
      },
      controls: {
        clear: 'both',
        display: 'inline-block',
        margin: '0 auto',
      },
      ledControls: {
        float: 'left',
      },
    };

    const { leds, colorList } = this.props;
    const list = leds.get('list');
    const brightness = leds.get('brightness');
    const activeColor = leds.get('activeColor');

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
        >
          {ledsHtml}
        </div>

        <div style={styles.controls}>

          <div style={styles.ledControls}>

            <LedsMenu
              style={globalStyles.float('left')}
              leds={leds}
            />

            <BrightnessControl brightness={brightness} />

            <ColorList
              style={globalStyles.float('left')}
              activeColor={leds.get('activeColor')}
              colorList={colorList}
            />
          </div>

          <ColorPicker
            style={globalStyles.float('left')}
            defaultValue={activeColor}
            onDrag={(e) => actions.changeActiveColor(e)}
          />
        </div>
      </div>
    );
  }
}
