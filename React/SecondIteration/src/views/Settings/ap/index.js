import React, { PropTypes } from 'react';

import {onInputChange} from 'utils/inputs';

import classes from './ApSettings.scss';

export const ApSettings =
  ({ssid, onInputChange, onFormSubmit}) =>
    <form
      className={classes['container']}
      onSubmit={onFormSubmit}
    >
      <fieldset>
        <legend>Accesspoint:</legend>

        <h5>Preferred Accesspoint:</h5>
        <ul>
          <li>
            <label forHtml='ssid'>ssid</label>
            <input
              name='ssid'
              type='text'
              value={ssid}
              onChange={onInputChange}
            />
          </li>
          <li>
            <label forHtml='password'>password</label>
            <input
              name='password'
              type='password'
              onChange={onInputChange}
            />
          </li>
          <li>
            <input
              value='save'
              type='submit'
            />
          </li>
        </ul>

        <h5>Saved Accesspoints</h5>

        <ul>
          <li>To be done...</li>
        </ul>

        <h5>Available Accesspoints:</h5>

        <ul>
          <li>To be done...</li>
        </ul>
      </fieldset>
    </form>;

ApSettings.propTypes = {
  ssid: PropTypes.string,
  onFormSubmit: PropTypes.func.isRequired,
  onInputChange: PropTypes.func.isRequired,
};

export default ApSettings;
