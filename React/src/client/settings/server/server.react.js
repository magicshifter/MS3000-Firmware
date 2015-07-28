import './server.styl';
import * as actions from './actions';
import Component from '../../components/component.react';
import React from 'react';
import immutable from 'immutable';
import {focusInvalidField} from '../../lib/validation';
import {msg} from '../../intl/store';

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
        <form onSubmit={(e) => this.onFormSubmit(e)}>
          <fieldset disabled={pendingActions.has(actions.saveServer.toString())}>
            <legend>{msg('forms.settings.server.legend')}</legend>
            <input
              autoFocus
              name='hostname'
              onChange={actions.updateFormField}
              placeholder={msg('forms.settings.server.placeholder.hostname')}
              value={form.fields.hostname}
            />
            <br />
            <input
              name='port'
              onChange={actions.updateFormField}
              placeholder={msg('forms.settings.server.placeholder.port')}
              type='number'
              value={form.fields.port}
            />
            <button
              children={msg('buttons.save')}
              type='submit'
            />
            {form.error &&
              <span className='error-message'>{form.error.message}</span>
            }
          </fieldset>
        </form>
      </div>
    );
  }

}
