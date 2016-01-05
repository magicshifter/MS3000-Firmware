import * as actions from './actions';
import Component from '../../components/component.react';
import React from 'react';
import immutable from 'immutable';
import {focusInvalidField} from '../../lib/validation';
import {msg} from '../../intl/store';

import styles from '../../styles/form';

export default class ServerForm extends Component {

  static propTypes = {
    serverSettings: React.PropTypes.instanceOf(immutable.Map).isRequired,
    pendingActions: React.PropTypes.instanceOf(immutable.Map).isRequired,
  };

  getForm() {
    return this.props.serverSettings.get('form');
  }

  onFormSubmit(e) {
    e.preventDefault();
    const fields = this.getForm().fields.toJS();
    actions.saveServer(fields)
      .catch(focusInvalidField(this));
  }

  render() {
    const form = this.getForm();
    const {pendingActions} = this.props;

    return (
      <div className='settings-server'>
        <form onSubmit={(e) => this.onFormSubmit(e)} style={styles.form}>
          <fieldset
            style={styles.fieldset}
            disabled={pendingActions.has(actions.saveServer.toString())}
          >
            <legend><h2>{msg('forms.settings.server.legend')}</h2></legend>
            <div style={styles.field}>
              <label htmlFor='hostname'>
                {msg('forms.settings.server.label.hostname')}
              </label>
              <input
                name='hostname'
                autoFocus
                style={styles.input}
                onChange={actions.updateFormField}
                placeholder={msg('forms.settings.server.placeholder.hostname')}
                value={form.fields.hostname}
              />
            </div>
            <div style={styles.field}>
              <label htmlFor='port'>
                {msg('forms.settings.server.label.port')}
              </label>
              <input
                name='port'
                style={styles.input}
                onChange={actions.updateFormField}
                placeholder={msg('forms.settings.server.placeholder.port')}
                type='number'
                value={form.fields.port}
              />
            </div>
            <div style={styles.buttonContainer}>
              <button
                style={styles.button}
                children={msg('buttons.save')}
                type='submit'
              />
              {form.error &&
                <span className='error-message'>{form.error.message}</span>
              }
            </div>
          </fieldset>
        </form>
      </div>
    );
  }
}
