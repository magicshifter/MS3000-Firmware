// need to import react here
import React, {Component, PropTypes} from 'react';

import Radium from 'radium';
import ColorPicker from 'react-color';

@Radium
export default class Settings extends Component {

  static propTypes = {
    settings: PropTypes.shape({
      host: PropTypes.string,
    }),
  }

  static defaultProps = {
    host: 'magicshifter.local',
  }

  constructor(props) {
    super(props);

    const {host} = props;

    this.state = {
      settings: {
        host,
      },
    };
  }

  onSubmit(e) {
    
  }

  render() {
    const {settings} = this.state;

    console.log('settings', settings);

    return (
      <section className='pixelEditor'>
        <div className='settingsContainer'>
          <ul>
            <li>
              <input type='text' name='host' />
            </li>
            <li>
              <input
                type='submit'
                onClick={this.onSubmit.bind(this)}
              />
            </li>
          </ul>
        </div>
      </section>
    );
  }
}
