import Component from '../components/component.react';
import DocumentTitle from 'react-document-title';
import LoginForm from '../auth/login.react';
import React from 'react';
import {msg} from '../intl/store';

class Login extends Component {

  render() {
    return (
      <DocumentTitle title={msg('pages.auth.title')}>
        <div className="login-page">
          <h2>{msg('pages.auth.header')}</h2>
          <LoginForm {...this.props} />
        </div>
      </DocumentTitle>
    );
  }

}

export default Login;
