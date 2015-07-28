import Component from '../components/component.react';
import DocumentTitle from 'react-document-title';
import LoginForm from '../auth/login.react';
import React from 'react';
import {msg} from '../intl/store';
import Page from '../components/page.react';

class Login extends Component {

  render() {
    return (
      <article className='login'>
        <Page page='login'></Page>
        <LoginForm {...this.props} />
      </article>
    );
  }
}

export default Login;
