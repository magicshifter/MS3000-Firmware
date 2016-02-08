import React, {PropTypes, Component} from 'react';
import {connect} from 'react-redux';

import {actions} from 'redux/modules/views/settings';
import {settingsViewType} from 'utils/propTypes';

import classes from './SettingsView.scss';

const mapStateToProps =
  state => ({
    settings: state.settingsView.toJS(),
  });

export class SettingsView extends Component {
  static propTypes = {
    settings: settingsViewType,
    setSettings: PropTypes.func.isRequired,
  };

  constructor(props) {
    super(props);

    this.onInputChange = this.onInputChange.bind(this);

    this.state = props.settings || {};
  }

  onInputChange(e) {
    const {name, value} = e.target;
    this.setState({[name]: value});
  }

  render() {
    const {setSettings} = this.props;
    const {protocol, host, syslogIp} = this.state;

    return (
      <div className={[classes['container'], 'container'].join(' ')}>
        <h2>Settings:</h2>

        <div className={classes['form']}>
          <ul>
            <li style={{display: 'none'}} key='protocol'>
              <label>protocol:</label>
              <input
                type='text'
                name='protocol'
                onChange={this.onInputChange}
                value={protocol}
              />
            </li>

            <li key='host'>
              <label>hostname:</label>
              <input
                type='text'
                name='host'
                onChange={this.onInputChange}
                value={host}
              />
            </li>

            <li key='syslogIp'>
              <label>syslog ip:</label>
              <input
                type='text'
                name='syslogIp'
                onChange={this.onInputChange}
                value={syslogIp}
              />
            </li>

            <li
              key='submit'
            >
              <input
                type='submit'
                value='Save Settings'
                onClick={() => setSettings(this.state)}
              />
            </li>
          </ul>
        </div>
      </div>
    );
  }
}

export default connect(mapStateToProps, actions)(SettingsView);
