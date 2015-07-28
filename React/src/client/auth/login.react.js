import './login.styl';
import * as actions from './actions';
import Component from '../components/component.react';
import React from 'react';
import exposeRouter from '../components/exposerouter.react';
import immutable from 'immutable';
import {focusInvalidField} from '../lib/validation';
import {msg} from '../intl/store';

class Login extends Component {

  static propTypes = {
    auth: React.PropTypes.instanceOf(immutable.Map).isRequired,
    pendingActions: React.PropTypes.instanceOf(immutable.Map).isRequired,
    router: React.PropTypes.func
  };

  getForm() {
    return this.props.auth.get('form');
  }

  onFormSubmit(e) {
    e.preventDefault();
    const fields = this.getForm().fields.toJS();
    actions.login(fields)
      .then(() => {
        this.redirectAfterLogin();
      })
      .catch(focusInvalidField(this));
  }

  redirectAfterLogin() {
    const {router} = this.props;
    const nextPath = router.getCurrentQuery().nextPath;
    router.replaceWith(nextPath || 'home');
  }

  render() {
    const form = this.getForm();
    const {pendingActions} = this.props;

    return (
      <div className='login'>
        <form onSubmit={(e) => this.onFormSubmit(e)}>
          <fieldset disabled={pendingActions.has(actions.login.toString())}>
            <legend><h1>{msg('forms.login.legend')}</h1></legend>
            <input
              autoFocus
              name='email'
              onChange={actions.updateFormField}
              placeholder={msg('forms.login.placeholder.email')}
              value={form.fields.email}
            />
            <br />
            <input
              name='password'
              onChange={actions.updateFormField}
              placeholder={msg('forms.login.placeholder.password')}
              type='password'
              value={form.fields.password}
            />
            <br />
            <button
              children={msg('buttons.login')}
              type='submit'
            />
            {/*
             <button type='submit'>{msg('buttons.signup')} />
            */}
            {form.error &&
              <span className='error-message'>{form.error.message}</span>
            }
            <div>{msg('forms.login.hint')}</div>
          </fieldset>
        </form>
      </div>
    );
  }

}

export default exposeRouter(Login);
