import React, { PropTypes } from 'react';

import classes from './UiSettings.scss';

export const UiSettings =
  ({protocol, host, syslogIp, onInputChange, onFormSubmit}) =>
    <form
      className={classes['container']}
      onSubmit={onFormSubmit}
    >
      <fieldset>
        <legend>Webinterface:</legend>

        <ul>
          <li style={{display: 'none'}} key='protocol'>
            <label>protocol:</label>
            <input
              type='text'
              name='protocol'
              onChange={onInputChange}
              value={protocol}
            />
          </li>

          <li key='host'>
            <label>hostname:</label>
            <input
              type='text'
              name='host'
              onChange={onInputChange}
              value={host}
            />
          </li>

          <li key='syslogIp'>
            <label>syslog ip:</label>
            <input
              type='text'
              name='syslogIp'
              onChange={onInputChange}
              value={syslogIp}
            />
          </li>

          <li
            key='submit'
          >
            <input
              type='submit'
              value='save'
            />
          </li>
        </ul>
      </fieldset>
    </form>;

UiSettings.propTypes = {
  protocol: PropTypes.string.isRequired,
  host: PropTypes.string.isRequired,
  syslogIp: PropTypes.string.isRequired,
  onInputChange: PropTypes.func.isRequired,
  onFormSubmit: PropTypes.func.isRequired,
};

export default UiSettings;
